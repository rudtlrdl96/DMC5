#include "PrecompileHeader.h"
#include "ReflectionProbe.h"
#include <GameEngineCore/GameEngineScreenShoot.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

bool ReflectionProbe::RenderTargetInit = false;
GameEngineRenderUnit ReflectionProbe::CubemapMergeTarget;
ReflectionProbe::CubeCaptureData ReflectionProbe::CutData; 
std::shared_ptr<GameEngineRenderTarget> ReflectionProbe::CaptureTarget = nullptr;

ReflectionProbe::ReflectionProbe()
{
	if (false == RenderTargetInit)
	{
		CubemapMergeTarget.SetMesh("FullRect");
		CubemapMergeTarget.SetMaterial("CubemapMerge");
		CubemapMergeTarget.ShaderResHelper.SetConstantBufferLink("CutData", CutData);
		RenderTargetInit = true;
	}
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
			MsgAssert("ReflectionProbe : 캡쳐할 텍스쳐의 크기가 0 입니다");
			return;
		}

		if (nullptr == CaptureTarget)
		{
			CaptureTarget = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, _Scale, float4::ZERONULL);
		}

		float4 CenterPos = GetTransform()->GetWorldPosition();
		float4 CenterRot = GetTransform()->GetWorldRotation();

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

		GameEngineTexture::Load(Path + "\\" + _CaptureTextureName.data() + "_FORWARD.PNG");
		GameEngineTexture::Load(Path + "\\" + _CaptureTextureName.data() + "_RIGHT.PNG");
		GameEngineTexture::Load(Path + "\\" + _CaptureTextureName.data() + "_LEFT.PNG");
		GameEngineTexture::Load(Path + "\\" + _CaptureTextureName.data() + "_BACK.PNG");
		GameEngineTexture::Load(Path + "\\" + _CaptureTextureName.data() + "_TOP.PNG");
		GameEngineTexture::Load(Path + "\\" + _CaptureTextureName.data() + "_BOT.PNG");
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

	IsInitCheck = true;
}

void ReflectionProbe::Start()
{
	// ..ContentResources//Texture//ReflectionTexture//


}

void ReflectionProbe::Update(float _DeltaTime)
{
	if (false == IsInitCheck)
	{
		MsgAssert("ReflectionProbe 컴포넌트의 Init 함수가 호출되지 않았습니다.");
	}
}

void ReflectionProbe::CubemapMerge(const CubeCaptureData& _Cut, std::shared_ptr<GameEngineRenderTarget> _Target, std::shared_ptr<GameEngineRenderTarget> _Merge)
{
	CutData = _Cut;

	_Target->Setting();
	CubemapMergeTarget.ShaderResHelper.SetTexture("DiffuseTex", _Merge->GetTexture(0));
	CubemapMergeTarget.Render(0.0f);
	CubemapMergeTarget.ShaderResHelper.AllResourcesReset();
	_Target->Reset();

	_Merge->Clear();
}