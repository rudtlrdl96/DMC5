#include "PrecompileHeader.h"
#include "Location2_EnemySpawner0.h"
#include "Enemy_Empusa.h"
#include "StageBaseLevel.h"
#include "BGMPlayer.h"
Location2_EnemySpawner0::Location2_EnemySpawner0()
{

}

Location2_EnemySpawner0::~Location2_EnemySpawner0()
{

}

void Location2_EnemySpawner0::Start()
{
	EnemySpawner::Start();

	Event = [this]()
		{
			GetLevel()->DynamicThis<StageBaseLevel>()->RedSealWallOn();

			Monsters.push_back(Poolable<Enemy_Empusa>::PopFromPool(GetLevel()));
			Monsters.push_back(Poolable<Enemy_Empusa>::PopFromPool(GetLevel()));
			Monsters.push_back(Poolable<Enemy_Empusa>::PopFromPool(GetLevel()));

			for (size_t i = 0; i < Monsters.size(); i++)
			{
				Monsters[i].lock()->GetPhysXComponent()->SetWorldPosition({ 0, 150.f * i, 0 });
			}
			BGMPlayer::SetBattleBGM();
		};
}
