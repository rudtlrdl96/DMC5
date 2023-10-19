#pragma once
#include "GameEngineRenderTarget.h"

struct BlurData
{
	float4 ScreenSize;
	float4 ScreenRatio;
};

// Ό³Έν :
class BloomEffect : public GameEnginePostProcess
{
public:
	// constrcuter destructer
	BloomEffect();
	~BloomEffect();

	std::shared_ptr<GameEngineRenderTarget> DebugTargetA = nullptr;
	std::shared_ptr<GameEngineRenderTarget> DebugTargetB = nullptr;

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;

private:
	std::shared_ptr<GameEngineRenderUnit> BlurUnit;
	std::shared_ptr<GameEngineRenderUnit> BloomBlurUnit;
	BlurData Data;


	void LevelChangeStart() override
	{
		ResultTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
		DebugTargetA->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
		DebugTargetB->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	}

	void LevelChangeEnd() override
	{
		ResultTarget->ReleaseTextures();
	}
};

