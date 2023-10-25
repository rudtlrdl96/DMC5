#include "PrecompileHeader.h"
#include "Location2_EnemySpawner0.h"
#include "Enemy_Empusa.h"
#include "StageBaseLevel.h"
#include "BGMPlayer.h"
#include "NetworkManager.h"
#include "BasePlayerActor.h"
Location2_EnemySpawner0::Location2_EnemySpawner0()
{
	CutScenePosStart = { 2245, 856, -300 };
	CutScenePosEnd = { 1789, 856, -773 };

	CutSceneRotStart = { 15, -56, 0 };
	CutSceneRotEnd = { 15, -30, 0 };
}

Location2_EnemySpawner0::~Location2_EnemySpawner0()
{

}

void Location2_EnemySpawner0::Start()
{
	EnemySpawner::Start();

	Event = [this]()
		{
			BasePlayerActor::GetMainPlayer()->SetCutScene({ 703, 722, 2392 }, { 461, 722, 2386 }, { 10, -165, 0 }, { 10, -179, 0 }, 5.0f);
			GameEngineLevel* Level = GetLevel();
			Level->TimeEvent.AddEvent(0.8f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
				{
					Level->DynamicThis<StageBaseLevel>()->RedSealWallOn();
				});
			BGMPlayer::SetBattleBGM();
			MonsterAliveCount = 2;

			if (false == NetworkManager::IsClient())
			{
				std::vector<float4> EnemyPos =
				{
					{ 494, 55, 1464 }, { -241, 82, 119 }
				};
				std::vector<float> EnemyRot = { -3, 12};
				for (size_t i = 0; i < 2; ++i)
				{
					Level->TimeEvent.AddEvent(i * 0.2f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
					{
							std::shared_ptr<Enemy_Empusa> Empusa = Poolable<Enemy_Empusa>::PopFromPool(Level, static_cast<int>(ActorOrder::Enemy));
							Empusa->GetPhysXComponent()->SetWorldPosition(EnemyPos[i]);
							Empusa->GetPhysXComponent()->SetWorldRotation(float4::UP * EnemyRot[i]);
							Empusa->GetTransform()->SetWorldPosition(EnemyPos[i]);
							Empusa->GetTransform()->SetWorldRotation(float4::UP * EnemyRot[i]);
							Empusa->GetPhysXComponent()->SetLinearVelocityZero();
							Empusa->GetPhysXComponent()->Off();
							Empusa->PushDeathCallback(std::bind(&EnemySpawner::DestroyMonster, this));
					});
					
				}
			}
			else
			{
				for (size_t i = 0; i < MonsterAliveCount; ++i)
				{
					NetworkObjectBase::PushReservedDestroyCallback(Net_ActorType::Empusa, std::bind(&EnemySpawner::DestroyMonster, this));
				}
			}
		};

	MonsterWave_Events.resize(1);
	MonsterWave_Events[0] = [this]()
	{
		MonsterAliveCount = 5;
		if (false == NetworkManager::IsClient())
		{
			GameEngineLevel* Level = GetLevel();
			std::vector<float4> EnemyPos =
			{
				{ -1469, 55, 1600 }, { 274, 82, 2461 }, { 888, 82, 1813 }, { -3, 82, 91 }, { -965, 82, 2174 }
			};
			std::vector<float> EnemyRot = { 120, -167, -127, 12, 120};

			for (size_t i = 0; i < 5; ++i)
			{
				Level->TimeEvent.AddEvent(i * 0.3f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
					{
						std::shared_ptr<Enemy_Empusa> Empusa = Poolable<Enemy_Empusa>::PopFromPool(Level, static_cast<int>(ActorOrder::Enemy));
						Empusa->GetPhysXComponent()->SetWorldPosition(EnemyPos[i]);
						Empusa->GetPhysXComponent()->SetWorldRotation(float4::UP * EnemyRot[i]);
						Empusa->GetTransform()->SetWorldPosition(EnemyPos[i]);
						Empusa->GetTransform()->SetWorldRotation(float4::UP * EnemyRot[i]);
						Empusa->GetPhysXComponent()->SetLinearVelocityZero();
						Empusa->GetPhysXComponent()->Off();
						Empusa->PushDeathCallback(std::bind(&EnemySpawner::DestroyMonster, this));
					});
			}
		}
	};
}
