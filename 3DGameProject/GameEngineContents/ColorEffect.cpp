#include "PrecompileHeader.h"
#include "ColorEffect.h"

ColorEffect* ColorEffect::CurColorEffect = nullptr;

ColorEffect::ColorEffect() 
{
}

ColorEffect::~ColorEffect() 
{
}

bool ColorEffect::IsEffectEnd()
{
	return (0.0f >= ShaderBuffer.ColorTime.x && 0.0f >= ShaderBuffer.ColorTime.y && 0.0f >= ShaderBuffer.ColorTime.z);
}

void ColorEffect::Start(GameEngineRenderTarget* _Target)
{
	ColorUnit = std::make_shared<GameEngineRenderUnit>();
	ColorUnit->SetMesh("FullRect");
	ColorUnit->SetMaterial("ColorEffect");

	ColorUnit->ShaderResHelper.SetConstantBufferLink("ColorEffectBuffer", ShaderBuffer);

	ResultTarget = GameEngineRenderTarget::Create();

	SetStartColor(float4(0.0f, 0.0f, 0.0f));
	SetEndColor(float4(0.0f, 0.0f, 0.1f));
}

void ColorEffect::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	if (true == IsEffectOn)
	{
		ShaderBuffer.ColorTime.x += _DeltaTime * Speed.x;
		ShaderBuffer.ColorTime.y += _DeltaTime * Speed.y;
		ShaderBuffer.ColorTime.z += _DeltaTime * Speed.z;

		if (1.0f <= ShaderBuffer.ColorTime.x)
		{
			ShaderBuffer.ColorTime.x = 1.0f;
		}

		if (1.0f <= ShaderBuffer.ColorTime.x)
		{
			ShaderBuffer.ColorTime.y = 1.0f;
		}

		if (1.0f <= ShaderBuffer.ColorTime.x)
		{
			ShaderBuffer.ColorTime.z = 1.0f;
		}
	}
	else
	{
		if (true == IsEffectEnd())
		{
			return;
		}

		ShaderBuffer.ColorTime.x -= _DeltaTime * Speed.x;
		ShaderBuffer.ColorTime.y -= _DeltaTime * Speed.y;
		ShaderBuffer.ColorTime.z -= _DeltaTime * Speed.z;

		if (0.0f > ShaderBuffer.ColorTime.x)
		{
			ShaderBuffer.ColorTime.x = 0.0f;
		}

		if (0.0f > ShaderBuffer.ColorTime.x)
		{
			ShaderBuffer.ColorTime.y = 0.0f;
		}

		if (0.0f > ShaderBuffer.ColorTime.x)
		{
			ShaderBuffer.ColorTime.z = 0.0f;
		}
	}

	ResultTarget->Clear();
	ColorUnit->ShaderResHelper.SetTexture("DiffuseTex", _Target->GetTexture(0));
	ResultTarget->Setting();
	ColorUnit->Render(_DeltaTime);
	ColorUnit->ShaderResHelper.AllResourcesReset();

	_Target->Clear();
	_Target->Merge(ResultTarget);
}