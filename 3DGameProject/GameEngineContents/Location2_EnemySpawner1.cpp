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

				//�̱��̰ų� ȣ��Ʈ�϶��� ���͸� ���� �� �����͸� ��ȯ�մϴ�.(Ŭ���̾�Ʈ�� ��� nullptr ��ȯ)
				Enemy = NetworkManager::CreateNetworkActor<Enemy_Empusa>(Level, static_cast<int>(ActorOrder::Enemy));
				if (nullptr == Enemy)
					continue;

				++MonsterAliveCount;
				Enemy->GetPhysXComponent()->SetWorldPosition({ 3725.f, 150.f * i, -8527.f });
				Enemy->PushDeathCallback(std::bind(&EnemySpawner::DestroyMonster, this));
			}

			BGMPlayer::SetBattleBGM();
		};
}
