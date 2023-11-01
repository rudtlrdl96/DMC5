#include "PrecompileHeader.h"
#include "EnemySpawner.h"
#include "Enemy_Empusa.h"
#include "TestStageLevel.h"
#include "ZoomEffect.h"
#include "BasePlayerActor.h"
#include "NetworkManager.h"
EnemySpawner::EnemySpawner()
{

}

EnemySpawner::~EnemySpawner()
{

}

void EnemySpawner::Start()
{
	EventZone::Start();
}

void EnemySpawner::DestroyMonster()
{
	--MonsterAliveCount;

	if (0 < MonsterAliveCount)
		return;
	if (MonsterWaveCount < MonsterWave_Events.size())
	{
		MonsterWave_Events[MonsterWaveCount++]();
		return;
	}

	if (false == NetworkManager::IsClient())
	{
		if (nullptr != BattleEndEvent)
		{
			BattleEndEvent();
		}
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
				if (true == IsCutScene)
				{
					BasePlayerActor::GetMainPlayer()->SetCutScene(CutScenePosStart, CutScenePosEnd, CutSceneRotStart, CutSceneRotEnd, 5.0f);
				}
			});

		GetLevel()->TimeEvent.AddEvent(3.0f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				if (true == IsRedSeal)
				{
					GetLevel()->DynamicThis<StageBaseLevel>()->RedSealWallOff();
				}
			});
	}
	Death();
}