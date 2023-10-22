#include "PrecompileHeader.h"
#include "EnemySpawner.h"
#include "Enemy_Empusa.h"
#include "TestStageLevel.h"
EnemySpawner::EnemySpawner()
{

}

EnemySpawner::~EnemySpawner()
{

}

void EnemySpawner::Start()
{
	EventZone::Start();
}

void EnemySpawner::DestroyMonster()
{
	--MonsterAliveCount;

	if (0 < MonsterAliveCount)
		return;

	BGMPlayer::SetBattleEnd();
	GetLevel()->DynamicThis<StageBaseLevel>()->RedSealWallOff();
	Death();
}