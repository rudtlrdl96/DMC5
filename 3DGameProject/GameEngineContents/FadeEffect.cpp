#include "PrecompileHeader.h"
#include "FadeEffect.h"

FadeEffect::FadeEffect()
{
}

FadeEffect::~FadeEffect()
{
}

FadeEffect* FadeEffect::CurLevelFade = nullptr;

void FadeEffect::Start(GameEngineRenderTarget* _Target)
{
	FadeUnit = std::make_shared<GameEngineRenderUnit>();
	FadeUnit->SetMesh("FullRect");
	FadeUnit->SetMaterial("Fade");

	FadeUnit->ShaderResHelper.SetConstantBufferLink("FADEDATA", FadeData);
	// 
	ResultTarget = GameEngineRenderTarget::Create();
}

void FadeIn()
{

}

void FadeOut()
{

}

void FadeEffect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	if (State == FadeState::None)
	{
		return;
	}

	if (State == FadeState::FadeIn && 1.0f <= FadeData.x)
	{
		return;
	}

	if (State == FadeState::FadeIn)
	{
		FadeData.x += Speed * _DeltaTime;
	}
	else if (State == FadeState::FadeOut) {
		FadeData.x -= Speed * _DeltaTime;
	}


	ResultTarget->Clear();
	FadeUnit->ShaderResHelper.SetTexture("DiffuseTex", _Target->GetTexture(0));
	ResultTarget->Setting();
	FadeUnit->Render(_DeltaTime);
	FadeUnit->ShaderResHelper.AllResourcesReset();

	_Target->Clear();
	_Target->Merge(ResultTarget);

	int a = 0;
}