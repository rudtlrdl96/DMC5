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
	size_t MonsterAliveCount = 0;

	float4 CutScenePosStart = float4::ZERO;	// 전투 끝날때 컷신 카메라 위치 Start
	float4 CutScenePosEnd = float4::ZERO;	// 전투 끝날때 컷신 카메라 위치 End
	float4 CutSceneRotStart = float4::ZERO;	// 전투 끝날때 컷신 카메라 회전 Start
	float4 CutSceneRotEnd = float4::ZERO;	// 전투 끝날때 컷신 카메라 회전 End
private:
	int MonsterWaveCount = 0;
};

