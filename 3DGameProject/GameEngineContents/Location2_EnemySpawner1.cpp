#include "PrecompileHeader.h"
#include "Location2_EnemySpawner1.h"
#include "Enemy_HellCaina.h"
#include "StageBaseLevel.h"
#include "BGMPlayer.h"
#include "NetworkManager.h"
#include "BasePlayerActor.h"
Location2_EnemySpawner1::Location2_EnemySpawner1()
{
	CutScenePosStart = { 2320, 1090, -8865 };
	CutScenePosEnd = { 2618, 821, -9430 };

	CutSceneRotStart = { 15, 137, 0 };
	CutSceneRotEnd = { 3, 153, 0 };
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
	//		//이 부분은 Host가 아닌 클라에서만 실행됩니다.
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

	//			//싱글이거나 호스트일때만 몬스터를 생성 후 포인터를 반환합니다.(클라이언트인 경우 nullptr 반환)
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
		GameEngineLevel* Level = GetLevel();
		BasePlayerActor::GetMainPlayer()->SetCutScene({ 2842, 1011, -8469 }, { 3128, 1283, -10125 }, { 28, 41, 0 }, { 33, 28, 0 }, 6.0f);
		Level->TimeEvent.AddEvent(1.0f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
			{
				Level->DynamicThis<StageBaseLevel>()->RedSealWallOn();
			});
		BGMPlayer::SetBattleBGM();
		MonsterAliveCount = 2;
		NetworkObjectBase::PushReservedDestroyCallback(Net_ActorType::HellCaina, std::bind(&EnemySpawner::DestroyMonster, this));

		if (false == NetworkManager::IsClient())
		{
			std::vector<float4> EnemyPos =
			{
				{ 3652 , 86, -9166 }, { 4289 , 86, -8878 }, { 3032 , 86, -9015 }
			};
			std::vector<float> EnemyRot = { 0, -16, 35};

			for (size_t i = 0; i < 3; ++i)
			{
				Level->TimeEvent.AddEvent(i * 0.3f + 2.0f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
				{
					std::shared_ptr<Enemy_HellCaina> Enemy = Poolable<Enemy_HellCaina>::PopFromPool(Level, static_cast<int>(ActorOrder::Enemy));
					Enemy->GetPhysXComponent()->SetWorldPosition(EnemyPos[i]);
					Enemy->GetPhysXComponent()->SetWorldRotation(float4::UP * EnemyRot[i]);
					Enemy->GetTransform()->SetWorldPosition(EnemyPos[i]);
					Enemy->GetTransform()->SetWorldRotation(float4::UP * EnemyRot[i]);
					Enemy->GetPhysXComponent()->SetLinearVelocityZero();
					Enemy->GetPhysXComponent()->Off();
					Enemy->PushDeathCallback(std::bind(&EnemySpawner::DestroyMonster, this));
				});
			}
		}
	};

	MonsterWave_Events.resize(1);
	MonsterWave_Events[0] = [this]()
		{
			MonsterAliveCount = 4;
			if (false == NetworkManager::IsClient())
			{
				GameEngineLevel* Level = GetLevel();
				std::vector<float4> EnemyPos =
				{
					{ 2984 , 86, -10050 }, { 4268 , 86, -10000 }, { 3658 , 86, -7647 }
				};
				std::vector<float> EnemyRot = { 28, -10, -174 };

				for (size_t i = 0; i < 3; ++i)
				{
					Level->TimeEvent.AddEvent(i * 0.3f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
						{
							std::shared_ptr<Enemy_HellCaina> Enemy = Poolable<Enemy_HellCaina>::PopFromPool(Level, static_cast<int>(ActorOrder::Enemy));
							Enemy->GetPhysXComponent()->SetWorldPosition(EnemyPos[i]);
							Enemy->GetPhysXComponent()->SetWorldRotation(float4::UP * EnemyRot[i]);
							Enemy->GetTransform()->SetWorldPosition(EnemyPos[i]);
							Enemy->GetTransform()->SetWorldRotation(float4::UP * EnemyRot[i]);
							Enemy->GetPhysXComponent()->SetLinearVelocityZero();
							Enemy->GetPhysXComponent()->Off();
							Enemy->PushDeathCallback(std::bind(&EnemySpawner::DestroyMonster, this));
						});
				}
			}
	};
}
