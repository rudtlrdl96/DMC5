#include "PrecompileHeader.h"
#include "GameEngineLight.h"
#include "GameEngineLevel.h"

// 쉐이더에서 Light.fx를 이용하는 경우 이녀석은 빛을 사용하는 녀석이다

GameEngineLight::GameEngineLight()
{
	SetName("GameEngineLight");

	LightDataValue.ShadowTargetSizeX = 1024;
	LightDataValue.ShadowTargetSizeY = 1024;
	LightDataValue.LightNear = 0.1f;
	LightDataValue.LightFar = 5000.1f;
	ShadowRange.x = 1024.0f;
	ShadowRange.y = 1024.0f;
}

GameEngineLight::~GameEngineLight()
{
}

void GameEngineLight::Start()
{
	GetLevel()->PushLight(DynamicThis<GameEngineLight>());

	// 크기가 곧 그림자가 맺히는 범위와 디테일을 의미하게 됩니다.
	ShadowTarget = GameEngineRenderTarget::Create();
	BakeShadowTarget[0] = GameEngineRenderTarget::Create();
}

void GameEngineLight::Update(float _DeltaTime)
{
	if (true == IsDebugDraw)
	{
		GetTransform()->SetWorldScale(float4(LightDataValue.LightRange, LightDataValue.LightRange, LightDataValue.LightRange));
		GameEngineDebug::DrawSphere(GetLevel()->GetCamera(0).get(), GetTransform(), float4::GREEN);
	}
}

void GameEngineLight::ShadowTargetTextureLoad(const float4 _ShadowScale /*= float4::ZERO*/)
{
	if (nullptr == ShadowTarget)
	{
		return;
	}

	if (false == IsShadowLight)
	{
		return;
	}

	if (_ShadowScale != float4::ZERO)
	{
		LightDataValue.ShadowTargetSizeX = _ShadowScale.x;
		LightDataValue.ShadowTargetSizeY = _ShadowScale.y;
	}

	CreateShadowTarget(ShadowTarget, static_cast<LightType>(LightDataValue.LightType));
	ShadowTarget->SetName("Shadow Target");

	for (std::pair<const int, std::shared_ptr<class GameEngineRenderTarget>>& BakeTarget : BakeShadowTarget)
	{
		if (nullptr != BakeTarget.second)
		{
			CreateShadowTarget(BakeTarget.second, static_cast<LightType>(LightDataValue.LightType));
			BakeTarget.second->SetName("Bake Target : " + std::to_string(BakeTarget.first));
			BakeTarget.second->Clear();
		}
	}

}

void GameEngineLight::ShadowTargetTextureRelease()
{
	if (nullptr != ShadowTarget)
	{
		ShadowTarget->ReleaseTextures();
	}

	for (std::pair<const int, std::shared_ptr<class GameEngineRenderTarget>>& BakeTarget : BakeShadowTarget)
	{
		if (nullptr != BakeTarget.second)
		{
			BakeTarget.second->ReleaseTextures();
		}
	}
}

void GameEngineLight::BakeShadow(std::shared_ptr<GameEngineCamera> _BakeCam, int _BakeIndex /*= 0*/)
{
	std::shared_ptr<GameEngineRenderTarget> NewBakeTarget = nullptr;

	if (true == BakeShadowTarget.contains(_BakeIndex))
	{
		NewBakeTarget = BakeShadowTarget[_BakeIndex];
	}

	if (nullptr == NewBakeTarget)
	{
		NewBakeTarget->AddNewTexture(DXGI_FORMAT_R16_FLOAT, { LightDataValue.ShadowTargetSizeX, LightDataValue.ShadowTargetSizeY }, float4::RED);
		//NewBakeTarget->CreateDepthTexture();
		NewBakeTarget->SetName("Bake Target : " + std::to_string(_BakeIndex));
	}

	_BakeCam->BakeShadow(DynamicThis<GameEngineLight>(), _BakeIndex);

	BakeShadowTarget[_BakeIndex] = NewBakeTarget;
}

