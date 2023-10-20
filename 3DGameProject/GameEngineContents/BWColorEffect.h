#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

// 설명 :
class BWColorEffect : public GameEnginePostProcess
{
	static BWColorEffect* CurBWColorEffect;

public:
	static BWColorEffect* GetBWColorEffect()
	{
		if (nullptr == CurBWColorEffect)
		{
			MsgAssert("BWColorEffect PostEffect를 생성하지 않고 받아오려 했습니다.");
		}

		return CurBWColorEffect;
	}

public:
	// constrcuter destructer
	BWColorEffect();
	~BWColorEffect();

	// delete Function
	BWColorEffect(const BWColorEffect& _Other) = delete;
	BWColorEffect(BWColorEffect&& _Other) noexcept = delete;
	BWColorEffect& operator=(const BWColorEffect& _Other) = delete;
	BWColorEffect& operator=(BWColorEffect&& _Other) noexcept = delete;

	inline void BWColorEffectOn()
	{
		IsEffectOn = true;
	}

	inline void BWColorEffectOff()
	{
		IsEffectOn = false;
	}

	inline void BWColorEffectSwitch()
	{
		IsEffectOn = !IsEffectOn;
	}

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;
private:
	std::shared_ptr<GameEngineRenderUnit> ColorUnit = nullptr;

	bool IsEffectOn = false;

	void LevelChangeStart() override
	{
		CurBWColorEffect = this;
		ResultTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	}

	void LevelChangeEnd() override
	{
		CurBWColorEffect = nullptr;
		ResultTarget->ReleaseTextures();
	}
};

