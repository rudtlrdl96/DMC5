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
}

void EnemySpawner::Update(float _DeltaTime)
{
	EventZone::Update(_DeltaTime);

	if (!IsEventStart)
	{
		return;
	}

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