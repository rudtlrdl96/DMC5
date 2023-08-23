#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
class StringParameter
{
public:
	std::string _Text = "";
	std::string _SeletText = "";
	std::string _ExplaneText = "";
	int Index = 0;

};
// 설명 :
class GameEngineCollision;
class ContentButton : public GameEngineActor
{
public:
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
	std::shared_ptr<GameEngineUIRenderer> GetRender_Select()
	{
		return Render_Select;
	}
	void SetUIText(const StringParameter& _Paramter);
protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	void FontCreate();
	std::shared_ptr<GameEngineUIRenderer> Render = nullptr;//폰트 안떠서 임시
	std::shared_ptr<GameEngineUIRenderer> Render_Select = nullptr;
	std::shared_ptr<class UIFontRender> Text = nullptr;
	std::shared_ptr<class UIFontRender> SeletText = nullptr;
	std::shared_ptr<class UIFontRender> ExplaneText = nullptr;
	std::function<void()> Click;

	std::string Font = "DMC5Font";

};

