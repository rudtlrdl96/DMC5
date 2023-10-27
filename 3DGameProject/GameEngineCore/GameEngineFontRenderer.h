#pragma once
#include <string_view>
#include "GameEngineRenderer.h"
#include "GameEngineFont.h"
#include "EngineContentRenderingStruct.h"

// Ό³Έν :
class GameEngineFontRenderer : public GameEngineRenderer
{
public:
	// constrcuter destructer
	GameEngineFontRenderer();
	~GameEngineFontRenderer();

	// delete Function
	GameEngineFontRenderer(const GameEngineFontRenderer& _Other) = delete;
	GameEngineFontRenderer(GameEngineFontRenderer&& _Other) noexcept = delete;
	GameEngineFontRenderer& operator=(const GameEngineFontRenderer& _Other) = delete;
	GameEngineFontRenderer& operator=(GameEngineFontRenderer&& _Other) noexcept = delete;

	void SetFont(const std::string_view& _Font);
	void SetText(const std::string_view& _Text);

	void SetScale(float _FontScale)
	{
		FontScale = _FontScale;
	}

	void SetColor(float4 _FontColor)
	{
		FontColor = _FontColor;
	}

	void SetAlpha(float _FontAlpha)
	{
		FontColor.a = _FontAlpha;
	}

	inline void SetFontFlag(FW1_TEXT_FLAG _Flag)
	{
		FwTextFlag = _Flag;
	}

protected:
	void Render(float _Delta) override;

private:
	void Start() override;

	std::shared_ptr<GameEngineRenderUnit> Unit;

	float FontScale = 10.0f;
	float4 FontColor = float4::RED;
	std::string Text;
	std::shared_ptr<GameEngineFont> Font;
	float4 AtlasData;
	ColorOption ColorOptionValue;
	float4 Clip = float4::ONE;
	float4 Flip = float4::ZERO;
	FW1_TEXT_FLAG FwTextFlag = FW1_TEXT_FLAG::FW1_TOP;
};

