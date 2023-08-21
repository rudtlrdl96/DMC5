#include "PrecompileHeader.h"
#include "GameEngineFSM.h"
#include <GameEngineBase/GameEngineString.h>
#include <GameEngineBase/GameEngineDebug.h>

GameEngineFSM::GameEngineFSM() 
{
}

GameEngineFSM::~GameEngineFSM() 
{
}


void GameEngineFSM::CreateState(const StateParameter& _StateFunction)
{
	if (nullptr != FindState(_StateFunction.StateValue))
	{
		MsgAssert("�̹� �����ϴ� �̸��� ������Ʈ�� �� ������� �߽��ϴ�." + std::to_string(_StateFunction.StateValue));
	}

	State& NewState = AllState[_StateFunction.StateValue];

	NewState.StateValue = _StateFunction.StateValue;
	NewState.End = _StateFunction.End;
	NewState.Start = _StateFunction.Start;
	NewState.Update = _StateFunction.Update;
}

void GameEngineFSM::ChangeFunction(int _State, std::function<void()> _Start,
	std::function<void(float _Delta)> _Update,
	std::function<void()> _End
) 
{
	if (nullptr == FindState(_State))
	{
		MsgAssert("�̹� �����ϴ� �̸��� ������Ʈ�� �� ������� �߽��ϴ�." + std::to_string(_State));
	}

	GameEngineFSM::State* Fsm = FindState(_State);

	Fsm->Start = _Start;
	Fsm->End = _End;
	Fsm->Update = _Update;

}

void GameEngineFSM::Update(float _DeltaTime)
{
	if (nullptr == CurState)
	{
		MsgAssert("���� ������Ʈ�� �������� �ʽ��ϴ�");
	}

	CurState->Update(_DeltaTime);
}

void GameEngineFSM::ChangeState(int _State) 
{
	if (nullptr != CurState)
	{
		if (nullptr != CurState->End)
		{
			CurState->End();
		}
	}

	CurState = FindState(_State);

	if (nullptr == CurState)
	{
		MsgAssert("�������� �ʴ� ������Ʈ�� �̵��Ϸ��� �߽��ϴ�" + std::to_string(_State));
	}

	CurState->Start();
}

GameEngineFSM::State* GameEngineFSM::FindState(int _State)
{

	if (AllState.end() == AllState.find(_State))
	{
		return nullptr;
	}

	return &AllState[_State];
}