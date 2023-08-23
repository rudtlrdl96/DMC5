#pragma once
#include <GameEngineCore/GameEngineFontRenderer.h>
// ���� :
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

	std::string const GetDMCFont()
	{
		return "DMC5Font";
	}
protected:
	void Start() override;

private:

};

