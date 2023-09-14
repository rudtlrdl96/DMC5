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

	Data.ScreenSize = { 1280.0f, 720.0f };
	Data.ScreenRatio.x = 0.33f;

	ResultTarget = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize() * Data.ScreenRatio.x, float4::ZERONULL);
	BlurTarget = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize() * Data.ScreenRatio.x, float4::ZERONULL);
}

void BloomEffect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	ResultTarget->Clear();
	BlurUnit->ShaderResHelper.SetTexture("LightTarget", _Target->GetTexture(1));
	ResultTarget->Setting();
	BlurUnit->Render(_DeltaTime);
	BlurUnit->ShaderResHelper.AllResourcesReset();

	BlurTarget->Clear();
	BlurTarget->Setting();
	BloomBlurUnit->ShaderResHelper.SetTexture("SmallBloomTex", ResultTarget->GetTexture(0));
	BloomBlurUnit->Render(_DeltaTime);
	BloomBlurUnit->ShaderResHelper.AllResourcesReset();

	_Target->Setting();
	BloomBlurUnit->ShaderResHelper.SetTexture("SmallBloomTex", BlurTarget->GetTexture(0));
	BloomBlurUnit->Render(_DeltaTime);
	BloomBlurUnit->ShaderResHelper.AllResourcesReset();
}
