#pragma once

// 적이 드랍하는 레드오브
class Item_EnemyRedOrb : public GameEngineComponent
{
public:
	// constructer destructer
	Item_EnemyRedOrb();
	~Item_EnemyRedOrb();

	// delete Function
	Item_EnemyRedOrb(const Item_EnemyRedOrb& _Other) = delete;
	Item_EnemyRedOrb(Item_EnemyRedOrb&& _Other) = delete;
	Item_EnemyRedOrb& operator=(const Item_EnemyRedOrb& _Other) = delete;
	Item_EnemyRedOrb& operator=(Item_EnemyRedOrb&& _Other) = delete;

	void OrbOn();

protected:
	void Start() override;

private:
	std::vector<std::shared_ptr<class Item_RedOrb>> Orbs;
};

