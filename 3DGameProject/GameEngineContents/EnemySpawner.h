#pragma once
#include "FieldMapObject.h"

class EnemySpawner : public FieldMapObject
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

	void SpawnEnemys();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::vector<float4> EnemyPositions = std::vector<float4>();
	std::vector<std::weak_ptr<class Enemy_Empusa>> Monsters;
	size_t AliveCount = 0;
};

