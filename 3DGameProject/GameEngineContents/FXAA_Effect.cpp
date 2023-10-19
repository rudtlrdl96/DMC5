#include "PrecompileHeader.h"
#include "FXAA_Effect.h"

FXAA_Effect::FXAA_Effect()
{
}

FXAA_Effect::~FXAA_Effect()
{
}

void FXAA_Effect::Start(GameEngineRenderTarget* _Target)
{
	TAAUnit = std::make_shared<GameEngineRenderUnit>();
	TAAUnit->SetMesh("FullRect");
	TAAUnit->SetMaterial("FXAA");

	TAAUnit->ShaderResHelper.SetConstantBufferLink("RenderBaseValue", BaseValue);

	BaseValue.ScreenScale = GameEngineWindow::GetScreenSize();

	ResultTarget = GameEngineRenderTarget::Create();
}

void FXAA_Effect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	if (GameEngineOptionValue::None == GameEngineOption::GetOption("FXAA"))
	{
		return;
	}
	
	ResultTarget->Clear();
	TAAUnit->ShaderResHelper.SetTexture("DiffuseTex", _Target->GetTexture(0));
	ResultTarget->Setting();
	TAAUnit->Render(_DeltaTime);
	TAAUnit->ShaderResHelper.AllResourcesReset();
	
	_Target->Clear();
	_Target->Merge(ResultTarget);
}
