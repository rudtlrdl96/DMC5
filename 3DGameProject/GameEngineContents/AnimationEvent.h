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
	ObjectUpdate,		// ������Ʈ�� On, Off. Transform�� ���� �� �̺�Ʈ
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
	EventType Type = EventType::None;	// �ִϸ��̼� �̺�Ʈ�� ����
	int Index = 0;
	bool IsUpdate = true;
	float4 Position = float4::ZERO;		// CallBack �Լ��� ���ڰ��� Position�� x������ ����
	float4 Rotation = float4::ZERO;
	float4 Scale = float4::ONE;
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
	
	/*
	* Dir ��ο� �ִ� �ִϸ��̼� FBX ������ ��� �ε��ϰ� .animation ������ ã�� �ִϸ��̼� �̺�Ʈ�� �߰��մϴ�
	*/
	static void LoadAll(const AnimLoadParameter& _Parameter);


protected:

private:
	std::string AnimationName;
	float Speed = 10.0f;
	std::map<size_t, std::vector<EventData>> Events;
};

