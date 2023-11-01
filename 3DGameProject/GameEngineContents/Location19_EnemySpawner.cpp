#include "PrecompileHeader.h"
#include "Location19_EnemySpawner.h"
#include "Location2_EnemySpawner0.h"
#include "StageBaseLevel.h"
#include "BGMPlayer.h"
#include "NetworkManager.h"
#include "BasePlayerActor.h"
#include "ZoomEffect.h"
#include "Enemy_HellAntenora.h"
#include "Enemy_ScudoAngelo.h"
#include "Enemy_ProtoAngelo.h"
#include "Location19_Level.h"
Location19_EnemySpawner::Location19_EnemySpawner()
{
}

Location19_EnemySpawner::~Location19_EnemySpawner()
{
}

void Location19_EnemySpawner::Start()
{
	EnemySpawner::Start();
	IsRedSeal = false;
	IsCutScene = false;
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

			Location19_Level* Level = dynamic_cast<Location19_Level*>(GetLevel());
			if (nullptr != Level)
			{
				Level->CreatePotal();
			}
		});

	if (false == NetworkManager::IsClient())
	{
		BattleEndEvent = [this]
			{
				NetworkManager::ExcuteNetworkEvent(Net_EventType::Location19_End);
				Location19_Level* Level = dynamic_cast<Location19_Level*>(GetLevel());
				if (nullptr != Level)
				{
					Level->CreatePotal();
				}
			};
	}

	Event = [this]()
		{
			GameEngineSound::Play("RedSeal_0.wav");
			GameEngineLevel* Level = GetLevel();
			BasePlayerActor::GetMainPlayer()->SetCutScene({ -2307, 1085, 1618 }, { -1250, 1085, 2358 }, { 30, 132, 0 }, { 20, 160, 0 }, 5.0f);
			BGMPlayer::SetBattleBGM();

			if (false == NetworkManager::IsClient())
			{
				MonsterAliveCount = 4;
				std::vector<float4> EnemyPos =
				{
					{ -2000 , 97, 438 }, { -145 , 105, 1750 }, { 1524 , 86, 386 }, { 549 , 70, -1810 }
				};
				std::vector<float> EnemyRot = { 105, -179, -95, -28 };

				for (size_t i = 0; i < 4; ++i)
				{
					Level->TimeEvent.AddEvent(i * 0.3f + 1.3f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
						{
							std::shared_ptr<Enemy_HellAntenora> Enemy = Poolable<Enemy_HellAntenora>::PopFromPool(Level, static_cast<int>(ActorOrder::Enemy));
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
			GameEngineSound::Play("RedSeal_0.wav");
			if (false == NetworkManager::IsClient())
			{
				MonsterAliveCount = 5;
				GameEngineLevel* Level = GetLevel();
				std::vector<float4> EnemyPos =
				{
					{ 1330 , 90, 483 }, { 1310 , 71, -194 }, { 943 , 60, 481 }, { 1616 , 106, 702 }, { 1880 , 77, -158 }
				};
				std::vector<float> EnemyRot = { -115, -91, -118, -118, -87 };

				for (size_t i = 0; i < 5; ++i)
				{
					Level->TimeEvent.AddEvent(i * 0.3f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
						{
							std::shared_ptr<BaseEnemyActor> Enemy;
							if (i == 0)
							{
								Enemy = Poolable<Enemy_ProtoAngelo>::PopFromPool(Level, static_cast<int>(ActorOrder::Enemy));
							}
							else
							{
								Enemy = Poolable<Enemy_ScudoAngelo>::PopFromPool(Level, static_cast<int>(ActorOrder::Enemy));
							}
							Enemy->GetPhysXComponent()->Off();
							Enemy->GetPhysXComponent()->SetLinearVelocityZero();
							Enemy->GetPhysXComponent()->SetWorldPosition(EnemyPos[i]);
							Enemy->GetPhysXComponent()->SetWorldRotation(float4::UP * EnemyRot[i]);
							Enemy->GetTransform()->SetWorldPosition(EnemyPos[i]);
							Enemy->GetTransform()->SetWorldRotation(float4::UP * EnemyRot[i]);
							Enemy->GetPhysXComponent()->SetLinearVelocityZero();
							Enemy->PushDeathCallback(std::bind(&EnemySpawner::DestroyMonster, this));
						});
				}
			}
		};
}
