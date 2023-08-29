#include "PrecompileHeader.h"
#include "ZoomEffect.h"

ZoomEffect::ZoomEffect() 
{
}

ZoomEffect::~ZoomEffect() 
{
}

void ZoomEffect::Start(GameEngineRenderTarget* _Target)
{
	ZoomUnit = std::make_shared<GameEngineRenderUnit>();
	ZoomUnit->SetMesh("FullRect");
	ZoomUnit->SetMaterial("ZoomEffect");

	ZoomUnit->ShaderResHelper.SetConstantBufferLink("ZoomEffectBuffer", ShaderBuffer);

	ResultTarget = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);

	if (false == GameEngineInput::IsKey("Debug_JudgementCutSwitch"))
	{
		GameEngineInput::CreateKey("Debug_JudgementCutSwitch", VK_F3);
	}

	SetSpeed(10.0f);
}

void ZoomEffect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("Debug_JudgementCutSwitch"))
	{
		if (true == IsEffectOn)
		{
			EffectOff();
		}
		else
		{
			EffectOn(1.2f);
		}
	}

	if (false == IsEffectOn && 1.0f <= ProgressTime)
	{
		return;
	}

	ProgressTime += _DeltaTime * Speed;

	if (true == IsEffectOn)
	{
		ShaderBuffer.Ratio = float4::LerpClamp(float4::ONE, float4(Ratio, Ratio, Ratio), ProgressTime);
	}
	else
	{
		ShaderBuffer.Ratio = float4::LerpClamp(float4(Ratio, Ratio, Ratio), float4::ONE, ProgressTime);
	}

	ResultTarget->Clear();
	ZoomUnit->ShaderResHelper.SetTexture("DiffuseTex", _Target->GetTexture(0));
	ResultTarget->Setting();
	ZoomUnit->Render(_DeltaTime);
	ZoomUnit->ShaderResHelper.AllResourcesReset();

	_Target->Clear();
	_Target->Merge(ResultTarget);
}