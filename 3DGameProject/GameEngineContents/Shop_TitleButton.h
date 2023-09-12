#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFont.h>
// Ό³Έν :

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
	std::shared_ptr<GameEngineUIRenderer> GetRender()
	{
		return Render;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender_Select()
	{
		return Render_Select;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender_Bottom()
	{
		return Render_Bottom;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender_Top()
	{
		return Render_Top;
	}
	void SetEvent(std::function<void()> _Click)
	{
		Click = _Click;
	}
	void SetUIText(std::string_view _Text)
	{
		Text = _Text;
	};
	void SetSelectValue(bool _Value)
	{
		IsSelect = _Value;
	}
	void SetBlinkValue(bool _Value)
	{
		IsBlink = _Value;
	}
	bool GetIsValue()
	{
		return IsValue;
	}
	void SetIsValue(bool _Value)
	{
		IsValue = _Value;
	}
	bool GetIsPosValue()
	{
		return IsPosValue;
	}
	void SetIsPosValue(bool _Value)
	{
		IsPosValue = _Value;
	}
	std::shared_ptr<class GameEngineFontRenderer> GetFont()
	{
		return FontRender;
	}
protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	void FontCreate();
	void SetTextFont(std::string_view _Text);
	std::shared_ptr<GameEngineUIRenderer> Render = nullptr;
	std::shared_ptr<GameEngineUIRenderer> Render_Select = nullptr;
	std::shared_ptr<GameEngineUIRenderer> Render_Bottom = nullptr;
	std::shared_ptr<GameEngineUIRenderer> Render_Top = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> FontRender = nullptr;


	std::function<void()> Click;
	std::string Font = "DMC5Font";
	std::string_view Text = "";
	bool IsSelect = false;
	bool IsValue = false;
	bool IsBlink = false;
	bool SwichValue = false;
	bool IsPosValue = false;

	float AddTime = 0.0f;
	float Ratio = 0.0f;

	float M0 = 0.0f;
	
};

