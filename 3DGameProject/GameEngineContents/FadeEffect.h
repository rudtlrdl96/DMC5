#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

enum class FadeState
{
	None,
	FadeIn,
	FadeOut,
	Wait,
};

// 설명 :
class FadeEffect : public GameEnginePostProcess
{
public:
	static FadeEffect* GetFadeEffect()
	{
		if (nullptr == CurLevelFade)
		{
			MsgAssert("FadeEffect를 생성하지 않고 받아오려 했습니다.");
		}

		return CurLevelFade;
	}


	// constrcuter destructer
	FadeEffect();
	~FadeEffect();

	// delete Function
	FadeEffect(const FadeEffect& _Other) = delete;
	FadeEffect(FadeEffect&& _Other) noexcept = delete;
	FadeEffect& operator=(const FadeEffect& _Other) = delete;
	FadeEffect& operator=(FadeEffect&& _Other) noexcept = delete;

	void FadeIn(float _Time = 1.0f)
	{
		State = FadeState::FadeIn;
		FadeData.x = 0.0f;
		Speed = 1.0f / _Time;
	}

	void FadeOut(float _Time = 1.0f)
	{
		State = FadeState::FadeOut;
		FadeData.x = 1.0f;
		Speed = 1.0f / _Time;
	}
	
	// 페이드 사용이 있을때까지 화면을 검은색으로 유지합니다
	void WaitCutScene()
	{
		State = FadeState::Wait;
		FadeData.x = 0.0f;
	}

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;

private:
	static FadeEffect* CurLevelFade;
	float4 FadeData = {1.0f, 1.0f, 1.0f, 1.0f};
	float Speed = 0.0f;

	FadeState State = FadeState::None;

	std::shared_ptr<GameEngineRenderUnit> FadeUnit = nullptr;

	void LevelChangeStart() override
	{
		CurLevelFade = this;
		ResultTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	}

	void LevelChangeEnd() override
	{
		CurLevelFade = nullptr;
		ResultTarget->ReleaseTextures();
	}
};

