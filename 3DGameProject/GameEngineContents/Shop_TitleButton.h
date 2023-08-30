#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFont.h>
// ���� :
class GameEngineCollision;
class Shop_TitleButton : public GameEngineActor
{
public:
	// constrcuter destructer
	Shop_TitleButton();
	~Shop_TitleButton();

	// delete Function
	Shop_TitleButton(const Shop_TitleButton& _Other) = delete;
	Shop_TitleButton(Shop_TitleButton&& _Other) noexcept = delete;
	Shop_TitleButton& operator=(const Shop_TitleButton& _Other) = delete;
	Shop_TitleButton& operator=(Shop_TitleButton&& _Other) noexcept = delete;
	void SetEvent(std::function<void()> _Click)
	{
		Click = _Click;
	}
	void SetUIText(std::string_view& _Text)
	{
		Text = _Text;
	};
	void SetSelectValue(bool _Value)
	{
		IsSelect = _Value;
	}
	bool GetIsValue()
	{
		return IsValue;
	}
	void SetIsValue(bool _Value)
	{
		IsValue = _Value;
	}

protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	void FontCreate();
	void RenderOnOff();
	void SetTextFont(std::string_view& _Text);
	std::shared_ptr<GameEngineUIRenderer> Render = nullptr;
	std::shared_ptr<GameEngineUIRenderer> Render_Select = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> FontRender = nullptr;


	std::function<void()> Click;
	std::string Font = "DMC5Font";
	std::string_view Text = "";
	bool IsSelect = false;
	bool IsValue = false;
	bool SwichValue = false;
};

