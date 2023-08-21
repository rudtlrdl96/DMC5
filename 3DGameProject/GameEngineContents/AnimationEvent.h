#pragma once

class EventData
{

};

// Ό³Έν :
class AnimationEvent
{
public:
	// constrcuter destructer
	AnimationEvent();
	~AnimationEvent();

	// delete Function
	AnimationEvent(const AnimationEvent& _Other) = delete;
	AnimationEvent(AnimationEvent&& _Other) noexcept = delete;
	AnimationEvent& operator=(const AnimationEvent& _Other) = delete;
	AnimationEvent& operator=(AnimationEvent&& _Other) noexcept = delete;

protected:

private:
	std::string AnimationName;
	std::map<size_t, EventData> Events;
};

