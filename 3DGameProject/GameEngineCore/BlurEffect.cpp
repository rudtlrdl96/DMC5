#include "PrecompileHeader.h"
#include "BlurEffect.h"
#include "GameEngineOption.h"

BlurEffect::BlurEffect()
{
}

BlurEffect::~BlurEffect()
{
}

void BlurEffect::Start(GameEngineRenderTarget* _Target)
{
	BlurUnit = std::make_shared<GameEngineRenderUnit>();
	BlurUnit->SetMesh("FullRect");
	BlurUnit->SetMaterial("Blur");

	ResultTarget = GameEngineRenderTarget::Create();
}

void BlurEffect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	if (GameEngineOption::GetOption("PostEffect") == GameEngineOptionValue::None)
	{
		return;
	}

	ResultTarget->Clear();

	BlurUnit->ShaderResHelper.SetTexture("DiffuseTex", _Target->GetTexture(0));
	ResultTarget->Setting();
	BlurUnit->Render(_DeltaTime);
	BlurUnit->ShaderResHelper.AllResourcesReset();

	_Target->Clear();
	_Target->Merge(ResultTarget);

}
