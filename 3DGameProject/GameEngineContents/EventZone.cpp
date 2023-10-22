#include "PrecompileHeader.h"
#include "EventZone.h"

#include <GameEngineCore/GameEngineCollision.h>
#include "BGMPlayer.h"
#include "StageBaseLevel.h"

EventZone::EventZone()
{

}

EventZone::~EventZone()
{

}

void EventZone::SetEvent(std::function<void()> _Event)
{
	Event = _Event;
}

void EventZone::Start()
{
	FieldMapObject::Start();
	TriggerZone = CreateComponent<GameEngineCollision>(CollisionOrder::EventZone);
	TriggerZone->SetColType(ColType::OBBBOX3D);
}

void EventZone::Update(float _DeltaTime)
{
	if (TriggerZone == nullptr)
	{
		MsgAssert("EventZone : TriggerZone 이 nullptr입니다")
	}

	if (Event == nullptr)
	{
		MsgAssert("EventZone : Event가 nullptr입니다")
	}

	if (IsEventStart)
	{
		return;
	}

	if (TriggerZone->Collision(CollisionOrder::Player, ColType::OBBBOX3D, ColType::OBBBOX3D))
	{
		Event();
		IsEventStart = true;
	}
}

void EventZone::DestroyMonster()
{
	--MonsterAliveCount;

	if (0 < MonsterAliveCount)
		return;

	BGMPlayer::SetBattleEnd();
	GetLevel()->DynamicThis<StageBaseLevel>()->RedSealWallOff();
	Death();
}
