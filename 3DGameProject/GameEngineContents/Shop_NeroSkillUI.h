#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class Shop_NeroSkillUI : public GameEngineActor
{
public:
	// constrcuter destructer
	Shop_NeroSkillUI();
	~Shop_NeroSkillUI();

	// delete Function
	Shop_NeroSkillUI(const Shop_NeroSkillUI& _Other) = delete;
	Shop_NeroSkillUI(Shop_NeroSkillUI&& _Other) noexcept = delete;
	Shop_NeroSkillUI& operator=(const Shop_NeroSkillUI& _Other) = delete;
	Shop_NeroSkillUI& operator=(Shop_NeroSkillUI&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	void LerpItem(float _Delta);
	void SetIndex();
	void SetExText();
	void SetValue();
	std::shared_ptr<class Shop_TitleButton> RedQueenButton = nullptr;
	std::shared_ptr<class Shop_ItemButton>  SkillButton = nullptr;
	std::shared_ptr<class Shop_ExplaneUI> ExplanePtr = nullptr;
	bool Test = false;

	int Index = 0;
	int PrevIndex = 5;
	bool Exception = false;
	bool IsValue = false;
	bool DownSwich = false;
	bool UpSwich = false;
	float LerpTime = 0.0f;
	float4 Pos_Null = { -190.0f,364.0f,0.0f };
	float4 Pos_0 = { -190.0f,234.0f,0.0f };
	float4 Pos_1 = { -190.0f,104.0f,0.0f };
	float4 Pos_2 = { -190.0f,-34.0f,0.0f };
	float4 Pos_3 = { -190.0f,-164.0f,0.0f };
	float4 Pos_4 = { -190.0f,-294.0f,0.0f };
	float4 Pos_5 = { -190.0f,-424.0f,0.0f };
};

