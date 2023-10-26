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

	void DestroyMonster();
protected:
	void Start() override;

	std::vector<std::function<void()>> MonsterWave_Events;
	std::function<void()> BattleEndEvent = nullptr;
	size_t MonsterAliveCount = 0;

	float4 CutScenePosStart = float4::ZERO;	// ���� ������ �ƽ� ī�޶� ��ġ Start
	float4 CutScenePosEnd = float4::ZERO;	// ���� ������ �ƽ� ī�޶� ��ġ End
	float4 CutSceneRotStart = float4::ZERO;	// ���� ������ �ƽ� ī�޶� ȸ�� Start
	float4 CutSceneRotEnd = float4::ZERO;	// ���� ������ �ƽ� ī�޶� ȸ�� End
private:
	int MonsterWaveCount = 0;
};

