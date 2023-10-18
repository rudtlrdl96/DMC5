#include "PrecompileHeader.h"
#include "JudgementCut.h"

JudgementCut* JudgementCut::CurLevelJudgementCut = nullptr;

JudgementCut::JudgementCut() 
{
}

JudgementCut::~JudgementCut() 
{
}

void JudgementCut::AddLine(float4 _Line)
{
	if (32 == ShaderBuffer.LineCount)
	{
		MsgAssert("������Ʈ �� ���̴��� �ִ� ���� ������ �ʰ��߽��ϴ�.");
		return;
	}

	ShaderBuffer.Lines[ShaderBuffer.LineCount] = _Line;
	++ShaderBuffer.LineCount;
}

void JudgementCut::JudgementCutOn()
{
	IsEffectOn = true;

	BaseValue.DeltaTime = 0.0f;
	BaseValue.SumDeltaTime = 0.0f;
}

void JudgementCut::JudgementCutOff()
{
	IsEffectOn = false;
}

void JudgementCut::Start(GameEngineRenderTarget* _Target)
{
	JudgementUnit = std::make_shared<GameEngineRenderUnit>();
	JudgementUnit->SetMesh("FullRect");
	JudgementUnit->SetMaterial("JudgementCut");

	JudgementUnit->ShaderResHelper.SetConstantBufferLink("RenderBaseValue", BaseValue);
	JudgementUnit->ShaderResHelper.SetConstantBufferLink("JudgementCutValue", ShaderBuffer);

	ResultTarget = GameEngineRenderTarget::Create();

	AddLine(float4(0.0f, 0.55f, 1.0f, 0.56f));
	AddLine(float4(0.0f, 0.3f , 1.0f, 0.9f));
	AddLine(float4(0.0f, 0.8f , 1.0f, 0.6f));
	AddLine(float4(0.0f, 0.15f, 1.0f, 0.35f));
	AddLine(float4(0.5f, 0.33f, 0.2f, 0.42f));
	AddLine(float4(0.56f, 0.0f, 0.86f, 1.0f));
	AddLine(float4(0.0f, 0.34f, 1.0f, 0.58f));

	if (false == GameEngineInput::IsKey("Debug_JudgementCutSwitch"))
	{
		GameEngineInput::CreateKey("Debug_JudgementCutSwitch", VK_F4);
	}
}

void JudgementCut::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	BaseValue.DeltaTime = _DeltaTime * Speed;

	if (false == IsEffectOn)
	{
		if (0.0f >= BaseValue.SumDeltaTime)
		{
			return;
		}

		BaseValue.SumDeltaTime -= _DeltaTime * Speed;
	}
	else
	{
		BaseValue.SumDeltaTime += _DeltaTime * Speed;
	}

	if (0.0f >= BaseValue.SumDeltaTime)
	{
		BaseValue.SumDeltaTime = 0.0f;
	}

	if (MaxCut < BaseValue.SumDeltaTime)
	{
		BaseValue.SumDeltaTime = MaxCut;
	}

	ResultTarget->Clear();
	JudgementUnit->ShaderResHelper.SetTexture("DiffuseTex", _Target->GetTexture(0));
	ResultTarget->Setting();
	JudgementUnit->Render(_DeltaTime);
	JudgementUnit->ShaderResHelper.AllResourcesReset();

	_Target->Clear();
	_Target->Merge(ResultTarget);
}