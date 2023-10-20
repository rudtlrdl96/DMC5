#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class Nero_ShopUI : public GameEngineActor
{
public:
	// constrcuter destructer
	Nero_ShopUI();
	~Nero_ShopUI();

	// delete Function
	Nero_ShopUI(const Nero_ShopUI& _Other) = delete;
	Nero_ShopUI(Nero_ShopUI&& _Other) noexcept = delete;
	Nero_ShopUI& operator=(const Nero_ShopUI& _Other) = delete;
	Nero_ShopUI& operator=(Nero_ShopUI&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	void LerpScreen(float _Delta);
	std::shared_ptr<class Shop_TitleButton> SkillButton = nullptr;
	std::shared_ptr<class Shop_TitleButton> DBButton = nullptr;

	std::shared_ptr<class Shop_NeroSkillUI> SkillScreen = nullptr;
	std::shared_ptr<class Shop_NeroArmUI> ArmScreen = nullptr;

	float LerpTime = 0.0f;
	int TitleIndex = 0;
	int PrevIndex = 0;
	bool IsValue = false;
	bool IsValue2 = false;
	bool IsSwichSkill = false;
	bool IsSwichItem = false;

	float4 SkillDefaultPos = { -1200.0f,0.0f,0.0f };
	float4 ItemDefaulPos = { -1200.f,0.0f,0.0f };
	float4 SkillStartPos = { 0.0f,0.0f,0.0f };
	float4 ItemStartPos = { -118.0f,0.0f,0.0f };
	float4 SkillEndPos = { 800.0f,0.0f,0.0f };
	float4 ItemEndPos = { 718.0f,0.0f,0.0f };
};

