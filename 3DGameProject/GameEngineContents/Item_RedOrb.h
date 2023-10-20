#pragma once
#include "ContentsEnum.h"
#include "FieldMapObject.h"
// Ό³Έν :
class Item_RedOrb : public FieldMapObject
{
public:
	// constrcuter destructer
	Item_RedOrb();
	~Item_RedOrb();

	// delete Function
	Item_RedOrb(const Item_RedOrb& _Other) = delete;
	Item_RedOrb(Item_RedOrb&& _Other) noexcept = delete;
	Item_RedOrb& operator=(const Item_RedOrb& _Other) = delete;
	Item_RedOrb& operator=(Item_RedOrb&& _Other) noexcept = delete;

	void Take();
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class FXSystem> Effect = nullptr;
	std::shared_ptr<class GameEngineCollision> Col = nullptr;
};

