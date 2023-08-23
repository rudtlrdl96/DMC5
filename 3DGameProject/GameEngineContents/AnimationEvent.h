#pragma once


class AnimLoadParameter
{
public:
	GameEngineDirectory Dir;
	std::shared_ptr<class GameEngineFBXRenderer> Renderer;
	std::vector<std::shared_ptr<GameEngineObject>>& Objects;
	std::vector<std::function<void()>> CallBacks_void;
	std::vector<std::function<void(int)>> CallBacks_int;
	std::vector<std::function<void(float)>> CallBacks_float;
};

enum class EventType
{
	None,
	ObjectUpdate,
	CallBackVoid,
	CallBackInt,
	CallBackFloat,
};

class EventData
{
public:
	EventData() {}
	EventData(EventType _Type, int _Frame)
	{
		Type = _Type;
		Frame = _Frame;
	}
	EventType Type = EventType::None;
	int Frame = -1;
	int Index = -1;
};

class ObjectUpdateEvent : public EventData
{
public:
	ObjectUpdateEvent(int _Frame)
	{
		Type = EventType::ObjectUpdate;
		Frame = _Frame;
	}
	float4 Pos = float4::ZERO;
	float4 Rot = float4::ZERO;
	float4 Size = float4::ONE;
	bool OnOff = false;
};

class CallBackEvent : public EventData
{
public:
	CallBackEvent(int _Frame)
	{
		Type = EventType::CallBackVoid;
		Frame = _Frame;
	}
	int Value = -1;
};
// Ό³Έν :
class AnimationEvent
{
	friend class AnimationToolWindow;
public:
	// constrcuter destructer
	AnimationEvent();
	~AnimationEvent();

	// delete Function
	AnimationEvent(const AnimationEvent& _Other) = delete;
	AnimationEvent(AnimationEvent&& _Other) noexcept = delete;
	AnimationEvent& operator=(const AnimationEvent& _Other) = delete;
	AnimationEvent& operator=(AnimationEvent&& _Other) noexcept = delete;

	static void LoadAll(const AnimLoadParameter& _Parameter);



protected:

private:
	std::string AnimationName;
	float _Speed = 10.0f;
	std::vector<EventData*> Events;
	//std::map<size_t, ObjectUpdateEvent> Events_ObjUpdate;
	//std::map<size_t, std::function<void()>> Events_CallBack_void;
	//std::map<size_t, std::function<void(int)>> Events_CallBack_int;
	//std::map<size_t, std::function<void(float)>> Events_CallBack_float;
};

