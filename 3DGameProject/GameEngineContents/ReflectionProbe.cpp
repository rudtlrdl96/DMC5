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

		CaptureTarget = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4(900, 900), float4::ZERONULL);

		float4 TextureScale = float4(_Scale.x * 4.0f, _Scale.y * 3.0f);
		TextureTarget = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4(1600, 900), float4::ZERONULL);

		float4 CenterPos = GetTransform()->GetWorldPosition();
		float4 CenterRot = GetTransform()->GetWorldRotation();

		// Forward
		GetLevel()->GetMainCamera()->CaptureCubemap(CaptureTarget, CenterPos, CenterRot, float4(900, 900));
		CutData.CutStartX = 0.25f;
		CutData.CutEndX = 0.5f;
		CutData.CutStartY = 1.0f / 3.0f;
		CutData.CutEndY = 2.0f / 3.0f;
		TextureTarget->Setting();
		CubemapMergeTarget.ShaderResHelper.SetTexture("DiffuseTex", CaptureTarget->GetTexture(0));
		CubemapMergeTarget.Render(0.0f);
		CubemapMergeTarget.ShaderResHelper.AllResourcesReset();
		TextureTarget->Reset();
		
		// Right
		GetLevel()->GetMainCamera()->CaptureCubemap(CaptureTarget, CenterPos, CenterRot + float4(0, 90, 0), float4(900, 900));
		CutData.CutStartX = 0.5f;
		CutData.CutEndX = 0.75f;
		CutData.CutStartY = 1.0f / 3.0f;
		CutData.CutEndY = 2.0f / 3.0f;
		TextureTarget->Setting();
		CubemapMergeTarget.ShaderResHelper.SetTexture("DiffuseTex", CaptureTarget->GetTexture(0));
		CubemapMergeTarget.Render(0.0f);
		CubemapMergeTarget.ShaderResHelper.AllResourcesReset();
		TextureTarget->Reset();
		
		// Left
		GetLevel()->GetMainCamera()->CaptureCubemap(CaptureTarget, CenterPos, CenterRot + float4(0, -90, 0), float4(900, 900));
		CutData.CutStartX = 0.0f;
		CutData.CutEndX = 0.25f;
		CutData.CutStartY = 1.0f / 3.0f;
		CutData.CutEndY = 2.0f / 3.0f;
		TextureTarget->Setting();
		CubemapMergeTarget.ShaderResHelper.SetTexture("DiffuseTex", CaptureTarget->GetTexture(0));
		CubemapMergeTarget.Render(0.0f);
		CubemapMergeTarget.ShaderResHelper.AllResourcesReset();
		TextureTarget->Reset();
		
		// Back
		GetLevel()->GetMainCamera()->CaptureCubemap(CaptureTarget, CenterPos, CenterRot + float4(0, 180, 0), float4(900, 900));
		CutData.CutStartX = 0.75f;
		CutData.CutEndX = 1.0f;
		CutData.CutStartY = 1.0f / 3.0f;
		CutData.CutEndY = 2.0f / 3.0f;
		TextureTarget->Setting();
		CubemapMergeTarget.ShaderResHelper.SetTexture("DiffuseTex", CaptureTarget->GetTexture(0));
		CubemapMergeTarget.Render(0.0f);
		CubemapMergeTarget.ShaderResHelper.AllResourcesReset();
		TextureTarget->Reset();
		
		// Top
		GetLevel()->GetMainCamera()->CaptureCubemap(CaptureTarget, CenterPos, CenterRot + float4(-90, 0, 0), float4(900, 900));
		CutData.CutStartX = 0.25f;
		CutData.CutEndX = 0.5f;
		CutData.CutStartY = 0.0f;
		CutData.CutEndY = 1.0f / 3.0f;
		TextureTarget->Setting();
		CubemapMergeTarget.ShaderResHelper.SetTexture("DiffuseTex", CaptureTarget->GetTexture(0));
		CubemapMergeTarget.Render(0.0f);
		CubemapMergeTarget.ShaderResHelper.AllResourcesReset();
		TextureTarget->Reset();

		// Bottom
		GetLevel()->GetMainCamera()->CaptureCubemap(CaptureTarget, CenterPos, CenterRot + float4(90, 0, 0), float4(900, 900));
		CutData.CutStartX = 0.25f;
		CutData.CutEndX = 0.5f;
		CutData.CutStartY = (2.0f / 3.0f);
		CutData.CutEndY = (1.0f);

		TextureTarget->Setting();
		CubemapMergeTarget.ShaderResHelper.SetTexture("DiffuseTex", CaptureTarget->GetTexture(0));
		CubemapMergeTarget.Render(0.0f);
		CubemapMergeTarget.ShaderResHelper.AllResourcesReset();
		TextureTarget->Reset();

		GameEngineScreenShoot::RenderTargetShoot(TextureTarget->GetTexture(0)->GetTexture2D());
	
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