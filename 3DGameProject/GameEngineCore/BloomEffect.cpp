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
	ResultTarget = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize() * 0.25f, float4::ZERONULL);
	BlurTarget0 = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize() * 0.25f, float4::ZERONULL);
	BlurTarget1 = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize() * 0.25f, float4::ZERONULL);
}

void BloomEffect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	ResultTarget->Clear();
	BlurUnit->ShaderResHelper.SetTexture("LightTarget", _Target->GetTexture(3));
	ResultTarget->Setting();
	BlurUnit->Render(_DeltaTime);
	BlurUnit->ShaderResHelper.AllResourcesReset();

	BlurTarget0->Clear();
	BlurTarget0->Setting();
	BloomBlurUnit->ShaderResHelper.SetTexture("SmallBloomTex", ResultTarget->GetTexture(0));
	BloomBlurUnit->Render(_DeltaTime);
	BloomBlurUnit->ShaderResHelper.AllResourcesReset();

	BlurTarget1->Clear();
	_Target->Setting();
	// _Target->Setting();
	BloomBlurUnit->ShaderResHelper.SetTexture("SmallBloomTex", BlurTarget0->GetTexture(0));
	BloomBlurUnit->Render(_DeltaTime);
	BloomBlurUnit->ShaderResHelper.AllResourcesReset();

	// 
}
