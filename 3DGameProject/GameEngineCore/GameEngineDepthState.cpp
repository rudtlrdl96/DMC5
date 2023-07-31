#include "PrecompileHeader.h"
#include "GameEngineDepthState.h"

GameEngineDepthState::GameEngineDepthState() 
{
}

GameEngineDepthState::~GameEngineDepthState() 
{
	if (nullptr != State)
	{
		State->Release();
		State = nullptr;
	}
}


void GameEngineDepthState::Setting() 
{
	if (nullptr == State)
	{
		MsgAssert("깊이버퍼 스테이트가 만들어지지 않았습니다.");
	}

	// 내가 만든 팩터를 넣어줄수가 있는데.
	GameEngineDevice::GetContext()->OMSetDepthStencilState(State, 0);

}

void GameEngineDepthState::ResCreate(const D3D11_DEPTH_STENCIL_DESC& _Value)
{
	Desc = _Value;

	if (S_OK != GameEngineDevice::GetDevice()->CreateDepthStencilState(&Desc, &State))
	{
		MsgAssert("깊이 버퍼 스테이트 생성에 실패했습니다.");
	}

}
