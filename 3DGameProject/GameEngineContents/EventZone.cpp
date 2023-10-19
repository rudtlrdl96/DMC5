#include "PrecompileHeader.h"
#include "EventZone.h"

#include <GameEngineCore/GameEngineCollision.h>

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

//void EventZone::SetTriggerZoneTransform(const float4& _Position, const float4& _Scale, const float4& _Rotation)
//{
//	if (TriggerZone == nullptr)
//	{
//		MsgAssert("TriggerZone가 nullptr인데 Transform을 변경하려 했습니다")
//	}
//
//	TriggerZone->GetTransform()->SetLocalPosition(_Position);
//	TriggerZone->GetTransform()->SetLocalScale(_Scale);
//	TriggerZone->GetTransform()->SetLocalRotation(_Rotation);
//}
