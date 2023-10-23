#include "PrecompileHeader.h"
#include "ReflectionProbe.h"
#include <GameEngineCore/GameEngineScreenShoot.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

std::shared_ptr<GameEngineRenderTarget> ReflectionProbe::CaptureTarget = nullptr;

ReflectionProbe::ReflectionProbe()
{
}

ReflectionProbe::~ReflectionProbe()
{
}

void ReflectionProbe::Init(const std::string_view& _CaptureTextureName, const float4& _Scale /*= float4(128, 128)*/)
{
	std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
		{
			"Texture", "ReflectionTexture"
		});

	if (nullptr == GameEngineTexture::Find(_CaptureTextureName.data() + std::string("_FORWARD.PNG")))
	{
		if (0 == _Scale.x || 0 == _Scale.y)
		{
			MsgAssert("ReflectionProbe : ĸ���� �ؽ����� ũ�Ⱑ 0 �Դϴ�");
			return;
		}

		if (nullptr == CaptureTarget)
		{
			CaptureTarget = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_UNORM, _Scale, float4::ZERONULL);
		}

		float4 CenterPos = GetTransform()->GetWorldPosition();
		float4 CenterRot = float4::ZERO;

		// Forward
		GetLevel()->GetMainCamera()->CaptureCubemap(CenterPos, CenterRot, float4(900, 900));
		CaptureTarget->Clear();
		CaptureTarget->Merge(GetLevel()->GetMainCamera()->GetCamTarget());
		GameEngineScreenShoot::RenderTargetShoot_DxTex(CaptureTarget, Path, _CaptureTextureName.data() + std::string("_FORWARD.PNG"));
		
		// Right
		GetLevel()->GetMainCamera()->CaptureCubemap(CenterPos, CenterRot + float4(0, 90, 0), float4(900, 900));
		CaptureTarget->Clear();
		CaptureTarget->Merge(GetLevel()->GetMainCamera()->GetCamTarget());
		GameEngineScreenShoot::RenderTargetShoot_DxTex(CaptureTarget, Path, _CaptureTextureName.data() + std::string("_RIGHT.PNG"));

		// Left
		GetLevel()->GetMainCamera()->CaptureCubemap(CenterPos, CenterRot + float4(0, -90, 0), float4(900, 900));
		CaptureTarget->Clear();
		CaptureTarget->Merge(GetLevel()->GetMainCamera()->GetCamTarget());
		GameEngineScreenShoot::RenderTargetShoot_DxTex(CaptureTarget, Path, _CaptureTextureName.data() + std::string("_LEFT.PNG"));

		// Back
		GetLevel()->GetMainCamera()->CaptureCubemap(CenterPos, CenterRot + float4(0, 180, 0), float4(900, 900));
		CaptureTarget->Clear();
		CaptureTarget->Merge(GetLevel()->GetMainCamera()->GetCamTarget());
		GameEngineScreenShoot::RenderTargetShoot_DxTex(CaptureTarget, Path, _CaptureTextureName.data() + std::string("_BACK.PNG"));

		// Top
		GetLevel()->GetMainCamera()->CaptureCubemap(CenterPos, CenterRot + float4(-90, 0, 0), float4(900, 900));
		CaptureTarget->Clear();
		CaptureTarget->Merge(GetLevel()->GetMainCamera()->GetCamTarget());
		GameEngineScreenShoot::RenderTargetShoot_DxTex(CaptureTarget, Path, _CaptureTextureName.data() + std::string("_TOP.PNG"));

		// Bottom
		GetLevel()->GetMainCamera()->CaptureCubemap(CenterPos, CenterRot + float4(90, 0, 0), float4(900, 900));
		CaptureTarget->Clear();
		CaptureTarget->Merge(GetLevel()->GetMainCamera()->GetCamTarget());
		GameEngineScreenShoot::RenderTargetShoot_DxTex(CaptureTarget, Path, _CaptureTextureName.data() + std::string("_BOT.PNG"));
		CaptureTarget->Clear();

		GameEngineTexture::Load(Path + "\\" + _CaptureTextureName.data() + "_RIGHT.PNG");
		GameEngineTexture::Load(Path + "\\" + _CaptureTextureName.data() + "_LEFT.PNG");
		GameEngineTexture::Load(Path + "\\" + _CaptureTextureName.data() + "_TOP.PNG");
		GameEngineTexture::Load(Path + "\\" + _CaptureTextureName.data() + "_BOT.PNG");
		GameEngineTexture::Load(Path + "\\" + _CaptureTextureName.data() + "_FORWARD.PNG");
		GameEngineTexture::Load(Path + "\\" + _CaptureTextureName.data() + "_BACK.PNG");
	}

	ReflectionCubeTexture = GameEngineTexture::Find(_CaptureTextureName);

	if (nullptr == ReflectionCubeTexture)
	{
		std::vector<std::shared_ptr<GameEngineTexture>> CubeTextures;
		CubeTextures.reserve(6);

		CubeTextures.push_back(GameEngineTexture::Find(_CaptureTextureName.data() + std::string("_RIGHT.PNG")));
		CubeTextures.push_back(GameEngineTexture::Find(_CaptureTextureName.data() + std::string("_LEFT.PNG")));
		CubeTextures.push_back(GameEngineTexture::Find(_CaptureTextureName.data() + std::string("_TOP.PNG")));
		CubeTextures.push_back(GameEngineTexture::Find(_CaptureTextureName.data() + std::string("_BOT.PNG")));
		CubeTextures.push_back(GameEngineTexture::Find(_CaptureTextureName.data() + std::string("_FORWARD.PNG")));
		CubeTextures.push_back(GameEngineTexture::Find(_CaptureTextureName.data() + std::string("_BACK.PNG")));

		ReflectionCubeTexture = GameEngineTexture::Create(_CaptureTextureName, CubeTextures);
	}
}

void ReflectionProbe::Start()
{
}

void ReflectionProbe::Update(float _DeltaTime)
{
}