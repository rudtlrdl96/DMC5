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
}

GameEngineLight::~GameEngineLight()
{
}

void GameEngineLight::Start()
{
	GetLevel()->PushLight(DynamicThis<GameEngineLight>());

	// 크기가 곧 그림자가 맺히는 범위와 디테일을 의미하게 됩니다.
	ShadowTarget = GameEngineRenderTarget::Create();
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


	ShadowTarget->AddNewTexture(DXGI_FORMAT_R16_FLOAT, { LightDataValue.ShadowTargetSizeX, LightDataValue.ShadowTargetSizeY }, float4::RED);
	ShadowTarget->CreateDepthTexture();
	ShadowTarget->SetName("Shadow Target");

	for (std::pair<const int, std::shared_ptr<class GameEngineRenderTarget>>& BakeTarget : BakeShadowTarget)
	{
		if (nullptr != BakeTarget.second)
		{
			BakeTarget.second->AddNewTexture(DXGI_FORMAT_R16_FLOAT, { LightDataValue.ShadowTargetSizeX, LightDataValue.ShadowTargetSizeY }, float4::RED);
			BakeTarget.second->CreateDepthTexture();
			BakeTarget.second->SetName("Bake Target : " + std::to_string(BakeTarget.first));
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
		NewBakeTarget->CreateDepthTexture();
		NewBakeTarget->SetName("Bake Target : " + std::to_string(_BakeIndex));
	}

	// Render Bake...
	// _BakeCam

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

	LightDataValue.LightProjectionMatrix.OrthographicLH(
		LightDataValue.ShadowTargetSizeX,
		LightDataValue.ShadowTargetSizeY,
		LightDataValue.LightNear,
		LightDataValue.LightFar);

	LightDataValue.CameraViewInverseMatrix = _Camera->GetView().InverseReturn();
	LightDataValue.LightProjectionInverseMatrix = LightDataValue.LightProjectionMatrix.InverseReturn();
	LightDataValue.LightViewProjectionMatrix = LightDataValue.LightViewMatrix * LightDataValue.LightProjectionMatrix;
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