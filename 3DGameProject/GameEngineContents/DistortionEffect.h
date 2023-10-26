#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

// Ό³Έν :
class DistortionEffect : public GameEnginePostProcess
{
	class DistortionOption
	{
	public:
		float4 ScreenSize = float4::ZERO;
		float4 DistortionValue = float4(1, 1);
		float4 PixelSizeValue = float4(10, 10);
	};

public:
	// constrcuter destructer
	DistortionEffect();
	~DistortionEffect();

	// delete Function
	DistortionEffect(const DistortionEffect& _Other) = delete;
	DistortionEffect(DistortionEffect&& _Other) noexcept = delete;
	DistortionEffect& operator=(const DistortionEffect& _Other) = delete;
	DistortionEffect& operator=(DistortionEffect&& _Other) noexcept = delete;
	
	inline void SetMaskTexture(std::shared_ptr<GameEngineRenderTarget> _RenderTarget, size_t _TextureIndex)
	{
		MaskTarget = _RenderTarget.get();
		MaskTexIndex = _TextureIndex;
	}

	inline void SetDistortionValue(float _ValueX, float _ValueY)
	{
		Option.DistortionValue.x = _ValueX;
		Option.DistortionValue.y = _ValueY;
	}

	inline void SetMaxPixelValue(float _ValueX, float _ValueY)
	{
		Option.PixelSizeValue.x = _ValueX;
		Option.PixelSizeValue.y = _ValueY;
	}

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;

private:
	std::shared_ptr<GameEngineRenderUnit> DistortionUnit = nullptr;

	GameEngineRenderTarget* MaskTarget = nullptr;
	size_t MaskTexIndex = 0;

	DistortionOption Option;

	void LevelChangeStart() override
	{
		ResultTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	}

	void LevelChangeEnd() override
	{
		ResultTarget->ReleaseTextures();
	}
};

