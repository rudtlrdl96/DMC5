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
	PrevFramePos = GameEngineRenderTarget::Create();
	CurFramePos = GameEngineRenderTarget::Create();

	MotionBlurUnit = std::make_shared<GameEngineRenderUnit>();
	MotionBlurUnit->SetMesh("FullRect");
	MotionBlurUnit->SetMaterial("MotionBlur");

	MotionBlurUnit->ShaderResHelper.SetConstantBufferLink("RenderBaseValue", BaseValue);

	BaseValue.ScreenScale = GameEngineWindow::GetScreenSize();
}

void MotionBlurEffect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	if (nullptr == CamPosTarget)
	{
		MsgAssert("카메라 위치 타겟이 설정되지 않았습니다.");
		return;
	}

	CurFramePos ->Clear();
	CurFramePos ->Merge(CamPosTarget);

	// MotionBlur
	ResultTarget->Clear();
	ResultTarget->Setting();

	MotionBlurUnit->ShaderResHelper.SetTexture("DiffuseTex", _Target->GetTexture(0));
	MotionBlurUnit->ShaderResHelper.SetTexture("PrevPosTex", PrevFramePos->GetTexture(0));
	MotionBlurUnit->ShaderResHelper.SetTexture("CurPosTex", CurFramePos->GetTexture(0));
			
	MotionBlurUnit->Render(_DeltaTime);
	MotionBlurUnit->ShaderResHelper.AllResourcesReset();

	_Target->Clear();
	_Target->Merge(ResultTarget);

	PrevFramePos ->Clear();
	PrevFramePos ->Merge(CurFramePos);
}