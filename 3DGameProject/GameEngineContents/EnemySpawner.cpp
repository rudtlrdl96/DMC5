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

void EnemySpawner::SpawnEnemys()
{

}

void EnemySpawner::Start()
{
	//Monsters.push_back(GetLevel()->CreateActor<Enemy_Empusa>());

	//for (size_t i = 0; i < 3; i++)
	//{
	//	std::shared_ptr<Enemy_Empusa> Enemy = nullptr;
	//	Enemy = Poolable<Enemy_Empusa>::PopFromPool(GetLevel());
	//	Monsters.push_back(Enemy);
	//	AliveCount++;

	//	Enemy->PushPoolEvent([this]()
	//		{
	//			AliveCount--;

	//			if (0 != AliveCount)
	//				return;

	//			GetLevel()->DynamicThis<StageBaseLevel>()->RedSealWallOff();
	//			Death();
	//		});
	//}

	Monsters.push_back(Poolable<Enemy_Empusa>::PopFromPool(GetLevel()));
	Monsters.push_back(Poolable<Enemy_Empusa>::PopFromPool(GetLevel()));
	Monsters.push_back(Poolable<Enemy_Empusa>::PopFromPool(GetLevel()));

	for (size_t i = 0; i < Monsters.size(); i++)
	{
		Monsters[i].lock()->GetPhysXComponent()->SetWorldPosition({0, 100.f * i, 0});
	}
}

void EnemySpawner::Update(float _DeltaTime)
{
	bool IsAllDeath = true;
	for (size_t i = 0; i < Monsters.size(); i++)
	{
		IsAllDeath &= Monsters[i].expired();
	}

	if (IsAllDeath)
	{
		GetLevel()->DynamicThis<StageBaseLevel>()->RedSealWallOff();
		Death();
	}
}