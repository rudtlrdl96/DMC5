#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

// ���� :
class ZoomEffect : public GameEnginePostProcess
{
public:
	static ZoomEffect* GetZoomEffect()
	{
		return CurLevelZoomEffect;
	}

private:
	static ZoomEffect* CurLevelZoomEffect;

	class ZoomEffectBuffer
	{
	public:
		float4 Ratio = {1.0f, 1.0f, 1.0f, 1.0f};
	};

public:
	// constrcuter destructer
	ZoomEffect();
	~ZoomEffect();

	// delete Function
	ZoomEffect(const ZoomEffect& _Other) = delete;
	ZoomEffect(ZoomEffect&& _Other) noexcept = delete;
	ZoomEffect& operator=(const ZoomEffect& _Other) = delete;
	ZoomEffect& operator=(ZoomEffect&& _Other) noexcept = delete;

	// Zoom �ӵ��� �����մϴ� �⺻ �� : 3.0f
	inline void SetSpeed(float _Ratio)
	{
		Speed = 3.0f;
	}
	
	inline void EffectOn(float _Ratio)
	{
		Ratio = _Ratio;
		IsEffectOn = true;
		ProgressTime = 0.0f;
	}

	inline void EffectOff()
	{
		IsEffectOn = false;
		ProgressTime = 0.0f;
	}

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;

private:
	std::shared_ptr<GameEngineRenderUnit> ZoomUnit = nullptr;

	ZoomEffectBuffer ShaderBuffer = ZoomEffectBuffer();

	bool IsEffectOn = false;
	float ProgressTime = 1.0f;
	float Ratio = 1.0f;
	float Speed = 3.0f;

	void LevelChangeStart() override
	{
		CurLevelZoomEffect = this;
		ResultTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	}

	void LevelChangeEnd() override
	{
		CurLevelZoomEffect = nullptr;
		ResultTarget->ReleaseTextures();
	}
};