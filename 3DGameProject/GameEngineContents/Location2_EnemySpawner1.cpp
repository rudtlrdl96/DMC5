#include "PrecompileHeader.h"
#include "Location2_EnemySpawner1.h"
#include "Enemy_HellCaina.h"
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

	//Event = [this]()
	//	{
	//		GameEngineLevel* Level = GetLevel();
	//		//�� �κ��� Host�� �ƴ� Ŭ�󿡼��� ����˴ϴ�.
	//		if (true == NetworkManager::IsClient())
	//		{
	//			BGMPlayer::SetBattleBGM();
	//			MonsterAliveCount = 3;
	//			Level->DynamicThis<StageBaseLevel>()->RedSealWallOn();
	//			NetworkObjectBase::PushReservedDestroyCallback(Net_ActorType::Empusa, std::bind(&EnemySpawner::DestroyMonster, this));
	//			return;
	//		}

	//		Level->DynamicThis<StageBaseLevel>()->RedSealWallOn();

	//		for (size_t i = 0; i < 3; ++i)
	//		{
	//			std::shared_ptr<Enemy_HellCaina> Enemy = nullptr;

	//			//�̱��̰ų� ȣ��Ʈ�϶��� ���͸� ���� �� �����͸� ��ȯ�մϴ�.(Ŭ���̾�Ʈ�� ��� nullptr ��ȯ)
	//			Enemy = NetworkManager::CreateNetworkActor<Enemy_HellCaina>(Level, static_cast<int>(ActorOrder::Enemy));
	//			if (nullptr == Enemy)
	//				continue;

	//			++MonsterAliveCount;
	//			Enemy->GetPhysXComponent()->SetWorldPosition({ 3725.f, 150.f * i, -8527.f });
	//			Enemy->PushDeathCallback(std::bind(&EnemySpawner::DestroyMonster, this));
	//		}

	//		BGMPlayer::SetBattleBGM();
	//	};

	Event = [this]()
	{
		BGMPlayer::SetBattleBGM();
		GameEngineLevel* Level = GetLevel();
		Level->DynamicThis<StageBaseLevel>()->RedSealWallOn();
		MonsterAliveCount = 4;
		NetworkObjectBase::PushReservedDestroyCallback(Net_ActorType::HellCaina, std::bind(&EnemySpawner::DestroyMonster, this));

		if (false == NetworkManager::IsClient())
		{
			std::vector<float4> CainaPos =
			{
				{ 2808 , 86, -8972 }, { 3178 , 86, -9266 }, { 4095 , 86, -9370 }, { 4443 , 86, -9254 }
			};
			for (size_t i = 0; i < 4; ++i)
			{
				std::shared_ptr<Enemy_HellCaina> Caina = Poolable<Enemy_HellCaina>::PopFromPool(Level, static_cast<int>(ActorOrder::Enemy));
				Caina->GetPhysXComponent()->SetWorldPosition(CainaPos[i]);
				Caina->GetPhysXComponent()->SetLinearVelocityZero();
				Caina->GetTransform()->SetWorldPosition(CainaPos[i]);
				Caina->GetPhysXComponent()->Off();
				Caina->PushDeathCallback(std::bind(&EnemySpawner::DestroyMonster, this));
			}
		}
	};
}
