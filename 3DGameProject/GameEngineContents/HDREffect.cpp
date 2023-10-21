#include "PrecompileHeader.h"
#include "HDREffect.h"

HDREffect::HDREffect()
{
}

HDREffect::~HDREffect()
{
}


void HDREffect::Start(GameEngineRenderTarget* _Target)
{
	HDRColorUnit = std::make_shared<GameEngineRenderUnit>();
	HDRColorUnit->SetMesh("FullRect");
	HDRColorUnit->SetMaterial("HDREffect");

	ResultTarget = GameEngineRenderTarget::Create();
}

void HDREffect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	ResultTarget->Clear();
	ResultTarget->Setting();
	HDRColorUnit->ShaderResHelper.SetTexture("DiffuseTex", _Target->GetTexture(0));
	HDRColorUnit->Render(_DeltaTime);
	HDRColorUnit->ShaderResHelper.AllResourcesReset();

	_Target->Clear();
	_Target->Merge(ResultTarget);
}