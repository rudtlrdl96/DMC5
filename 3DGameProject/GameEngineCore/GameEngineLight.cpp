#include "PrecompileHeader.h"
#include "GameEngineLight.h"
#include "GameEngineLevel.h"

// 쉐이더에서 Light.fx를 이용하는 경우 이녀석은 빛을 사용하는 녀석이다

const float4 GameEngineLight::PointViewDatas[6][2] = {
	{{1, 0, 0},  {0, 1, 0}},	// Right
	{{-1, 0, 0}, {0, 1, 0}},	// Left
	{{0, 1, 0},  {0, 0, -1}},	// Top
	{{0, -1, 0}, {0, 0, 1}},	// Bot
	{{0, 0, 1},  {0, 1, 0}},	// Forward
	{{0, 0, -1}, {0, -1, 0}},	// Back
};
/// 0 == EyeDIr, 1 == UpDIr


const float4 GameEngineLight::ShadowTextureScales[9] =
{
	{64, 64},
	{128, 128},
	{256, 256},
	{512, 512},
	{1024, 1024},
	{2048, 2048},
	{4096, 4096},
	{8192, 8192},
	{16384, 16384}
};

const std::string GameEngineLight::ShadowTextureEnums[9] =
{
	"64",
	"128",
	"256",
	"512",
	"1024",
	"2048",
	"4096",
	"8192",
	"16384",
};

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
		//GetTransform()->SetWorldScale(float4(LightDataValue.LightRange, LightDataValue.LightRange, LightDataValue.LightRange));
		GetTransform()->SetWorldScale(float4(100.f, 100.f, 100.f));
		GameEngineDebug::DrawSphere(GetLevel()->GetCamera(0).get(), GetTransform(), float4::GREEN);
	}
}

void GameEngineLight::ShadowTargetTextureLoad(ShadowTextureScale _Scale /*= ShadowTextureScale::S_256*/)
{
	if (nullptr == ShadowTarget)
	{
		return;
	}

	if (false == IsShadowLight)
	{
		return;
	}

	ShadowTextureScaleValue = _Scale;

	LightDataValue.ShadowTargetSizeX = ShadowTextureScales[static_cast<int>(_Scale)].x;
	LightDataValue.ShadowTargetSizeY = ShadowTextureScales[static_cast<int>(_Scale)].y;

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
		NewBakeTarget = GameEngineRenderTarget::Create();
		CreateShadowTarget(NewBakeTarget, static_cast<LightType>(LightDataValue.LightType));
		NewBakeTarget->SetName("Bake Target : " + std::to_string(_BakeIndex));
	}

	_BakeCam->BakeShadow(DynamicThis<GameEngineLight>(), _BakeIndex);

	BakeShadowTarget[_BakeIndex] = NewBakeTarget;
}

void GameEngineLight::SetShadowTextureScale(ShadowTextureScale _Scale)
{
	ShadowTargetTextureRelease();
	ShadowTargetTextureLoad(_Scale);
}

void GameEngineLight::SetLightRange(float _Range)
{
	LightDataValue.LightNear = 0.1f;
	LightDataValue.LightFar = _Range;
	LightDataValue.LightRange = _Range;
}

