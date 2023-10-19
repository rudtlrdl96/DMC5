#include "PrecompileHeader.h"
#include "BloomEffect.h"

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

	ResultTarget->Clear();
	ResultTarget->Setting();

	BlurUnit->ShaderResHelper.SetTexture("LightTarget", _Target->GetTexture(1));
	BlurUnit->Render(_DeltaTime);
	BlurUnit->ShaderResHelper.AllResourcesReset();

	//DebugTargetA->Clear();
	//DebugTargetA->Merge(ResultTarget);

	_Target->Setting(1);
	
	BloomBlurUnit->ShaderResHelper.SetTexture("SmallBloomTex", ResultTarget->GetTexture(0));
	BloomBlurUnit->Render(_DeltaTime);
	BloomBlurUnit->ShaderResHelper.AllResourcesReset();

	//DebugTargetB->Clear();
	//DebugTargetB->Merge(_Target, 1);
}
