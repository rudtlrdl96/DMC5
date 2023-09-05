#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFont.h>
// ���� :
class ShopTextParameter
{
public:
	std::string _Name = "";
	std::string _Price = "";
	std::string _png = "";
};
class GameEngineCollision;
class Shop_ItemButton : public GameEngineActor
{
public:
	static std::vector<std::shared_ptr<Shop_ItemButton>> Items;

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
	static void CreateItemUI(GameEngineLevel* _Level, float4 _Pos, const ShopTextParameter& _Param, GameEngineTransform* _Trans);

protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	void FontCreate();
	void RenderOnOff();
	void BlinkRender(float _Delta);
	void MoveRender(float _Delta);
	void SizeUpDown(float _Delta);

	float4 LerpScaleSize(float4 _Start, float4 _End, float _Ratio); // ���������� �����
	std::shared_ptr<GameEngineUIRenderer> Render = nullptr; //���� �ƴҶ� �⺻����
	std::shared_ptr<GameEngineUIRenderer> Render_Select = nullptr; // ���õǸ� �������� ��
	std::shared_ptr<GameEngineUIRenderer> SkillBase_Render = nullptr; //���� �ƴҶ� �⺻ ��ųâ Ʋ
	std::shared_ptr<GameEngineUIRenderer> SkillSelect_Render = nullptr; // ���ý� ������ Ŀ���� �׵θ��� ������ ����
	std::shared_ptr<GameEngineUIRenderer> Skill_Render = nullptr; // ���ð� �ƴҶ� ũ�����̰� �ٸ�
	std::shared_ptr<GameEngineUIRenderer> BuyButton_Render = nullptr; // ��ư����
	std::shared_ptr<GameEngineUIRenderer> Money_Render = nullptr; // ��
	std::shared_ptr<class GameEngineFontRenderer> NameText_Render = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> PriceText_Render = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> BuyText_Render = nullptr;


	std::function<void()> Click;
	std::string Font = "DMC5Font";
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

