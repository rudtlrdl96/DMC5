#pragma once
#include "GameEngineRenderTarget.h"

// Ό³Έν :
class BlurEffect : public GameEnginePostProcess
{
public:
	// constrcuter destructer
	BlurEffect();
	~BlurEffect();

	// delete Function
	BlurEffect(const BlurEffect& _Other) = delete;
	BlurEffect(BlurEffect&& _Other) noexcept = delete;
	BlurEffect& operator=(const BlurEffect& _Other) = delete;
	BlurEffect& operator=(BlurEffect&& _Other) noexcept = delete;

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;

private:
	std::shared_ptr<GameEngineRenderUnit> BlurUnit;

	void LevelChangeStart() override
	{
		ResultTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	}

	void LevelChangeEnd() override
	{
		ResultTarget->ReleaseTextures();
	}
};

