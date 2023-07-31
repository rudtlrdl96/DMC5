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
		MsgAssert("이미 존재하는 이름의 스테이트를 또 만들려고 했습니다." + std::string(_StateFunction.Name));
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
		MsgAssert("이미 존재하는 이름의 스테이트를 또 만들려고 했습니다." + std::string(_Name));
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
		MsgAssert("현재 스테이트가 존재하지 않습니다");
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
		MsgAssert("존재하지 않는 스테이트로 이동하려고 했습니다" + std::string(_State));
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