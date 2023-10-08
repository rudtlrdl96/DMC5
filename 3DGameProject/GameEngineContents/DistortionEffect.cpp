#include "PrecompileHeader.h"
#include "DistortionEffect.h"

DistortionEffect::DistortionEffect()
{
}

DistortionEffect::~DistortionEffect()
{
}

void DistortionEffect::Start(GameEngineRenderTarget* _Target)
{
	DistortionUnit = std::make_shared<GameEngineRenderUnit>();
	DistortionUnit->SetMesh("FullRect");
	DistortionUnit->SetMaterial("Distortion");

	Option.ScreenSize = GameEngineWindow::GetScreenSize() * 0.1f;
	DistortionUnit->ShaderResHelper.SetConstantBufferLink("DistortionOption", Option);

	ResultTarget = GameEngineRenderTarget::Create();
}

void DistortionEffect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	if (nullptr == MaskTarget)
	{
		return;
	}

	ResultTarget->Clear();

	DistortionUnit->ShaderResHelper.SetTexture("DiffuseTex", _Target->GetTexture(0));
	DistortionUnit->ShaderResHelper.SetTexture("MaskTex", MaskTarget->GetTexture(static_cast<int>(MaskTexIndex)));

	ResultTarget->Setting();

	DistortionUnit->Render(_DeltaTime);
	DistortionUnit->ShaderResHelper.AllResourcesReset();

	_Target->Clear();
	_Target->Merge(ResultTarget);
}