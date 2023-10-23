#include "PrecompileHeader.h"
#include "EventZone.h"

#include <GameEngineCore/GameEngineCollision.h>
#include "BGMPlayer.h"
#include "StageBaseLevel.h"

#include "BasePlayerActor.h"

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

	size_t PlayerCount = BasePlayerActor::GetPlayers().size();

	if (PlayerCount == 0)
	{
		return;
	}

	std::vector<std::shared_ptr<GameEngineCollision>> Temp;

	TriggerZone->CollisionAll(CollisionOrder::Player, Temp, ColType::OBBBOX3D, ColType::OBBBOX3D);

	if (Temp.size() == PlayerCount)
	{
		Event();
		IsEventStart = true;
	}
}


