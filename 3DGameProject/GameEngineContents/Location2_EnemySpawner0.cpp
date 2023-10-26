#include "PrecompileHeader.h"
#include "Location2_EnemySpawner0.h"
#include "Enemy_Empusa.h"
#include "StageBaseLevel.h"
#include "BGMPlayer.h"
#include "NetworkManager.h"
#include "BasePlayerActor.h"
#include "ZoomEffect.h"
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

	// 클라이언트에서 처리될 BattleEndEvent 지정 (서버에서 패킷을 보내서 실행시킨다)
	NetworkManager::PushNetworkEvent(Net_EventType::RedWallOff_Location2_Spawner0, [=]
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
		BattleEndEvent = std::bind(NetworkManager::ExcuteNetworkEvent, Net_EventType::RedWallOff_Location2_Spawner0);
	}

	Event = [this]()
		{
			BasePlayerActor::GetMainPlayer()->SetCutScene({ 703, 722, 2392 }, { 461, 722, 2386 }, { 10, -165, 0 }, { 10, -179, 0 }, 5.0f);
			GameEngineLevel* Level = GetLevel();
			Level->TimeEvent.AddEvent(0.8f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
				{
					Level->DynamicThis<StageBaseLevel>()->RedSealWallOn();
				});
			BGMPlayer::SetBattleBGM();

			if (false == NetworkManager::IsClient())
			{
				MonsterAliveCount = 2;
				std::vector<float4> EnemyPos =
				{
					{ 494, 55, 1464 }, { -241, 82, 119 }
				};
				std::vector<float> EnemyRot = { -3, 12};
				for (size_t i = 0; i < 2; ++i)
				{
					Level->TimeEvent.AddEvent(i * 0.2f + 0.1f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
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
				Level->TimeEvent.AddEvent(i * 0.3f + 0.1f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
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
