#pragma once
#include "GameEngineRenderTarget.h"


class GameEngineCaptureTexture
{
public:
	static std::shared_ptr<GameEngineRenderTarget> CaptureTexture(const std::string_view& _CaptureName, const float4& _Scale, std::shared_ptr<GameEngineRenderTarget> _CaptureTarget, size_t _Index = 0);
	static std::shared_ptr<GameEngineRenderTarget> FindTexture(const std::string_view& _CaptureName);
	static void ReleaseTexture(const std::string_view& _CaptureName);
	static void ClearAllTexture();

private:
	static std::map<std::string, std::shared_ptr<GameEngineRenderTarget>> CaptureTarget;

	GameEngineCaptureTexture();
	~GameEngineCaptureTexture();

	GameEngineCaptureTexture(const GameEngineCaptureTexture& _Other) = delete;
	GameEngineCaptureTexture(GameEngineCaptureTexture&& _Other) noexcept = delete;
	GameEngineCaptureTexture& operator=(const GameEngineCaptureTexture& _Other) = delete;
	GameEngineCaptureTexture& operator=(GameEngineCaptureTexture&& _Other) noexcept = delete;

};

