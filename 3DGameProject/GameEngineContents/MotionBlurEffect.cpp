#include "PrecompileHeader.h"
#include "MotionBlurEffect.h"

MotionBlurEffect::MotionBlurEffect()
{
}

MotionBlurEffect::~MotionBlurEffect()
{
}

void MotionBlurEffect::Start(GameEngineRenderTarget* _Target)
{
	ResultTarget = GameEngineRenderTarget::Create();
}

void MotionBlurEffect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{

}