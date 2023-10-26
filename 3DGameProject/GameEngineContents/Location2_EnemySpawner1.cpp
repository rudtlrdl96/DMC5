#include "PrecompileHeader.h"
#include "Location2_EnemySpawner1.h"
#include "Enemy_HellCaina.h"
#include "StageBaseLevel.h"
#include "BGMPlayer.h"
#include "NetworkManager.h"
#include "BasePlayerActor.h"
#include "ZoomEffect.h"
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

	// 클라이언트에서 처리될 BattleEndEvent 지정 (서버에서 패킷을 보내서 실행시킨다)
	NetworkManager::PushNetworkEvent(Net_EventType::RedWallOff_Location2_Spawner1, [=]
		{
			BGMPlayer::SetBattleEnd();
			ZoomEffect::GetZoomEffect()->SetSpeed(6.0f);
			ZoomEffect::GetZoomEffect()->EffectOn(1.5f);
			float BeforeTimeScale = GameEngineTime::GlobalTime.GetGlobalTimeScale();
			GameEngineTime::GlobalTime.SetGlobalTimeScale(0.5f);
			GetLevel()->TimeEvent.AddEvent(1.0f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					GameEngineTime::GlobalTime.SetGlobalTimeScale(BeforeTimeScale);
					ZoomEffect::GetZoomEffect()->SetSpeed(3.0f);
					ZoomEffect::GetZoomEffect()->EffectOff();
				});

			GetLevel()->TimeEvent.AddEvent(2.5f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					if (CutScenePosStart != float4::ZERO)
					{
						BasePlayerActor::GetMainPlayer()->SetCutScene(CutScenePosStart, CutScenePosEnd, CutSceneRotStart, CutSceneRotEnd, 5.0f);
					}
				});

			GetLevel()->TimeEvent.AddEvent(3.0f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					GetLevel()->DynamicThis<StageBaseLevel>()->RedSealWallOff();
				});

		});
	if (true == NetworkManager::IsServer())
	{
		BattleEndEvent = std::bind(NetworkManager::ExcuteNetworkEvent, Net_EventType::RedWallOff_Location2_Spawner1);
	}

	Event = [this]()
	{
		GameEngineLevel* Level = GetLevel();
		BasePlayerActor::GetMainPlayer()->SetCutScene({ 2842, 1011, -8469 }, { 3128, 1283, -10125 }, { 28, 41, 0 }, { 33, 28, 0 }, 6.0f);
		Level->TimeEvent.AddEvent(1.0f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
			{
				Level->DynamicThis<StageBaseLevel>()->RedSealWallOn();
			});
		BGMPlayer::SetBattleBGM();
		//NetworkObjectBase::PushReservedDestroyCallback(Net_ActorType::HellCaina, std::bind(&EnemySpawner::DestroyMonster, this));

		if (false == NetworkManager::IsClient())
		{
			MonsterAliveCount = 2;
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
			if (false == NetworkManager::IsClient())
			{
				MonsterAliveCount = 4;
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
