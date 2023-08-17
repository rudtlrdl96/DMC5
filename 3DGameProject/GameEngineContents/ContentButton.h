#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
class StatStringParameter
{
public:
	std::string Name = "";
	std::string ExplaneName = "";
	std::string TwoExplaneName = "";
	std::string Value = "";
	int Index = 0;

};
// Ό³Έν :
class GameEngineUIRenderer;
class GameEngineCollision;
class ContentButton : public GameEngineActor
{
public:
	static ContentButton* SelectButton;

	// constrcuter destructer
	ContentButton();
	~ContentButton();

	// delete Function
	ContentButton(const ContentButton& _Other) = delete;
	ContentButton(ContentButton&& _Other) noexcept = delete;
	ContentButton& operator=(const ContentButton& _Other) = delete;
	ContentButton& operator=(ContentButton&& _Other) noexcept = delete;
	void SetEvent(std::function<void()> _Click)
	{
		Click = _Click;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender()
	{
		return Render;
	}

	std::shared_ptr<GameEngineUIRenderer> GetExplaneRender()
	{
		return ExplaneRender;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender_Select()
	{
		return Render_Select;
	}
	void SetUIText(const StatStringParameter& _Paramter);
protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	void FontCreate();
	std::shared_ptr<GameEngineUIRenderer> Render;
	std::shared_ptr<GameEngineUIRenderer> Render_Select;
	std::shared_ptr<GameEngineUIRenderer> ExplaneRender;
	std::shared_ptr<class UIFontRender> StatName;
	std::shared_ptr<class UIFontRender> StatExplane;
	std::shared_ptr<class UIFontRender> TwoExplane;
	std::shared_ptr<class UIFontRender> StatValue;
	std::function<void()> Click;

	std::string Font = "DMC5Font";

	float4 YellowColor = { 1.0f,0.93f,0.721f };

};

