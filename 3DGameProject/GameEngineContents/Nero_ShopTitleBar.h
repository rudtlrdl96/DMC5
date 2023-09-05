#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class Nero_ShopTitleBar : public GameEngineActor
{
public:
	// constrcuter destructer
	Nero_ShopTitleBar();
	~Nero_ShopTitleBar();

	// delete Function
	Nero_ShopTitleBar(const Nero_ShopTitleBar& _Other) = delete;
	Nero_ShopTitleBar(Nero_ShopTitleBar&& _Other) noexcept = delete;
	Nero_ShopTitleBar& operator=(const Nero_ShopTitleBar& _Other) = delete;
	Nero_ShopTitleBar& operator=(Nero_ShopTitleBar&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	std::shared_ptr<class Shop_TitleButton> SkillButton = nullptr;
	std::shared_ptr<class Shop_TitleButton> DBButton = nullptr;

	std::shared_ptr<class Shop_NeroSkillUI> RQButton = nullptr;

	float LerpTime = 0.0f;
	int TitleIndex = 0;

	bool IsSwich = false;
};

