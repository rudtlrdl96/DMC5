#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "Nero_InvenToryUI.h"
// Ό³Έν :
class Nero_Inven :public GameEngineActor
{
public:
	
	// constrcuter destructer
	Nero_Inven();
	~Nero_Inven();

	// delete Function
	Nero_Inven(const Nero_Inven& _Other) = delete;
	Nero_Inven(Nero_Inven&& _Other) noexcept = delete;
	Nero_Inven& operator=(const Nero_Inven& _Other) = delete;
	Nero_Inven& operator=(Nero_Inven&& _Other) noexcept = delete;

	std::vector<std::shared_ptr<class Nero_InvenToryUI>> GetInvenVec()
	{
		return Inventory;
	}

protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	
	std::vector<std::shared_ptr<class Nero_InvenToryUI>> Inventory;
	std::shared_ptr<class Nero_InvenToryUI> Inven0 = nullptr;
	std::shared_ptr<class Nero_InvenToryUI> Inven1 = nullptr;
	std::shared_ptr<class Nero_InvenToryUI> Inven2 = nullptr;
	std::list<DevilBreaker>* ArmList;

};

