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

	std::vector<std::weak_ptr<class BaseEnemyActor>> Monsters;
	size_t MonsterAliveCount = 0;
	void DestroyMonster();

private:
	std::vector<float4> EnemyPositions = std::vector<float4>();
};

