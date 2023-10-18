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
	TriggerZone = CreateComponent<GameEngineCollision>(CollisionOrder::EventZone);
	TriggerZone->SetColType(ColType::OBBBOX3D);
}

void EventZone::Update(float _DeltaTime)
{
	if (TriggerZone == nullptr)
	{
		MsgAssert("EventZone : TriggerZone �� nullptr�Դϴ�")
	}

	if (Event == nullptr)
	{
		MsgAssert("EventZone : Event�� nullptr�Դϴ�")
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
//		MsgAssert("TriggerZone�� nullptr�ε� Transform�� �����Ϸ� �߽��ϴ�")
//	}
//
//	TriggerZone->GetTransform()->SetLocalPosition(_Position);
//	TriggerZone->GetTransform()->SetLocalScale(_Scale);
//	TriggerZone->GetTransform()->SetLocalRotation(_Rotation);
//}
