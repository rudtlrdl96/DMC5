#pragma once
#include "EnemySpawner.h"
class Location2_EnemySpawner1 : public EnemySpawner
{
public:
	// construtor destructor
	Location2_EnemySpawner1();
	~Location2_EnemySpawner1();

	// delete Function
	Location2_EnemySpawner1(const Location2_EnemySpawner1& _Other) = delete;
	Location2_EnemySpawner1(Location2_EnemySpawner1&& _Other) noexcept = delete;
	Location2_EnemySpawner1& operator=(const Location2_EnemySpawner1& _Other) = delete;
	Location2_EnemySpawner1& operator=(Location2_EnemySpawner1&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

