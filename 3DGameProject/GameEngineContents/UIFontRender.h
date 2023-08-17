#pragma once
#include <GameEngineCore/GameEngineFontRenderer.h>
// Ό³Έν :
class UIFontRender :public GameEngineFontRenderer
{
public:
	// constrcuter destructer
	UIFontRender();
	~UIFontRender();

	// delete Function
	UIFontRender(const UIFontRender& _Other) = delete;
	UIFontRender(UIFontRender&& _Other) noexcept = delete;
	UIFontRender& operator=(const UIFontRender& _Other) = delete;
	UIFontRender& operator=(UIFontRender&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

