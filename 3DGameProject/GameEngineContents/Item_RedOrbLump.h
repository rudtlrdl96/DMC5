#pragma once
#include "FieldMapObject.h"

class Item_RedOrbLump : public FieldMapObject
{
public:
	// constructer destructer
	Item_RedOrbLump();
	~Item_RedOrbLump();

	// delete Function
	Item_RedOrbLump(const Item_RedOrbLump& _Other) = delete;
	Item_RedOrbLump(Item_RedOrbLump&& _Other) = delete;
	Item_RedOrbLump& operator=(const Item_RedOrbLump& _Other) = delete;
	Item_RedOrbLump& operator=(Item_RedOrbLump&& _Other) = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	std::shared_ptr<class FXSystem> Effect = nullptr;
	std::shared_ptr<class GameEngineCollision> Col = nullptr;
	std::vector<std::shared_ptr<class Item_RedOrb>> Orbs;
	std::shared_ptr<class PhysXCapsuleComponent> PhysX = nullptr;

	int HP = 8;
	void Damage();
	void Break();
};

