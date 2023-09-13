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
	ObjectUpdate,		// 오브젝트의 On, Off. Transform의 변경 등 이벤트
	CallBackVoid, 
	CallBackInt,
	CallBackFloat,
	CallBackFloat4,
};

class EventData
{
public:
	EventData() {
		Position = float4::ZERO;
		Rotation = float4::ZERO;
		Scale = { 100, 100, 100 };
	}
	EventData(EventType _Type) {
		Position = float4::ZERO;
		Rotation = float4::ZERO;
		Scale = { 100, 100, 100 };
		Type = _Type;
	}
	EventType Type = EventType::None;	// 애니메이션 이벤트의 종류
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

// 설명 :
class AnimationEvent : public GameEngineSerializObject
{
	friend class AnimationToolWindow;
public:
	~AnimationEvent();
	AnimationEvent()
	{
		AnimationName = "";
		Speed = 1.0f;
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
	* Dir 경로에 있는 애니메이션 FBX 파일을 모두 로드하고 .animation 파일을 찾아 애니메이션 이벤트를 추가합니다
	*/
	static void LoadAll(const AnimLoadParameter& _Parameter);
	static void AnimationAllBuild(std::shared_ptr<GameEngineLevel> _Level, const std::string_view& Dir, const std::string& MeshName);

	void Clear()
	{
		AnimationName = " ";
		Speed = 1.0f;
		Events.clear();
	}
protected:

private:
	std::string AnimationName;
	float Speed = 1.0;
	bool Loop = true;
	std::map<size_t, std::vector<EventData>> Events;
};

