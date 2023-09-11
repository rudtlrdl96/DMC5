#pragma once
#include "GameEngineRenderTarget.h"

// Ό³Έν :
class BloomEffect : public GameEnginePostProcess
{
public:
	// constrcuter destructer
	BloomEffect();
	~BloomEffect();

	std::shared_ptr<GameEngineRenderTarget> BlurTarget0;
	std::shared_ptr<GameEngineRenderTarget> BlurTarget1;

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;

private:
	std::shared_ptr<GameEngineRenderUnit> BlurUnit;
	std::shared_ptr<GameEngineRenderUnit> BloomBlurUnit;



};

