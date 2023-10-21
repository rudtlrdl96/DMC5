#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

// Ό³Έν :
class HDREffect : public GameEnginePostProcess
{
public:
	// constrcuter destructer
	HDREffect();
	~HDREffect();

	// delete Function
	HDREffect(const HDREffect& _Other) = delete;
	HDREffect(HDREffect&& _Other) noexcept = delete;
	HDREffect& operator=(const HDREffect& _Other) = delete;
	HDREffect& operator=(HDREffect&& _Other) noexcept = delete;

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;
private:
	std::shared_ptr<GameEngineRenderUnit> HDRColorUnit = nullptr;

	void LevelChangeStart() override
	{
		ResultTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	}

	void LevelChangeEnd() override
	{
		ResultTarget->ReleaseTextures();
	}
};

