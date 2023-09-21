#pragma once
#include "FieldMapObject.h"

class EventZone : public FieldMapObject
{
public:
	// construtor destructor
	EventZone();
	~EventZone();

	// delete Function
	EventZone(const EventZone& _Other) = delete;
	EventZone(EventZone&& _Other) noexcept = delete;
	EventZone& operator=(const EventZone& _Other) = delete;
	EventZone& operator=(EventZone&& _Other) noexcept = delete;
	
	//void SetTriggerZoneTransform(const float4& _Position, const float4& _Scale, const float4& _Rotation);

protected:
	void Update(float _DeltaTime) override;

protected:
	std::shared_ptr<GameEngineCollision> TriggerZone = nullptr;
	std::function<void()> Event = nullptr;
	bool IsEventStart = false;


private:
};

