#pragma once
#include "GameEngineRenderTarget.h"

struct BlurData
{
	float4 ScreenSize;
	float4 ScreenRatio;
};

// ���� :
class BloomEffect : public GameEnginePostProcess
{
public:
	// constrcuter destructer
	BloomEffect();
	~BloomEffect();

	std::shared_ptr<GameEngineRenderTarget> BlurTarget;

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;

private:
	std::shared_ptr<GameEngineRenderUnit> BlurUnit;
	std::shared_ptr<GameEngineRenderUnit> BloomBlurUnit;
	BlurData Data;

};

