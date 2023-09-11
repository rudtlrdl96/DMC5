#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class Vergil_ShopTitleBar : public GameEngineActor
{
public:
	// constrcuter destructer
	Vergil_ShopTitleBar();
	~Vergil_ShopTitleBar();

	// delete Function
	Vergil_ShopTitleBar(const Vergil_ShopTitleBar& _Other) = delete;
	Vergil_ShopTitleBar(Vergil_ShopTitleBar&& _Other) noexcept = delete;
	Vergil_ShopTitleBar& operator=(const Vergil_ShopTitleBar& _Other) = delete;
	Vergil_ShopTitleBar& operator=(Vergil_ShopTitleBar&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	void LerpScreen(float _Delta);
	std::shared_ptr<class Shop_TitleButton> SkillButton = nullptr;
	std::shared_ptr<class Shop_VergilYamatoUI> YamatoScreen = nullptr;

	float LerpTime = 0.0f;
	int TitleIndex = 0;
	int PrevIndex = 0;
	bool IsValue = false;
	bool IsValue2 = false;
	bool IsSwichSkill = false;
	bool IsSwichItem = false;

};

