#pragma once


class AnimLoadParameter
{
public:
	const std::string_view& Dir;
	std::shared_ptr<class GameEngineFBXRenderer> Renderer;
	std::vector<std::shared_ptr<GameEngineObject>> Objects = std::vector<std::shared_ptr<GameEngineObject>>();
	std::vector<std::function<void()>> CallBacks_void = std::vector<std::function<void()>>();
	std::vector<std::function<void(int)>> CallBacks_int = std::vector<std::function<void(int)>>();
	std::vector<std::function<void(float)>> CallBacks_float = std::vector<std::function<void(float)>>();
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
	EventData(EventType _Type) {
		Type = _Type;
	}
	EventType Type = EventType::None;
	int Index = 0;
	bool IsUpdate = true;
	float4 Position = float4::ZERO;
	float4 Rotation = float4::ZERO;
	float4 Scale = float4::ONE;
};

// Ό³Έν :
class AnimationEvent : public GameEngineSerializObject
{
	friend class AnimationToolWindow;
public:
	~AnimationEvent();
	AnimationEvent()
	{
		AnimationName = "";
		Speed = 10.0f;
		Events = std::map<size_t, std::vector<EventData>>();
	}

	void Write(GameEngineSerializer& _File) override
	{
		_File << AnimationName;
		_File << Speed;
		EventsWrite(_File);
	}

	void Read(GameEngineSerializer& _File) override
	{
		_File >> AnimationName;
		_File >> Speed;
		EventsRead(_File);
	}

	void EventsWrite(GameEngineSerializer& _File);
	void EventsRead(GameEngineSerializer& _File);


	static void LoadAll(const AnimLoadParameter& _Parameter);


protected:

private:
	std::string AnimationName;
	float Speed = 10.0f;
	std::map<size_t, std::vector<EventData>> Events;
};

