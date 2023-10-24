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
			GameEngineLevel* Level = GetLevel();
			if (true == NetworkManager::IsClient())
			{
				BGMPlayer::SetBattleBGM();
				MonsterAliveCount = 3;
				Level->DynamicThis<StageBaseLevel>()->RedSealWallOn();
				return;
			}

			Level->DynamicThis<StageBaseLevel>()->RedSealWallOn();

			for (size_t i = 0; i < 3; ++i)
			{
				std::shared_ptr<Enemy_Empusa> Enemy = nullptr;

				//싱글이거나 호스트일때만 몬스터를 생성 후 포인터를 반환합니다.(클라이언트인 경우 nullptr 반환)
				//Enemy = NetworkManager::CreateNetworkActor<Enemy_Empusa>(Level, static_cast<int>(ActorOrder::Enemy));
				Enemy = Poolable<Enemy_Empusa>::PopFromPool(Level, static_cast<int>(ActorOrder::Enemy));
				if (nullptr == Enemy)
					continue;

				++MonsterAliveCount;
				Enemy->GetPhysXComponent()->SetWorldPosition({ 0, 150.f * i, 0 });
				Enemy->PushDeathCallback(std::bind(&EnemySpawner::DestroyMonster, this));
			}

			BGMPlayer::SetBattleBGM();
		};
}
