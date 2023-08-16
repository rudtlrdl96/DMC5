#include "PrecompileHeader.h"
#include <typeinfo>

#include "NetFsmBase.h"
#include "NetFsmStateBase.h"

NetFsmBase::NetFsmBase()
{

}

NetFsmBase::~NetFsmBase()
{

}


void NetFsmBase::InitState(std::shared_ptr<NetFsmStateBase> _State, const std::string& _StateName)
{
	if (true == AllState.contains(_StateName))
	{
		std::string ErrorDesc = _StateName + "�̸��� ���� State�� �̹� �����մϴ�";
		MsgAssert(ErrorDesc);
		return;
	}

	_State->FsmPtr = this;
	_State->Name = _StateName;
	AllState[_StateName] = _State;

	_State->Start();
}




void NetFsmBase::ChangeState(const std::string_view& _Name)
{
	//���� ���°�
	std::shared_ptr<NetFsmStateBase>  PrevState = CurState;

	//���� ���°�
	const std::string NextStateName = _Name.data();
	if (false == AllState.contains(NextStateName))
	{
		MsgAssert("�ٲٷ��� ��Ʋ State�� ������� ���� �����ϴ�");
		return;
	}
	std::shared_ptr<NetFsmStateBase>  NextState = AllState[NextStateName];

	//���� ���°� ��ü
	CurState = NextState;


	//���� ���°��� �����ߴٸ� Exitȣ��
	if (nullptr != PrevState)
	{
		PrevState->ExitState();
	}

	//���� ���°��� Enterȣ��
	NextState->EnterState();
}



void NetFsmBase::Update(float _DeltaTime)
{
	if (nullptr == CurState)
	{
		MsgAssert("���� FSM�� ���¸� ���������� �ʾҽ��ϴ�");
		return;
	}

	FsmTimer += _DeltaTime;
	CurState->Update(_DeltaTime);
}




std::shared_ptr<NetFsmStateBase> NetFsmBase::GetCurState()
{
	if (nullptr == CurState)
	{
		MsgAssert("���� ���°��� nullptr�Դϴ�");
		return nullptr;
	}

	return CurState;
}