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

	//Ŭ���� ���ø����� State����
	template<typename StateType>
	void ChangeState()
	{
		static std::string Name;
		GetClassTypeName<StateType>(Name);
		ChangeState(Name);
	}
	//�̸����� State ����
	void ChangeState(const std::string_view& _Name);


	//������Ʈ
	void Update(float _DeltaTime);

	//������ State ������ �ֱ�
	std::shared_ptr<NetFsmStateBase> GetCurState();

	//FSM�� ���̺�Ÿ��
	inline float GetFsmTime() const
	{
		return FsmTimer;
	}

protected:
	//State�� ����� Fsm�ڷᱸ���� �����ϱ�
	template <typename StateType>
	std::shared_ptr<StateType> CreateState(const std::string_view& _StateName = "")
	{
		std::shared_ptr<StateType> NewState = std::make_shared<StateType>();
		std::string StateName = _StateName.data();

		//���ڷ� �̸��� ���������� ���� ��쿣 Ŭ���� ������ ����
		if (true == StateName.empty())
		{
			GetClassTypeName<StateType>(StateName);
		}

		//���� State �ʱ�ȭ
		InitState(NewState, StateName);
		return NewState;
	}


	template<typename StateType>
	void EraseState()
	{
		std::string StateName;
		GetClassTypeName<StateType>(StateName);
		EraseState(StateName);
	}

	void EraseState(const std::string_view& _StateName);



private:
	std::map<std::string, std::shared_ptr<NetFsmStateBase>> AllState;
	std::shared_ptr<NetFsmStateBase> CurState = nullptr;

	float FsmTimer = 0.f;

	//���� State�� �ʱ�ȭ
	void InitState(std::shared_ptr<NetFsmStateBase> _State, const std::string& _StateName);


	//���ø� ���ڷ� ���� Ŭ������ �̸� ����
	template<typename ClassType>
	void  GetClassTypeName(std::string& _Name)
	{
		const type_info& Info = typeid(ClassType);
		_Name = Info.name();

		//"class Ŭ������"�� �պκ� "class "�����
		_Name.replace(0, 6, "");
	}
};

