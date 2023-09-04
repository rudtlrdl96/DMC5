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
	std::shared_ptr<class Shop_TitleButton> RedQueenButton = nullptr;
	std::shared_ptr<class Shop_ItemButton>  SkillButton = nullptr;
	bool Test = false;
	
};

