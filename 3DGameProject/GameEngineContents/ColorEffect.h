#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

// ���� :
class ColorEffect : public GameEnginePostProcess
{
	class ColorBuffer
	{
	public:
		float4 ColorStart = float4::ZERO;
		float4 ColorEnd = float4::ZERO;
		float4 ColorTime = float4::ZERO;
	};

	static ColorEffect* CurColorEffect;

public:
	static ColorEffect* GetColorEffect()
	{
		if (nullptr == CurColorEffect)
		{
			MsgAssert("ColorEffect PostEffect�� �������� �ʰ� �޾ƿ��� �߽��ϴ�.");
		}

		return CurColorEffect;
	}

public:
	// constrcuter destructer
	ColorEffect();
	~ColorEffect();

	// delete Function
	ColorEffect(const ColorEffect& _Other) = delete;
	ColorEffect(ColorEffect&& _Other) noexcept = delete;
	ColorEffect& operator=(const ColorEffect& _Other) = delete;
	ColorEffect& operator=(ColorEffect&& _Other) noexcept = delete;

	// ���� �÷��� �����մϴ� w ���� ���õ˴ϴ�.
	inline void SetStartColor(const float4& _Color)
	{
		ShaderBuffer.ColorStart = _Color;
	}

	// ���� �÷��� �����մϴ� w ���� ���õ˴ϴ�.
	inline void SetEndColor(const float4& _Color)
	{
		ShaderBuffer.ColorEnd = _Color;
	}

	// ���� �ӵ��� �����մϴ� w ���� ���õ˴ϴ�.
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

	void LevelChangeStart() override
	{
		CurColorEffect = this;
		ResultTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	}

	void LevelChangeEnd() override
	{
		CurColorEffect = nullptr;
		ResultTarget->ReleaseTextures();
	}
};

