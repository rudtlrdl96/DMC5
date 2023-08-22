#pragma once

enum class EventType
{
	ObjectUpdate,
	CallBack,
};

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

	void AAA();

	static void LoadAll(const GameEngineDirectory& _Dir, std::shared_ptr<class GameEngineFBXRenderer> _Renderer, const std::vector<std::shared_ptr<GameEngineObject>>& Objects, std::vector<std::function<void()>> CallBacks);

protected:

private:
	std::string AnimationName;
	float _Speed = 0.0f;
	std::map<size_t, EventData> Events;
};

