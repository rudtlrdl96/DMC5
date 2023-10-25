#include "PrecompileHeader.h"
#include "Location2_EnemySpawner0.h"
#include "Enemy_Empusa.h"
#include "StageBaseLevel.h"
#include "BGMPlayer.h"
#include "NetworkManager.h"

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
			BGMPlayer::SetBattleBGM();
			GameEngineLevel* Level = GetLevel();
			Level->DynamicThis<StageBaseLevel>()->RedSealWallOn();
			MonsterAliveCount = 3;
			NetworkObjectBase::PushReservedDestroyCallback(Net_ActorType::Empusa, std::bind(&EnemySpawner::DestroyMonster, this));

			if (false == NetworkManager::IsClient())
			{
				std::vector<float4> EnemyPos =
				{
					{ 1029, 55, 1337 }, { 91, 55, 82 }, { -1228, 55, 712 }
				};
				for (size_t i = 0; i < 3; ++i)
				{
					std::shared_ptr<Enemy_Empusa> Empusa = Poolable<Enemy_Empusa>::PopFromPool(Level, static_cast<int>(ActorOrder::Enemy));
					Empusa->GetPhysXComponent()->SetWorldPosition(EnemyPos[i]);
					Empusa->GetTransform()->SetWorldPosition(EnemyPos[i]);
					Empusa->PushDeathCallback(std::bind(&EnemySpawner::DestroyMonster, this));
				}
			}
		};
}
