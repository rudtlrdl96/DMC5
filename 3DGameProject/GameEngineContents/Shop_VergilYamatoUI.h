#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
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
protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	void LerpItem(float _Delta);
	void SetIndex();
	void SetExText();
	void SetValue();
	std::shared_ptr<class Shop_TitleButton> YamatoButton = nullptr;
	std::shared_ptr<class Shop_ItemButton>  SkillButton = nullptr;
	std::shared_ptr<class Shop_ExplaneUI> ExplanePtr = nullptr;
	bool Test = false;

	int Index = 0;
	int PrevIndex = 4;
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