void GameEngineLight::LightUpdate(GameEngineCamera* _Camera, float _DeltaTime)
{
	LightDataValue.LightPos = GetTransform()->GetWorldPosition();
	LightDataValue.LightDir = GetTransform()->GetWorldForwardVector();
	LightDataValue.ViewLightPos = LightDataValue.LightPos * _Camera->GetView();
	LightDataValue.ViewLightDir = LightDataValue.LightDir * _Camera->GetView();
	LightDataValue.ViewLightRevDir = -LightDataValue.ViewLightDir;
	LightDataValue.CameraPosition = _Camera->GetTransform()->GetWorldPosition() * _Camera->GetView();
	LightDataValue.CameraWorldPosition = _Camera->GetTransform()->GetWorldPosition();	

	_Camera->GetTransform()->SetCameraMatrix(_Camera->GetView(), _Camera->GetProjection());

	if (LightDataValue.LightType == static_cast<int>(LightType::Directional))
	{
		if (1 != ViewDatas.size())
		{
			ViewDatas.resize(1);
		}

		ViewDatas[0].LightViewMatrix.LookToLH(
			GetTransform()->GetWorldPosition(),
			GetTransform()->GetWorldForwardVector(),
			GetTransform()->GetWorldUpVector());

		ViewDatas[0].LightViewInverseMatrix = ViewDatas[0].LightViewMatrix.InverseReturn();

		ViewDatas[0].LightProjectionMatrix.OrthographicLH(
			ShadowRange.x,
			ShadowRange.y,
			LightDataValue.LightNear,
			LightDataValue.LightFar * 2.0f);

		ViewDatas[0].LightProjectionInverseMatrix = ViewDatas[0].LightProjectionMatrix.InverseReturn();

		ViewDatas[0].LightViewProjectionMatrix = ViewDatas[0].LightViewMatrix * ViewDatas[0].LightProjectionMatrix;
	}
	else if (LightDataValue.LightType == static_cast<int>(LightType::Point))
	{
		if (6 != ViewDatas.size())
		{
			ViewDatas.resize(6);
		}

		for (size_t i = 0; i < 6; i++)
		{
			float4 WorldPos = GetTransform()->GetWorldPosition();
			float4 EyeDir = PointViewDatas[i][0];
			float4 UpDir = PointViewDatas[i][1];

			ViewDatas[i].LightViewMatrix.LookToLH(
				WorldPos,
				EyeDir,
				UpDir);

			ViewDatas[i].LightViewInverseMatrix = ViewDatas[i].LightViewMatrix.InverseReturn();

			ViewDatas[i].LightProjectionMatrix.PerspectiveFovLH(
				90.0f,
				1.0f,
				LightDataValue.LightNear,
				LightDataValue.LightFar * 2.0f);

			ViewDatas[i].LightProjectionInverseMatrix = ViewDatas[i].LightProjectionMatrix.InverseReturn();
			ViewDatas[i].LightViewProjectionMatrix = ViewDatas[i].LightViewMatrix * ViewDatas[i].LightProjectionMatrix;
		}
	}
	else // Spot
	{
		if (1 != ViewDatas.size())
		{
			ViewDatas.resize(1);
		}

		float LightAngle = LightDataValue.LightAngle;

		if (1 > LightAngle)
		{
			LightAngle = 1;
		}

		if (1 == ViewDatas.size())
		{
			ViewDatas.resize(1);
		}

		ViewDatas[0].LightViewMatrix.LookToLH(
			GetTransform()->GetWorldPosition(),
			GetTransform()->GetWorldForwardVector(),
			GetTransform()->GetWorldUpVector());

		ViewDatas[0].LightViewInverseMatrix = ViewDatas[0].LightViewMatrix.InverseReturn();

		ViewDatas[0].LightProjectionMatrix.PerspectiveFovLH(
			LightAngle,
			1.0f,
			LightDataValue.LightNear,
			LightDataValue.LightFar);

		ViewDatas[0].LightProjectionInverseMatrix = ViewDatas[0].LightProjectionMatrix.InverseReturn();

		ViewDatas[0].LightViewProjectionMatrix = ViewDatas[0].LightViewMatrix * ViewDatas[0].LightProjectionMatrix;
	}

	LightViewSetting(0);

	LightDataValue.CameraView = _Camera->GetView();
	LightDataValue.CameraViewInverseMatrix = _Camera->GetView().InverseReturn();
}


void GameEngineLight::LightViewSetting(size_t _Index)
{
	if (ViewDatas.size() <= _Index)
	{
		MsgAssert("Light View Data를 초과해 세팅하려 했습니다");
		_Index = 0;
	}

	LightDataValue.LightViewMatrix = ViewDatas[_Index].LightViewMatrix;
	LightDataValue.LightViewInverseMatrix = ViewDatas[_Index].LightViewInverseMatrix;
	LightDataValue.LightProjectionMatrix = ViewDatas[_Index].LightProjectionMatrix;
	LightDataValue.LightProjectionInverseMatrix = ViewDatas[_Index].LightProjectionInverseMatrix;
	LightDataValue.LightViewProjectionMatrix = ViewDatas[_Index].LightViewProjectionMatrix;
}

