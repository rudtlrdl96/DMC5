#include "PrecompileHeader.h"
#include "MotionBlurEffect.h"

bool MotionBlurEffect::IsEffectOff = false;

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

	BaseValue.ScreenScale = GameEngineWindow::GetScreenSize();

	//MotionBlurUnit->ShaderResHelper.SetConstantBufferLink("RenderBaseValue", BaseValue);
	MotionBlurUnit->ShaderResHelper.SetConstantBufferLink("MotionBlurData", Data);
}

void MotionBlurEffect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	if (false == IsEffectOff)
	{
		return;
	}

	if (nullptr == CamPosTarget)
	{
		MsgAssert("카메라 위치 타겟이 설정되지 않았습니다.");
		return;
	}	
	
	if (nullptr == CamMaskTarget)
	{
		MsgAssert("카메라 마스크 타겟이 설정되지 않았습니다.");
		return;
	}

	if (nullptr == Cam)
	{
		MsgAssert("카메라가 설정되지 않았습니다.");
		return;
	}

	if (false == FirstCheck)
	{
		CurFramePos->Clear();
		CurFramePos->Merge(CamPosTarget);

		PrevFramePos->Clear();
		PrevFramePos->Merge(CurFramePos);

		Data.PrevFrameViewProjection = Cam->GetView() * Cam->GetProjection();

		FirstCheck = true;
		return;
	}

	CurFramePos ->Clear();
	CurFramePos ->Merge(CamPosTarget);

	// MotionBlur
	ResultTarget->Clear();
	ResultTarget->Setting();

	MotionBlurUnit->ShaderResHelper.SetTexture("DiffuseTex", _Target->GetTexture(0));
	MotionBlurUnit->ShaderResHelper.SetTexture("MaskTex", CamMaskTarget->GetTexture(0));
	MotionBlurUnit->ShaderResHelper.SetTexture("PrevPosTex", PrevFramePos->GetTexture(0));
	MotionBlurUnit->ShaderResHelper.SetTexture("CurPosTex", CurFramePos->GetTexture(0));
			
	MotionBlurUnit->Render(_DeltaTime);
	MotionBlurUnit->ShaderResHelper.AllResourcesReset();

	_Target->Clear();
	_Target->Merge(ResultTarget);

	PrevFramePos ->Clear();
	PrevFramePos ->Merge(CurFramePos);

	Data.PrevFrameViewProjection = Cam->GetView() * Cam->GetProjection();

	int a = 0;
}