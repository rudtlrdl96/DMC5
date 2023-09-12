#include "PrecompileHeader.h"
#include "ReflectionProbe.h"
#include <GameEngineCore/GameEngineScreenShoot.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

bool ReflectionProbe::RenderTargetInit = false;
GameEngineRenderUnit ReflectionProbe::CubemapMergeTarget;
ReflectionProbe::CubeCaptureData ReflectionProbe::CutData; 

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
	if (nullptr != CaptureTarget)
	{
		CaptureTarget->ReleaseTextures();
		CaptureTarget = nullptr;
	}

	if (nullptr != TextureTarget)
	{
		TextureTarget->ReleaseTextures();
		TextureTarget = nullptr;
	}
}

void ReflectionProbe::Init(const std::string_view& _CaptureTextureName, const float4& _Scale /*= float4(128, 128)*/)
{
	if (nullptr == GameEngineTexture::Find(_CaptureTextureName))
	{
		if (0 == _Scale.x || 0 == _Scale.y)
		{
			MsgAssert("ReflectionProbe : 캡쳐할 텍스쳐의 크기가 0 입니다");
			return;
		}

		CaptureTarget = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, _Scale, float4::ZERONULL);

		float4 TextureSacle = float4(_Scale.x * 4.0f, _Scale.y * 3.0f);
		TextureTarget = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, TextureSacle, float4::ZERONULL);

		float4 CenterPos = GetTransform()->GetWorldPosition();
		float4 CenterRot = GetTransform()->GetWorldRotation();

		// Forward
		GetLevel()->GetMainCamera()->CaptureCubemap(CenterPos, CenterRot, float4(900, 900));
		CaptureTarget->Merge(GetLevel()->GetMainCamera()->GetCamTarget());
		CubemapMerge({ 0.25f, 0.5f, (1.0f / 3.0f), (2.0f / 3.0f) }, TextureTarget, CaptureTarget);

		// Right
		GetLevel()->GetMainCamera()->CaptureCubemap(CenterPos, CenterRot + float4(0, 90, 0), float4(900, 900));
		CaptureTarget->Merge(GetLevel()->GetMainCamera()->GetCamTarget());
		CubemapMerge({ 0.5f, 0.75f, (1.0f / 3.0f), (2.0f / 3.0f) }, TextureTarget, CaptureTarget);

		// Left
		GetLevel()->GetMainCamera()->CaptureCubemap(CenterPos, CenterRot + float4(0, -90, 0), float4(900, 900));
		CaptureTarget->Merge(GetLevel()->GetMainCamera()->GetCamTarget());
		CubemapMerge({0.0f, 0.25f, (1.0f / 3.0f), (2.0f / 3.0f) }, TextureTarget, CaptureTarget);

		// Back
		GetLevel()->GetMainCamera()->CaptureCubemap(CenterPos, CenterRot + float4(0, 180, 0), float4(900, 900));
		CaptureTarget->Merge(GetLevel()->GetMainCamera()->GetCamTarget());
		CubemapMerge({ 0.75f, 1.0f, (1.0f / 3.0f), (2.0f / 3.0f) }, TextureTarget, CaptureTarget);

		// Top
		GetLevel()->GetMainCamera()->CaptureCubemap(CenterPos, CenterRot + float4(-90, 0, 0), float4(900, 900));
		CaptureTarget->Merge(GetLevel()->GetMainCamera()->GetCamTarget());
		CubemapMerge({ 0.25f, 0.5f, 0.0f, (1.0f / 3.0f)}, TextureTarget, CaptureTarget);

		// Bottom
		GetLevel()->GetMainCamera()->CaptureCubemap(CenterPos, CenterRot + float4(90, 0, 0), float4(900, 900));
		CaptureTarget->Merge(GetLevel()->GetMainCamera()->GetCamTarget());
		CubemapMerge({ 0.25f, 0.5f, (2.0f / 3.0f), 1.0f}, TextureTarget, CaptureTarget);

		{
			std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
				{
					"Texture", "ReflectionTexture"
				});

			GameEngineScreenShoot::RenderTargetCubemapShoot(TextureTarget, _Scale, Path, _CaptureTextureName);
		}
	
		GameEngineCoreWindow::Clear();
		GameEngineCoreWindow::AddDebugRenderTarget(0, "AllRenderTarget", GetLevel()->GetMainCamera()->GetCamAllRenderTarget());
		GameEngineCoreWindow::AddDebugRenderTarget(1, "LightRenderTarget", GetLevel()->GetMainCamera()->GetDeferredLightTarget());
		GameEngineCoreWindow::AddDebugRenderTarget(2, "MainCameraForwardTarget", GetLevel()->GetMainCamera()->GetCamForwardTarget());
		GameEngineCoreWindow::AddDebugRenderTarget(3, "DeferredTarget", GetLevel()->GetMainCamera()->GetCamDeferrdTarget());
		GameEngineCoreWindow::AddDebugRenderTarget(4, "CaptureCube", TextureTarget);
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