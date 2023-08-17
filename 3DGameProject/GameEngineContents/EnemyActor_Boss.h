#pragma once
#include "BaseEnemyActor.h"

// Ό³Έν :
class EnemyActor_Boss : public BaseEnemyActor
{
public:
	// constrcuter destructer
	EnemyActor_Boss();
	~EnemyActor_Boss();

	// delete Function
	EnemyActor_Boss(const EnemyActor_Boss& _Other) = delete;
	EnemyActor_Boss(EnemyActor_Boss&& _Other) noexcept = delete;
	EnemyActor_Boss& operator=(const EnemyActor_Boss& _Other) = delete;
	EnemyActor_Boss& operator=(EnemyActor_Boss&& _Other) noexcept = delete;

protected:
	void Start();
private:

};

