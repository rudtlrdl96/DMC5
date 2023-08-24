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
	ObjectUpdate,		// 오브젝트의 On, Off. Transform의 변경 등 이벤트
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
	EventType Type = EventType::None;	// 애니메이션 이벤트의 종류
	int Index = 0;
	bool IsUpdate = true;
	float4 Position = float4::ZERO;		// CallBack 함수의 인자값은 Position의 x값으로 적용
	float4 Rotation = float4::ZERO;
	float4 Scale = float4::ONE;
};

// 설명 :
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
	* Dir 경로에 있는 애니메이션 FBX 파일을 모두 로드하고 .animation 파일을 찾아 애니메이션 이벤트를 추가합니다
	*/
	static void LoadAll(const AnimLoadParameter& _Parameter);


protected:

private:
	std::string AnimationName;
	float Speed = 10.0f;
	std::map<size_t, std::vector<EventData>> Events;
};

