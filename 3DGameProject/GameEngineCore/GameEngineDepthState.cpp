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
		MsgAssert("���̹��� ������Ʈ�� ��������� �ʾҽ��ϴ�.");
	}

	// ���� ���� ���͸� �־��ټ��� �ִµ�.
	GameEngineDevice::GetContext()->OMSetDepthStencilState(State, 0);

}

void GameEngineDepthState::ResCreate(const D3D11_DEPTH_STENCIL_DESC& _Value)
{
	Desc = _Value;

	if (S_OK != GameEngineDevice::GetDevice()->CreateDepthStencilState(&Desc, &State))
	{
		MsgAssert("���� ���� ������Ʈ ������ �����߽��ϴ�.");
	}

}
