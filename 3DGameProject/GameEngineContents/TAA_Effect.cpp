#include "PrecompileHeader.h"
#include "TAA_Effect.h"

TAA_Effect::TAA_Effect()
{
}

TAA_Effect::~TAA_Effect()
{
}

void TAA_Effect::Start(GameEngineRenderTarget* _Target)
{
	TAAUnit = std::make_shared<GameEngineRenderUnit>();
	TAAUnit->SetMesh("FullRect");
	TAAUnit->SetMaterial("TAA");

	TAAUnit->ShaderResHelper.SetConstantBufferLink("RenderBaseValue", BaseValue);

	ResultTarget = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
}

void TAA_Effect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	ResultTarget->Clear();
	TAAUnit->ShaderResHelper.SetTexture("DiffuseTex", _Target->GetTexture(0));
	ResultTarget->Setting();
	TAAUnit->Render(_DeltaTime);
	TAAUnit->ShaderResHelper.AllResourcesReset();

	_Target->Clear();
	_Target->Merge(ResultTarget);
}
