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
	std::string _png = "";
	std::string _BaseNone = "";
	std::string _BaseSelect = "";
	bool IsValue = false;
	int Value = 0;
};
class GameEngineCollision;
class Shop_ItemButton : public GameEngineActor
{
public:
	static std::vector<std::shared_ptr<Shop_ItemButton>> Items;
	static std::vector<std::shared_ptr<Shop_ItemButton>> Skills; 
		static std::vector<std::shared_ptr<Shop_ItemButton>> SecoundSkills;
	// constrcuter destructer
	Shop_ItemButton();
	~Shop_ItemButton();

	// delete Function
	Shop_ItemButton(const Shop_ItemButton& _Other) = delete;
	Shop_ItemButton(Shop_ItemButton&& _Other) noexcept = delete;
	Shop_ItemButton& operator=(const Shop_ItemButton& _Other) = delete;
	Shop_ItemButton& operator=(Shop_ItemButton&& _Other) noexcept = delete;
	std::shared_ptr<GameEngineUIRenderer> GetRender()
	{
		return Render;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender_Select()
	{
		return Render_Select;
	}
	std::shared_ptr<GameEngineUIRenderer> GetSkillRender()
	{
		return Skill_Render;
	}
	std::shared_ptr<GameEngineUIRenderer> GetBaseRender()
	{
		return SkillBase_Render;
	}
	std::shared_ptr<GameEngineUIRenderer> GetSelectRender()
	{
		return SkillSelect_Render;
	}
	std::shared_ptr<class GameEngineFontRenderer> GetPriceText()
	{
		return PriceText_Render;
	}
	std::shared_ptr<class GameEngineFontRenderer> GetNameText()
	{
		return NameText_Render;
	}
	void SetEvent(std::function<void()> _Click)
	{
		Click = _Click;
	}

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
	void SetSizeValue(bool _Value)
	{
		SizeValue = _Value;
	}
	static void CreateItemUI(GameEngineLevel* _Level, float4 _Pos, const ShopTextParameter& _Param, GameEngineTransform* _Trans,float4 _RenderScale,float4 _RenderPos);

protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	static bool IsScaleValue;
	void FontCreate();
	void RenderOnOff();
	void BlinkRender(float _Delta);
	void MoveRender(float _Delta);
	void SizeUpDown(float _Delta);

	float4 LerpScaleSize(float4 _Start, float4 _End, float _Ratio); // 공통기능으로 만들기
	std::shared_ptr<GameEngineUIRenderer> Render = nullptr; //선택 아닐때 기본렌더
	std::shared_ptr<GameEngineUIRenderer> Render_Select = nullptr; // 선택되면 차오르는 놈
	std::shared_ptr<GameEngineUIRenderer> SkillBase_Render = nullptr; //선택 아닐때 기본 스킬창 틀
	std::shared_ptr<GameEngineUIRenderer> SkillSelect_Render = nullptr; // 선택시 러프로 커지며 테두리가 빛나는 렌더
	std::shared_ptr<GameEngineUIRenderer> Skill_Render = nullptr; // 선택과 아닐때 크기차이가 다름
	std::shared_ptr<GameEngineUIRenderer> BuyButton_Render = nullptr; // 버튼렌더
	std::shared_ptr<GameEngineUIRenderer> Money_Render = nullptr; // 돈
	std::shared_ptr<class GameEngineFontRenderer> NameText_Render = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> PriceText_Render = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> BuyText_Render = nullptr;


	std::function<void()> Click;
	std::string Font = "DMC5Font";
	bool SizeValue = false;
	bool IsSelect = false;
	bool IsValue = false;
	bool SwichValue = false;
	bool IsPosValue = false;
	float AddTime = 0.0f;
	float ScaleUpTime = 0.0f;
	float ScaleDownTime = 0.0f;
	float M0 = 0.0f;
	float FallTime = 0.0f;
	float FillTime = 0.0f;
	float Ratio = 10.0f;
	float4 UpScale = { 225.0f,142.0f,0.0f };
	float4 DownScale = { 157.0f,100.0f,0.0f };


	float4 NoneColor = { 0.431f,0.589f,0.627f };
	float4 SelectColor = { 0.69f,0.792f,0.827f };
};

