#pragma once


class AnimLoadParameter
{
public:
	const std::string_view& Dir;
	std::shared_ptr<class GameEngineFBXRenderer> Renderer;
	std::vector<GameEngineObject*> Objects = std::vector<GameEngineObject*>();
	std::vector<std::function<void()>> CallBacks_void = std::vector<std::function<void()>>();
	std::vector<std::function<void(int)>> CallBacks_int = std::vector<std::function<void(int)>>();
	std::vector<std::function<void(float)>> CallBacks_float = std::vector<std::function<void(float)>>();
	std::vector<std::function<void(float4)>> CallBacks_float4 = std::vector<std::function<void(float4)>>();
};

enum class EventType
{
	None,
	ObjectUpdate,		// ������Ʈ�� On, Off. Transform�� ���� �� �̺�Ʈ
	CallBackVoid,
	CallBackInt,
	CallBackFloat,
	CallBackFloat4,
};

class EventData
{
public:
	EventData() {}
	EventData(EventType _Type) {
		Type = _Type;
	}
	EventType Type = EventType::None;	// �ִϸ��̼� �̺�Ʈ�� ����
	int Index = 0;
	bool IsUpdate = true;

	union
	{
		struct
		{
			float4 Position;
			float4 Rotation;
			float4 Scale;
		};
		int IntValue;
		float FloatValue;
	};
};

// ���� :
class AnimationEvent : public GameEngineSerializObject
{
	friend class AnimationToolWindow;
public:
	~AnimationEvent();
	AnimationEvent()
	{
		AnimationName = "";
		Speed = 6.0f;
		Events = std::map<size_t, std::vector<EventData>>();
	}

	void Write(GameEngineSerializer& _File) override
	{
		_File << AnimationName;
		_File << Speed;
		_File << Loop;
		EventsWrite(_File);
	}

	void Read(GameEngineSerializer& _File) override
	{
		_File >> AnimationName;
		_File >> Speed;
		_File >> Loop;
		EventsRead(_File);
	}

	void EventsWrite(GameEngineSerializer& _File);
	void EventsRead(GameEngineSerializer& _File);
	
	/*
	* Dir ��ο� �ִ� �ִϸ��̼� FBX ������ ��� �ε��ϰ� .animation ������ ã�� �ִϸ��̼� �̺�Ʈ�� �߰��մϴ�
	*/
	static void LoadAll(const AnimLoadParameter& _Parameter);

	void Clear()
	{
		AnimationName = " ";
		Speed = 6.0f;
		Events.clear();
	}
protected:

private:
	std::string AnimationName;
	float Speed = 1.0;
	bool Loop = true;
	std::map<size_t, std::vector<EventData>> Events;
};

