#pragma once
#include "FieldMapObject.h"
enum class EventZoneOption
{
	AllPlayer,
	OnePlayer
};


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
	
	void SetEvent(std::function<void()> _Event);
	static int ServerPlayerCount;
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

protected:
	std::shared_ptr<GameEngineCollision> TriggerZone = nullptr;
	std::function<void()> Event = nullptr;
	bool IsEventStart = false;
	EventZoneOption Option = EventZoneOption::AllPlayer;

private:
};

