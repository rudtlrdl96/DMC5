#pragma once

class NetFsmStateBase;

class NetFsmBase
{
public:
	NetFsmBase();
	virtual ~NetFsmBase() = 0;

	NetFsmBase(const NetFsmBase& _Other) = delete;
	NetFsmBase(NetFsmBase&& _Other) noexcept = delete;
	NetFsmBase& operator=(const NetFsmBase& _Other) = delete;
	NetFsmBase& operator=(const NetFsmBase&& _Other) noexcept = delete;

	//클래스 템플릿으로 State변경
	template<typename StateType>
	void ChangeState()
	{
		static std::string Name;
		GetClassTypeName<StateType>(Name);
		ChangeState(Name);
	}
	//이름으로 State 변경
	void ChangeState(const std::string_view& _Name);


	//업데이트
	void Update(float _DeltaTime);

	//현재의 State 포인터 주기
	std::shared_ptr<NetFsmStateBase> GetCurState();

	//FSM의 라이브타임
	inline float GetFsmTime() const
	{
		return FsmTimer;
	}

protected:
	//State를 만들고 Fsm자료구조에 보관하기
	template <typename StateType>
	std::shared_ptr<StateType> CreateState(const std::string_view& _StateName = "")
	{
		std::shared_ptr<StateType> NewState = std::make_shared<StateType>();
		std::string StateName = _StateName.data();

		//인자로 이름을 지정해주지 않은 경우엔 클래스 명으로 지정
		if (true == StateName.empty())
		{
			GetClassTypeName<StateType>(StateName);
		}

		//만든 State 초기화
		InitState(NewState, StateName);
		return NewState;
	}



private:
	std::map<std::string, std::shared_ptr<NetFsmStateBase>> AllState;
	std::shared_ptr<NetFsmStateBase> CurState = nullptr;

	float FsmTimer = 0.f;

	//만든 State를 초기화
	void InitState(std::shared_ptr<NetFsmStateBase> _State, const std::string& _StateName);


	//템플릿 인자로 받은 클래스의 이름 추출
	template<typename ClassType>
	void  GetClassTypeName(std::string& _Name)
	{
		const type_info& Info = typeid(ClassType);
		_Name = Info.name();

		//"class 클래스명"의 앞부분 "class "지우기
		_Name.replace(0, 6, "");
	}
};

