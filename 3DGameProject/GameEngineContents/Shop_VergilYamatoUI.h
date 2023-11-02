#pragma once
#include <GameEngineCore/GameEngineActor.h>
// ���� :
class Shop_VergilYamatoUI : public GameEngineActor
{
public:
	// constrcuter destructer
	Shop_VergilYamatoUI();
	~Shop_VergilYamatoUI();

	// delete Function
	Shop_VergilYamatoUI(const Shop_VergilYamatoUI& _Other) = delete;
	Shop_VergilYamatoUI(Shop_VergilYamatoUI&& _Other) noexcept = delete;
	Shop_VergilYamatoUI& operator=(const Shop_VergilYamatoUI& _Other) = delete;
	Shop_VergilYamatoUI& operator=(Shop_VergilYamatoUI&& _Other) noexcept = delete;
	std::shared_ptr<class Shop_ExplaneUI> GetExPlane()
	{
		return ExplanePtr;
	}	
	int GetYamatoCount()
	{
		return Count;
	}
	static bool IsRapidSlash;
	static bool IsUpperSlash;
	static bool IsAerialRave;
	static bool IsYamatoCombo;
	static bool IsJudgmentCutEnd;
	void IsActiveYamato(bool _Value)
	{
		IsActive = _Value;
	}
protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	void LerpItem(float _Delta);
	void SetIndex();
	void SetExText();
	void SetValue();
	void ActivSkill();
	std::shared_ptr<class Shop_TitleButton> YamatoButton = nullptr;
	std::shared_ptr<class Shop_ItemButton>  SkillButton = nullptr;
	std::shared_ptr<class Shop_ExplaneUI> ExplanePtr = nullptr;
	bool Test = false;
	int Count = 0;
	int Index = 0;
	int PrevIndex = 4;
	bool IsActive = false;
	float4 Pos_0 = { -190.0f,234.0f,0.0f };
	float4 Pos_1 = { -190.0f,104.0f,0.0f };
	float4 Pos_2 = { -190.0f,-34.0f,0.0f };
	float4 Pos_3 = { -190.0f,-164.0f,0.0f };
	float4 Pos_4 = { -190.0f,-294.0f,0.0f };

};

