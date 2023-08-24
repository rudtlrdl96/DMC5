#pragma once
#include "BaseLevel.h"

// Ό³Έν :
class EnemyTestLevel : public BaseLevel
{
public:
	// constrcuter destructer
	EnemyTestLevel();
	~EnemyTestLevel();

	// delete Function
	EnemyTestLevel(const EnemyTestLevel& _Other) = delete;
	EnemyTestLevel(EnemyTestLevel&& _Other) noexcept = delete;
	EnemyTestLevel& operator=(const EnemyTestLevel& _Other) = delete;
	EnemyTestLevel& operator=(EnemyTestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;

	void LevelChangeStart() override;

private:


};

