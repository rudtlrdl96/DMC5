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
	EventData(EventType _Type) {
		Type = _Type;
	}
	EventType Type = EventType::None;
	int Index = 0;
	bool IsUpdate = true;
	float4 Position = float4::ZERO;
	float4 Rotation = float4::ZERO;
	float4 Scale = float4::ONE;

	void Write(GameEngineSerializer& _File)
	{
		_File << Type;
		_File << Index;
		_File << IsUpdate;
		_File << Position.x;
		_File << Position.y;
		_File << Position.z;
		_File << Rotation.x;
		_File << Rotation.y;
		_File << Rotation.z;
		_File << Scale.x;
		_File << Scale.y;
		_File << Scale.z;
	}

	void Read(GameEngineSerializer& _File)
	{
		_File >> Type;
		_File >> Index;
		_File >> IsUpdate;
		_File >> Position.x;
		_File >> Position.y;
		_File >> Position.z;
		_File >> Rotation.x;
		_File >> Rotation.y;
		_File >> Rotation.z;
		_File >> Scale.x;
		_File >> Scale.y;
		_File >> Scale.z;
	}
};

// Ό³Έν :
class AnimationEvent : public GameEngineSerializObject
{
	friend class AnimationToolWindow;
public:
	~AnimationEvent();

	AnimationEvent(const AnimationEvent& _Other) = delete;
	AnimationEvent(AnimationEvent&& _Other) noexcept = delete;
	AnimationEvent& operator=(const AnimationEvent& _Other) = delete;
	AnimationEvent& operator=(AnimationEvent&& _Other) noexcept = delete;

	AnimationEvent()
	{
		AnimationName = "";
		Speed = 10.0f;
		Events = std::map<size_t, std::vector<EventData>>();
	}

	void Write(GameEngineSerializer& _File) override
	{
		//_File << AnimationName;
		//_File << Speed;
		EventsWrite(_File);
		//_File << Events;
	}

	void Read(GameEngineSerializer& _File) override
	{
		//_File >> AnimationName;
		//_File >> Speed;
		EventsRead(_File);
		//_File >> Events;
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

