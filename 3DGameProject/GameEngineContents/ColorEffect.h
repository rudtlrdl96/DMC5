#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

// 설명 :
class ColorEffect : public GameEnginePostProcess
{
	class ColorBuffer
	{
	public:
		float4 ColorStart = float4::ZERO;
		float4 ColorEnd = float4::ZERO;
		float4 ColorTime = float4::ZERO;
	};
public:
	// constrcuter destructer
	ColorEffect();
	~ColorEffect();

	// delete Function
	ColorEffect(const ColorEffect& _Other) = delete;
	ColorEffect(ColorEffect&& _Other) noexcept = delete;
	ColorEffect& operator=(const ColorEffect& _Other) = delete;
	ColorEffect& operator=(ColorEffect&& _Other) noexcept = delete;

	// 시작 컬러를 설정합니다 w 값은 무시됩니다.
	inline void SetStartColor(const float4& _Color)
	{
		ShaderBuffer.ColorStart = _Color;
	}

	// 엔드 컬러를 설정합니다 w 값은 무시됩니다.
	inline void SetEndColor(const float4& _Color)
	{
		ShaderBuffer.ColorEnd = _Color;
	}

	// 보간 속도를 설정합니다 w 값은 무시됩니다.
	inline void SetSpeed(const float4& _Speed)
	{
		Speed = _Speed;
	}

	inline void ColorEffectOn()
	{
		IsEffectOn = true;
		ShaderBuffer.ColorTime = float4(0.0f, 0.0f, 0.0f);
	}

	inline void ColorEffectOff()
	{
		IsEffectOn = false;
		ShaderBuffer.ColorTime = float4(0.0f, 0.0f, 0.0f);
	}

	bool IsEffectEnd();

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;
private:
	std::shared_ptr<GameEngineRenderUnit> ColorUnit = nullptr;

	ColorBuffer ShaderBuffer = ColorBuffer();

	bool IsEffectOn = false;

	float4 Speed = float4(1.0f, 1.0f, 1.0f, 1.0f);
};

