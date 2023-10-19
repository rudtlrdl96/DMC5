#include "PrecompileHeader.h"
#include "Location2_EnemySpawner1.h"
#include "Enemy_Empusa.h"
#include "StageBaseLevel.h"

Location2_EnemySpawner1::Location2_EnemySpawner1()
{

}

Location2_EnemySpawner1::~Location2_EnemySpawner1()
{

}

void Location2_EnemySpawner1::Start()
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
				Monsters[i].lock()->GetPhysXComponent()->SetWorldPosition({ 3725.f, 150.f * i, -8527.f });
			}
			BGMPlayer::SetBattleBGM();
		};
}
