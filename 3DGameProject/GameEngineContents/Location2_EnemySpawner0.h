#pragma once
#include "EnemySpawner.h"

class Location2_EnemySpawner0 : public EnemySpawner
{
public:
	// construtor destructor
	Location2_EnemySpawner0();
	~Location2_EnemySpawner0();

	// delete Function
	Location2_EnemySpawner0(const Location2_EnemySpawner0& _Other) = delete;
	Location2_EnemySpawner0(Location2_EnemySpawner0&& _Other) noexcept = delete;
	Location2_EnemySpawner0& operator=(const Location2_EnemySpawner0& _Other) = delete;
	Location2_EnemySpawner0& operator=(Location2_EnemySpawner0&& _Other) noexcept = delete;

protected:
	void Start() override;

private:
	size_t EmpusaAliveCount = 0;

	void DestroyEmpusa();
};

