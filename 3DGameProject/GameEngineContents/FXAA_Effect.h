#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

class FXAA_Effect : public GameEnginePostProcess
{
public:
	FXAA_Effect();
	~FXAA_Effect();

	FXAA_Effect(const FXAA_Effect& _Other) = delete;
	FXAA_Effect(FXAA_Effect&& _Other) noexcept = delete;
	FXAA_Effect& operator=(const FXAA_Effect& _Other) = delete;
	FXAA_Effect& operator=(FXAA_Effect&& _Other) noexcept = delete;

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;

private:
	std::shared_ptr<GameEngineRenderUnit> TAAUnit = nullptr;
	RenderBaseValue BaseValue = RenderBaseValue();

	void LevelChangeStart() override
	{
		ResultTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	}

	void LevelChangeEnd() override
	{
		ResultTarget->ReleaseTextures();
	}
};