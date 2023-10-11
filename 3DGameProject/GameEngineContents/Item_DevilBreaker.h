#pragma once
#include "ContentsEnum.h"
// Ό³Έν :
class Item_DevilBreaker : public GameEngineActor
{
public:
	// constrcuter destructer
	Item_DevilBreaker();
	~Item_DevilBreaker();

	// delete Function
	Item_DevilBreaker(const Item_DevilBreaker& _Other) = delete;
	Item_DevilBreaker(Item_DevilBreaker&& _Other) noexcept = delete;
	Item_DevilBreaker& operator=(const Item_DevilBreaker& _Other) = delete;
	Item_DevilBreaker& operator=(Item_DevilBreaker&& _Other) noexcept = delete;

	void SetDevilBreaker(DevilBreaker _Value);
	DevilBreaker Take();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	DevilBreaker Type = DevilBreaker::None;
	std::shared_ptr<class GameEngineFBXRenderer> ArmRenderer = nullptr;
	std::shared_ptr<class EffectRenderer> NameRenderer = nullptr;
	std::shared_ptr<class GameEngineCollision> Col = nullptr;
};

