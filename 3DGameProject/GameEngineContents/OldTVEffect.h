#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

// Ό³Έν :
class OldTVEffect : public GameEnginePostProcess
{
public:
	// constrcuter destructer
	OldTVEffect();
	~OldTVEffect();

	// delete Function
	OldTVEffect(const OldTVEffect& _Other) = delete;
	OldTVEffect(OldTVEffect&& _Other) noexcept = delete;
	OldTVEffect& operator=(const OldTVEffect& _Other) = delete;
	OldTVEffect& operator=(OldTVEffect&& _Other) noexcept = delete;

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;

private:
	RenderBaseValue BaseValue;
	std::shared_ptr<GameEngineRenderUnit> BlurUnit = nullptr;
};

