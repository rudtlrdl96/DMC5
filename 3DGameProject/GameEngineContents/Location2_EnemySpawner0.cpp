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
			Level->DynamicThis<StageBaseLevel>()->RedSealWallOn();

			for (size_t i = 0; i < 3; ++i)
			{
				std::shared_ptr<Enemy_Empusa> Enemy = nullptr;

				//�̱��̰ų� ȣ��Ʈ�϶��� ���͸� ���� �� �����͸� ��ȯ�մϴ�.(Ŭ���̾�Ʈ�� ��� nullptr ��ȯ)
				Enemy = NetworkManager::CreateNetworkActor<Enemy_Empusa>(Level, static_cast<int>(ActorOrder::Enemy));
				if (nullptr == Enemy)
					continue;

				++MonsterAliveCount;
				Enemy->GetPhysXComponent()->SetWorldPosition({ 0, 150.f * i, 0 });
				Enemy->PushDestroyCallback(std::bind(&EnemySpawner::DestroyMonster, this));
			}

			BGMPlayer::SetBattleBGM();
		};
}
