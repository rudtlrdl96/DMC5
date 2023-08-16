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
		std::string ErrorDesc = _StateName + "이름을 가진 State가 이미 존재합니다";
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
	//이전 상태값
	std::shared_ptr<NetFsmStateBase>  PrevState = CurState;

	//다음 상태값
	const std::string NextStateName = _Name.data();
	if (false == AllState.contains(NextStateName))
	{
		MsgAssert("바꾸려는 배틀 State를 만들어준 적이 없습니다");
		return;
	}
	std::shared_ptr<NetFsmStateBase>  NextState = AllState[NextStateName];

	//현재 상태값 교체
	CurState = NextState;


	//이전 상태값이 존재했다면 Exit호출
	if (nullptr != PrevState)
	{
		PrevState->ExitState();
	}

	//다음 상태값에 Enter호출
	NextState->EnterState();
}



void NetFsmBase::Update(float _DeltaTime)
{
	if (nullptr == CurState)
	{
		MsgAssert("현재 FSM의 상태를 설정해주지 않았습니다");
		return;
	}

	FsmTimer += _DeltaTime;
	CurState->Update(_DeltaTime);
}




std::shared_ptr<NetFsmStateBase> NetFsmBase::GetCurState()
{
	if (nullptr == CurState)
	{
		MsgAssert("현재 상태값이 nullptr입니다");
		return nullptr;
	}

	return CurState;
}