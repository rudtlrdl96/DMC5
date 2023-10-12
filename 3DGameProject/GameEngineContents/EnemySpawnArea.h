#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "BaseEnemyActor.h"


///	<param name="[Code] : ">스폰하려는 몬스터의 종류 </param>
///	<param name="[SpawnLocalPos] : ">몬스터의 스폰 위치를 정합니다 EnemySpawnArea 액터의 위치가 기준이 됩니다. </param>
///	<param name="[SpawnLocalRotation] : ">몬스터의 스폰 회전을 정합니다 EnemySpawnArea 액터의 위치가 기준이 됩니다. </param>
class EnemySpawnData
{
public:
	EnemyCode Code = EnemyCode::None;

	float4 SpawnLocalPos = float4::ZERO;
	float4 SpawnLocalRotation = float4::ZERO;
};

// 설명 :
class EnemySpawnArea : public GameEngineActor, public GameEngineNetObject
{
public:
	// constrcuter destructer
	EnemySpawnArea();
	~EnemySpawnArea();

	// delete Function
	EnemySpawnArea(const EnemySpawnArea& _Other) = delete;
	EnemySpawnArea(EnemySpawnArea&& _Other) noexcept = delete;
	EnemySpawnArea& operator=(const EnemySpawnArea& _Other) = delete;
	EnemySpawnArea& operator=(EnemySpawnArea&& _Other) noexcept = delete;

	// 웨이브에 몬스터 스폰 데이터를 추가합니다
	void InsertSpawnData(int _WaveOrder, EnemySpawnData _EnemyData);

	// 현재 웨이브의 몬스터를 소환합니다 웨이브 소환에 성공할 경우 true를 반환, 더 이상 웨이브 데이터가 없을 경우 false를 반환합니다
	bool PlayWave();

	// 웨이브를 리셋합니다 소환된 몬스터는 모두 삭제됩니다
	void WaveReset();

	// 모든 몬스터를 삭제합니다 사망 연출이 실행되지 않습니다.
	void AllEnemyClear();

	// 모든 스폰이 끝났는지 반환합니다.
	bool IsSpawnEnd();

	// 현재 소환된 몬스터가 모두 죽었는지 반환합니다
	bool IsWaveEnd();

protected:

private:
	std::map<int, std::vector<EnemySpawnData>> WaveDatas;
	std::vector<std::shared_ptr<BaseEnemyActor>> SpawnEnemys;

	int CurWave = 0;
	int SpawnIndex = 0;
};

