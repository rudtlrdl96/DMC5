#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class Shop_VergilMirgeUI : public GameEngineActor
{
public:
	// constrcuter destructer
	Shop_VergilMirgeUI();
	~Shop_VergilMirgeUI();

	// delete Function
	Shop_VergilMirgeUI(const Shop_VergilMirgeUI& _Other) = delete;
	Shop_VergilMirgeUI(Shop_VergilMirgeUI&& _Other) noexcept = delete;
	Shop_VergilMirgeUI& operator=(const Shop_VergilMirgeUI& _Other) = delete;
	Shop_VergilMirgeUI& operator=(Shop_VergilMirgeUI&& _Other) noexcept = delete;
	std::shared_ptr<class Shop_ExplaneUI> GetExPlane()
	{
		return ExplanePtr;
	}
	static bool IsSpiralBlade;
	static bool IsStormBlade;
	static bool IsLesteringBlade;
	static bool IsHeavyRainBlade;
protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	void SetIndex();
	void SetExText();
	void SetValue();
	void ActivSkill();
	std::shared_ptr<class Shop_TitleButton> YamatoButton = nullptr;
	std::shared_ptr<class Shop_ItemButton>  SkillButton = nullptr;
	std::shared_ptr<class Shop_ExplaneUI> ExplanePtr = nullptr;
	bool Test = false;

	int Index = 0;
	int PrevIndex = 3;
	float4 Pos_0 = { -190.0f,234.0f,0.0f };
	float4 Pos_1 = { -190.0f,104.0f,0.0f };
	float4 Pos_2 = { -190.0f,-34.0f,0.0f };
	float4 Pos_3 = { -190.0f,-164.0f,0.0f };

};

