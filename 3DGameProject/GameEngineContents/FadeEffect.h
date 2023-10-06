#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

enum class FadeState
{
	None,
	FadeIn,
	FadeOut,
};

// Ό³Έν :
class FadeEffect : public GameEnginePostProcess
{
public:
	// constrcuter destructer
	FadeEffect();
	~FadeEffect();

	// delete Function
	FadeEffect(const FadeEffect& _Other) = delete;
	FadeEffect(FadeEffect&& _Other) noexcept = delete;
	FadeEffect& operator=(const FadeEffect& _Other) = delete;
	FadeEffect& operator=(FadeEffect&& _Other) noexcept = delete;

	void FadeIn() 
	{
		State = FadeState::FadeIn;
		FadeData.x = 1.0f;
	}

	void FadeOut() 
	{
		State = FadeState::FadeOut;
		FadeData.x = 0.0f;
	}

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;

private:
	float4 FadeData = {1.0f, 1.0f, 1.0f, 1.0f};

	FadeState State = FadeState::None;

	std::shared_ptr<GameEngineRenderUnit> FadeUnit = nullptr;

	void LevelChangeStart() override
	{
		ResultTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	}

	void LevelChangeEnd() override
	{
		ResultTarget->ReleaseTextures();
	}
};