void GameEngineLight::CreateShadowTarget(std::shared_ptr<class GameEngineRenderTarget> _Target, LightType _Type)
{
	switch (_Type)
	{
	case LightType::Point:
	{
		_Target->AddNewCubeTexture(DXGI_FORMAT_R16_FLOAT, { LightDataValue.ShadowTargetSizeX, LightDataValue.ShadowTargetSizeY }, float4(100000, 0, 0, 1.0f));
	}
	break;

	case LightType::Spot:
	{
		_Target->AddNewTexture(DXGI_FORMAT_R16_FLOAT, { LightDataValue.ShadowTargetSizeX, LightDataValue.ShadowTargetSizeY }, float4(100000, 0, 0, 1.0f));	
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
	
	if (true == ImGui::BeginCombo("Shaodw Texture Scale", "Change Scale", 0))
	{
		ShadowTextureScale CurScale = ShadowTextureScaleValue;

		if (ImGui::Selectable("64"))
		{
			ShadowTextureScaleValue = ShadowTextureScale::S_64;
		}
		if (ImGui::Selectable("128"))
		{
			ShadowTextureScaleValue = ShadowTextureScale::S_128;
		}
		if (ImGui::Selectable("256"))
		{
			ShadowTextureScaleValue = ShadowTextureScale::S_256;
		}
		if (ImGui::Selectable("512"))
		{
			ShadowTextureScaleValue = ShadowTextureScale::S_512;
		}
		if (ImGui::Selectable("1024"))
		{
			ShadowTextureScaleValue = ShadowTextureScale::S_1024;
		}
		if (ImGui::Selectable("2048"))
		{
			ShadowTextureScaleValue = ShadowTextureScale::S_2048;
		}
		if (ImGui::Selectable("4096"))
		{
			ShadowTextureScaleValue = ShadowTextureScale::S_4096;
		}
		if (ImGui::Selectable("8192"))
		{
			ShadowTextureScaleValue = ShadowTextureScale::S_8192;
		}		
		if (ImGui::Selectable("16384"))
		{
			ShadowTextureScaleValue = ShadowTextureScale::S_16384;
		}

		if (CurScale != ShadowTextureScaleValue)
		{
			SetShadowTextureScale(ShadowTextureScaleValue);
		}

		ImGui::EndCombo();
	}

	ImGui::Spacing();
	ImGui::Spacing();

	float LightColor[3] = { LightDataValue.LightColor.x, LightDataValue.LightColor.y, LightDataValue.LightColor.z };

	ImGui::DragFloat3("Light Color", LightColor, 0.01f);

	LightDataValue.LightColor.x = LightColor[0];
	LightDataValue.LightColor.y = LightColor[1];
	LightDataValue.LightColor.z = LightColor[2];

	{
		float InputRange = LightDataValue.LightRange;
		ImGui::DragFloat("Light Range", &InputRange);
		SetLightRange(InputRange);
	}

	switch (LightDataValue.LightType)
	{
	case 0: // Directional
	{
	}
		break;

	case 1: // Point
	{
	}
		break;

	case 2: // Spot
	{
		float InputAngle = LightDataValue.LightAngle;
		ImGui::DragFloat("Light Angle", &InputAngle);
		SetLightAngle(InputAngle);
	}
		break;
	default:
		break;
	}

	ImGui::Spacing();
	ImGui::Spacing();

	{
		float InputLightPower = LightDataValue.LightPower;
		ImGui::DragFloat("Light Power", &InputLightPower);
		SetLightPower(InputLightPower);

		float InputDifLightPower = LightDataValue.DifLightPower;
		ImGui::DragFloat("Diffuse Light Power", &InputDifLightPower);
		SetDifLightPower(InputDifLightPower);

		float InputSpcLightPower = LightDataValue.SpcLightPower;
		ImGui::DragFloat("Spacular Light Power", &InputSpcLightPower);
		SetSpcLightPower(InputSpcLightPower);

		float InputAmbLightPower = LightDataValue.AmbLightPower;
		ImGui::DragFloat("Ambient Light Power", &InputAmbLightPower);
		SetAmbLightPower(InputAmbLightPower);
	}
}