void GameEngineLight::LightUpdate(GameEngineCamera* _Camera, float _DeltaTime)
{

	LightDataValue.LightPos = GetTransform()->GetWorldPosition();
	LightDataValue.LightDir = GetTransform()->GetLocalForwardVector();
	LightDataValue.ViewLightPos = LightDataValue.LightPos * _Camera->GetView();
	LightDataValue.ViewLightDir = LightDataValue.LightDir * _Camera->GetView();
	LightDataValue.ViewLightRevDir = -LightDataValue.ViewLightDir;
	LightDataValue.CameraPosition = _Camera->GetTransform()->GetWorldPosition() * _Camera->GetView();
	LightDataValue.CameraWorldPosition = _Camera->GetTransform()->GetWorldPosition();	

	_Camera->GetTransform()->SetCameraMatrix(_Camera->GetView(), _Camera->GetProjection());


	LightDataValue.LightViewMatrix.LookToLH(
		GetTransform()->GetWorldPosition(),
		GetTransform()->GetWorldForwardVector(),
		GetTransform()->GetWorldUpVector());


	if (LightDataValue.LightType == static_cast<int>(LightType::Directional))
	{
		LightDataValue.LightProjectionMatrix.OrthographicLH(
			ShadowRange.x,
			ShadowRange.y,
			LightDataValue.LightNear,
			LightDataValue.LightFar);
	}
	else
	{
		LightDataValue.LightProjectionMatrix.PerspectiveFovLH(
			90.0f,
			ShadowRange.x / ShadowRange.y,
			LightDataValue.LightNear,
			LightDataValue.LightFar);
	}

	LightDataValue.CameraView = _Camera->GetView();
	LightDataValue.CameraViewInverseMatrix = _Camera->GetView().InverseReturn();

	LightDataValue.LightProjectionInverseMatrix = LightDataValue.LightProjectionMatrix.InverseReturn();
	LightDataValue.LightViewProjectionMatrix = LightDataValue.LightViewMatrix * LightDataValue.LightProjectionMatrix;
}


void GameEngineLight::CreateShadowTarget(std::shared_ptr<class GameEngineRenderTarget> _Target, LightType _Type)
{
	switch (_Type)
	{
	case LightType::Point:
	{
		_Target->AddNewCubeTexture(DXGI_FORMAT_R16_FLOAT, { LightDataValue.ShadowTargetSizeX, LightDataValue.ShadowTargetSizeY }, float4::RED);
	}
	break;
	default:
	{
		_Target->AddNewTexture(DXGI_FORMAT_R16_FLOAT, { LightDataValue.ShadowTargetSizeX, LightDataValue.ShadowTargetSizeY }, float4::RED);
	}
	break;
	}
}

#include <GameEngineCore/imgui.h>

void GameEngineLight::DrawEditor()
{
	ImGui::Spacing();
	ImGui::Spacing();

	float LightColor[3] = { LightDataValue.LightColor.x, LightDataValue.LightColor.y, LightDataValue.LightColor.z };

	ImGui::DragFloat3("Light Color", LightColor, 0.01f);

	LightDataValue.LightColor.x = LightColor[0];
	LightDataValue.LightColor.y = LightColor[1];
	LightDataValue.LightColor.z = LightColor[2];

	switch (LightDataValue.LightType)
	{
	case 0: // Directional
	{
	}
		break;

	case 1: // Point
	{
		ImGui::DragFloat("Light Range", &LightDataValue.LightRange, 1.0f);	
	}
		break;

	case 2: // Spot
	{
		ImGui::DragFloat("Light Range", &LightDataValue.LightRange, 1.0f);
		ImGui::DragFloat("Light Angle", &LightDataValue.LightAngle, 0.1f);
	}
		break;
	default:
		break;
	}
}