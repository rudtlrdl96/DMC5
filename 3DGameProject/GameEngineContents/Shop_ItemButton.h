#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFont.h>
// 설명 :
class ShopTextParameter
{
public:
	std::string _Name = "";
	std::string _Price = "";
	int Index = 0;

};
class GameEngineCollision;
class Shop_ItemButton : public GameEngineActor
{
public:
	// constrcuter destructer
	Shop_ItemButton();
	~Shop_ItemButton();

	// delete Function
	Shop_ItemButton(const Shop_ItemButton& _Other) = delete;
	Shop_ItemButton(Shop_ItemButton&& _Other) noexcept = delete;
	Shop_ItemButton& operator=(const Shop_ItemButton& _Other) = delete;
	Shop_ItemButton& operator=(Shop_ItemButton&& _Other) noexcept = delete;
	std::shared_ptr<GameEngineUIRenderer> GetSkillRender()
	{
		return Skill_Render;
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
	void SetTextFont(const ShopTextParameter& _Parameter);
protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	void FontCreate();
	std::shared_ptr<GameEngineUIRenderer> Render = nullptr; //선택 아닐때 기본렌더
	std::shared_ptr<GameEngineUIRenderer> Render_Select = nullptr; // 선택되면 차오르는 놈
	std::shared_ptr<GameEngineUIRenderer> SkillBase_Render = nullptr; //선택 아닐때 기본 스킬창 틀
	std::shared_ptr<GameEngineUIRenderer> SkillSelect_Render = nullptr; // 선택시 러프로 커지며 테두리가 빛나는 렌더
	std::shared_ptr<GameEngineUIRenderer> Skill_Render = nullptr; // 선택과 아닐때 크기차이가 다름
	std::shared_ptr<GameEngineUIRenderer> BuyButton_Render = nullptr; // 버튼렌더

	std::shared_ptr<class GameEngineFontRenderer> NameText_Render = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> PriceText_Render = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> BuyText_Render = nullptr;


	std::function<void()> Click;
	std::string Font = "DMC5Font";
	std::string_view Text = "";
	bool IsSelect = false;
	bool IsValue = false;
	bool SwichValue = false;
	bool IsPosValue = false;

	float AddTime = 0.0f;
	float ScaleUpTime = 0.0f;
	float ScaleDownTime = 0.0f;

	float FallTime = 0.0f;
	float M0 = 0.0f;
	float4 UpScale = { 225.0f,142.0f,0.0f };
	float4 DownScale = { 157.0f,100.0f,0.0f };
};

