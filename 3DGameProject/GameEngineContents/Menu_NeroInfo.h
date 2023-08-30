#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "InfoUIRender.h"
#include "InvenUIButton.h"
// Ό³Έν :
class Menu_NeroInfo : public GameEngineActor
{
public:
	// constrcuter destructer
	Menu_NeroInfo();
	~Menu_NeroInfo();

	// delete Function
	Menu_NeroInfo(const Menu_NeroInfo& _Other) = delete;
	Menu_NeroInfo(Menu_NeroInfo&& _Other) noexcept = delete;
	Menu_NeroInfo& operator=(const Menu_NeroInfo& _Other) = delete;
	Menu_NeroInfo& operator=(Menu_NeroInfo&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;

private:

	std::shared_ptr<InfoUIRender> NeroInfoPtr = nullptr;
	std::shared_ptr<InfoUIRender> NeroInvenPtr = nullptr;
	std::shared_ptr<InvenUIButton> Arrow1Ptr = nullptr;
	std::shared_ptr<InvenUIButton> Arrow2Ptr = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> FontRender = nullptr;


	std::vector<int> Test;
	int Index = 0;
	int PrevIndex = 0;
	bool InvenPlusValue = false;
	bool InvenMinusValue = false;
};

