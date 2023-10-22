#include "PrecompileHeader.h"
#include "Location2_EnemySpawner1.h"
#include "Enemy_Empusa.h"
#include "StageBaseLevel.h"
#include "BGMPlayer.h"
#include "NetworkManager.h"

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
			GameEngineLevel* Level = GetLevel();
			Level->DynamicThis<StageBaseLevel>()->RedSealWallOn();

			for (size_t i = 0; i < 3; ++i)
			{
				std::shared_ptr<Enemy_Empusa> Enemy = nullptr;

				//싱글이거나 호스트일때만 몬스터를 생성 후 포인터를 반환합니다.(클라이언트인 경우 nullptr 반환)
				Enemy = NetworkManager::CreateNetworkActor<Enemy_Empusa>(Level, static_cast<int>(ActorOrder::Enemy));
				if (nullptr == Enemy)
					continue;

				++MonsterAliveCount;
				Enemy->GetPhysXComponent()->SetWorldPosition({ 3725.f, 150.f * i, -8527.f });
				Enemy->PushDestroyCallback(std::bind(&EnemySpawner::DestroyMonster, this));
			}

			BGMPlayer::SetBattleBGM();
		};
}
