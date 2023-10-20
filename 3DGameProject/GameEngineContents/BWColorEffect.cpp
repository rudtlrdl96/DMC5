#include "PrecompileHeader.h"
#include "BWColorEffect.h"

BWColorEffect* BWColorEffect::CurBWColorEffect = nullptr;

BWColorEffect::BWColorEffect()
{
}

BWColorEffect::~BWColorEffect()
{
}


void BWColorEffect::Start(GameEngineRenderTarget* _Target)
{
	ColorUnit = std::make_shared<GameEngineRenderUnit>();
	ColorUnit->SetMesh("FullRect");
	ColorUnit->SetMaterial("BWColorEffect");

	ResultTarget = GameEngineRenderTarget::Create();
}

void BWColorEffect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	if (false == IsEffectOn)
	{
		return;
	}

	ResultTarget->Clear();
	ResultTarget->Setting();
	ColorUnit->ShaderResHelper.SetTexture("DiffuseTex", _Target->GetTexture(0));
	ColorUnit->Render(_DeltaTime);
	ColorUnit->ShaderResHelper.AllResourcesReset();

	_Target->Clear();
	_Target->Merge(ResultTarget);
}