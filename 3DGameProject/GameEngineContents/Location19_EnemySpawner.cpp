#include "PrecompileHeader.h"
#include "Location19_EnemySpawner.h"
#include "Location2_EnemySpawner0.h"
#include "Enemy_Empusa.h"
#include "StageBaseLevel.h"
#include "BGMPlayer.h"
#include "NetworkManager.h"
#include "BasePlayerActor.h"
#include "ZoomEffect.h"
#include "Enemy_HellCaina.h"

Location19_EnemySpawner::Location19_EnemySpawner() 
{
}

Location19_EnemySpawner::~Location19_EnemySpawner() 
{
}

void Location19_EnemySpawner::Start()
{
	EnemySpawner::Start();

	// 클라이언트에서 처리될 BattleEndEvent 지정 (서버에서 패킷을 보내서 실행시킨다)
	NetworkManager::PushNetworkEvent(Net_EventType::Location19_End, [=]
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

			GetLevel()->TimeEvent.AddEvent(3.0f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
				});

		});
	if (true == NetworkManager::IsServer())
	{
		BattleEndEvent = std::bind(NetworkManager::ExcuteNetworkEvent, Net_EventType::Location19_End);
	}

	Event = [this]()
		{
			GameEngineLevel* Level = GetLevel();
			BasePlayerActor::GetMainPlayer()->SetCutScene({ -2307, 1085, 1618 }, { -1250, 1085, 2358 }, { 30, 132, 0 }, { 20, 160, 0 }, 5.0f);
			BGMPlayer::SetBattleBGM();

			if (false == NetworkManager::IsClient())
			{
				MonsterAliveCount = 4;
				std::vector<float4> EnemyPos =
				{
					{ 3652 , 86, -9166 }, { 4289 , 86, -8878 }, { 3032 , 86, -9015 }, { 3032 , 86, -9015 }
				};
				std::vector<float> EnemyRot = { 0, -16, 35, 0 };

				for (size_t i = 0; i < 4; ++i)
				{
					/*Level->TimeEvent.AddEvent(i * 0.3f + 2.0f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
						{
							std::shared_ptr<Enemy_HellCaina> Enemy = Poolable<Enemy_HellCaina>::PopFromPool(Level, static_cast<int>(ActorOrder::Enemy));
							Enemy->GetPhysXComponent()->SetWorldPosition(EnemyPos[i]);
							Enemy->GetPhysXComponent()->SetWorldRotation(float4::UP * EnemyRot[i]);
							Enemy->GetTransform()->SetWorldPosition(EnemyPos[i]);
							Enemy->GetTransform()->SetWorldRotation(float4::UP * EnemyRot[i]);
							Enemy->GetPhysXComponent()->SetLinearVelocityZero();
							Enemy->GetPhysXComponent()->Off();
							Enemy->PushDeathCallback(std::bind(&EnemySpawner::DestroyMonster, this));
						});*/
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

				//for (size_t i = 0; i < 3; ++i)
				//{
				//	Level->TimeEvent.AddEvent(i * 0.3f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
				//		{
				//			std::shared_ptr<Enemy_HellCaina> Enemy = Poolable<Enemy_HellCaina>::PopFromPool(Level, static_cast<int>(ActorOrder::Enemy));
				//			Enemy->GetPhysXComponent()->SetWorldPosition(EnemyPos[i]);
				//			Enemy->GetPhysXComponent()->SetWorldRotation(float4::UP * EnemyRot[i]);
				//			Enemy->GetTransform()->SetWorldPosition(EnemyPos[i]);
				//			Enemy->GetTransform()->SetWorldRotation(float4::UP * EnemyRot[i]);
				//			Enemy->GetPhysXComponent()->SetLinearVelocityZero();
				//			Enemy->GetPhysXComponent()->Off();
				//			Enemy->PushDeathCallback(std::bind(&EnemySpawner::DestroyMonster, this));
				//		});
				//}
			}
		};
}
