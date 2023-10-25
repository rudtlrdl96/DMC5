#pragma once
#include "EventZone.h"
#include "BGMPlayer.h"

class EnemySpawner : public EventZone
{
public:
	// construtor destructor
	EnemySpawner();
	~EnemySpawner();

	// delete Function
	EnemySpawner(const EnemySpawner& _Other) = delete;
	EnemySpawner(EnemySpawner&& _Other) noexcept = delete;
	EnemySpawner& operator=(const EnemySpawner& _Other) = delete;
	EnemySpawner& operator=(EnemySpawner&& _Other) noexcept = delete;

protected:
	void Start() override;
	void DestroyMonster();
	size_t MonsterAliveCount = 0;

private:
	std::vector<float4> EnemyPositions = std::vector<float4>();
};

