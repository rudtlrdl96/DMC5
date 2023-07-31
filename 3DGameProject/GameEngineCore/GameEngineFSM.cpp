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
	if (nullptr != FindState(_StateFunction.Name))
	{
		MsgAssert("�̹� �����ϴ� �̸��� ������Ʈ�� �� ������� �߽��ϴ�." + std::string(_StateFunction.Name));
	}

	std::string UpperName = GameEngineString::ToUpper(_StateFunction.Name);

	State& NewState = AllState[UpperName];

	NewState.Name = _StateFunction.Name;
	NewState.End = _StateFunction.End;
	NewState.Start = _StateFunction.Start;
	NewState.Update = _StateFunction.Update;
}

void GameEngineFSM::ChangeFunction(const std::string_view& _Name, std::function<void()> _Start,
	std::function<void(float _Delta)> _Update,
	std::function<void()> _End
) 
{
	if (nullptr == FindState(_Name))
	{
		MsgAssert("�̹� �����ϴ� �̸��� ������Ʈ�� �� ������� �߽��ϴ�." + std::string(_Name));
	}

	GameEngineFSM::State* Fsm = FindState(_Name);

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

void GameEngineFSM::ChangeState(const std::string_view& _State) 
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
		MsgAssert("�������� �ʴ� ������Ʈ�� �̵��Ϸ��� �߽��ϴ�" + std::string(_State));
	}

	CurState->Start();
}

GameEngineFSM::State* GameEngineFSM::FindState(const std::string_view& _Name)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);

	if (AllState.end() == AllState.find(UpperName))
	{
		return nullptr;
	}

	return &AllState[UpperName];
}