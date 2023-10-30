#pragma once
#include "EnemySpawner.h"

// Ό³Έν :
class Location19_EnemySpawner : public EnemySpawner
{
public:
	// constrcuter destructer
	Location19_EnemySpawner();
	~Location19_EnemySpawner();

	// delete Function
	Location19_EnemySpawner(const Location19_EnemySpawner& _Other) = delete;
	Location19_EnemySpawner(Location19_EnemySpawner&& _Other) noexcept = delete;
	Location19_EnemySpawner& operator=(const Location19_EnemySpawner& _Other) = delete;
	Location19_EnemySpawner& operator=(Location19_EnemySpawner&& _Other) noexcept = delete;

protected:

private:

};

