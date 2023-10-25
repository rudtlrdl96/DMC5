#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

class MotionBlurEffect : public GameEnginePostProcess
{
public:
	MotionBlurEffect();
	~MotionBlurEffect();

	MotionBlurEffect(const MotionBlurEffect& _Other) = delete;
	MotionBlurEffect(MotionBlurEffect&& _Other) noexcept = delete;
	MotionBlurEffect& operator=(const MotionBlurEffect& _Other) = delete;
	MotionBlurEffect& operator=(MotionBlurEffect&& _Other) noexcept = delete;

	inline void SetCamPosTarget(std::shared_ptr<GameEngineRenderTarget> _CamPosTarget)
	{
		CamPosTarget = _CamPosTarget;
	}

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;
	
private:
	std::shared_ptr<GameEngineRenderTarget> PrevFramePos = nullptr;
	std::shared_ptr<GameEngineRenderTarget> CurFramePos = nullptr;

	std::shared_ptr<GameEngineRenderTarget> CamPosTarget = nullptr;

	std::shared_ptr<GameEngineRenderUnit> MotionBlurUnit;

	RenderBaseValue BaseValue;

	void LevelChangeStart() override
	{
		ResultTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
		PrevFramePos->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
		CurFramePos->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	}

	void LevelChangeEnd() override
	{
		ResultTarget->ReleaseTextures();
		PrevFramePos->ReleaseTextures();
		CurFramePos->ReleaseTextures();
	}

};

