#include "PrecompileHeader.h"
#include "BloomEffect.h"
#include "GameEngineOption.h"

BloomEffect::BloomEffect()
{
}

BloomEffect::~BloomEffect()
{
}

void BloomEffect::Start(GameEngineRenderTarget* _Target)
{
	BlurUnit = std::make_shared<GameEngineRenderUnit>();
	BlurUnit->SetMesh("FullRect");
	BlurUnit->SetMaterial("Bloom");

	BloomBlurUnit = std::make_shared<GameEngineRenderUnit>();
	BloomBlurUnit->SetMesh("FullRect");
	BloomBlurUnit->SetMaterial("BloomBlur");	
	
	BloomMergeUnit = std::make_shared<GameEngineRenderUnit>();
	BloomMergeUnit->SetMesh("FullRect");
	BloomMergeUnit->SetMaterial("MaxMerge");

	BloomBlurUnit->ShaderResHelper.SetConstantBufferLink("BlurData", Data);

	Data.ScreenSize = GameEngineWindow::GetScreenSize();

	Data.ScreenRatio.x = 2.0f / Data.ScreenSize.x;
	Data.ScreenRatio.y = 2.0f / Data.ScreenSize.y;

	ResultTarget = GameEngineRenderTarget::Create();
	//DebugTargetA = GameEngineRenderTarget::Create();
	//DebugTargetB = GameEngineRenderTarget::Create();
}

void BloomEffect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	if (GameEngineOption::GetOption("PostEffect") == GameEngineOptionValue::None)
	{
		return;
	}

	ResultTarget->Clear();
	ResultTarget->Setting();

	//BlurUnit->ShaderResHelper.SetTexture("LightTarget", _Target->GetTexture(1));
	//BlurUnit->Render(_DeltaTime);
	//BlurUnit->ShaderResHelper.AllResourcesReset();

	BloomBlurUnit->ShaderResHelper.SetTexture("SmallBloomTex", _Target->GetTexture(1));
	BloomBlurUnit->Render(_DeltaTime);
	BloomBlurUnit->ShaderResHelper.AllResourcesReset();

	_Target->Setting(1);
	BloomMergeUnit->ShaderResHelper.SetTexture("DiffuseTex", ResultTarget->GetTexture(0));
	BloomMergeUnit->Render(0.0f);
	BloomMergeUnit->ShaderResHelper.AllResourcesReset();
}
