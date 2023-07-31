#include "PrecompileHeader.h"
#include "GameEngineBlend.h"

GameEngineBlend::GameEngineBlend() 
{
}

GameEngineBlend::~GameEngineBlend() 
{
	if (nullptr != State)
	{
		State->Release();
		State = nullptr;
	}
}

void GameEngineBlend::Setting() 
{
	if (nullptr == State)
	{
		MsgAssert("�������� ���� �����Ͷ����� ������Ʈ �Դϴ�.");
	}

	if (Desc.RenderTarget[0].SrcBlend == D3D11_BLEND_BLEND_FACTOR)
	{
		MsgAssert("���� ó�� ���� �ʴ� ���� ����Դϴ� ���� ���� �Է±���� ����� �ּ���.");
	}
	

	// ���� ���� ���͸� �־��ټ��� �ִµ�.
	GameEngineDevice::GetContext()->OMSetBlendState(State, nullptr, Mask);

}

void GameEngineBlend::ResCreate(const D3D11_BLEND_DESC& _Value)
{
	Desc = _Value;

	if (S_OK != GameEngineDevice::GetDevice()->CreateBlendState(&Desc, &State))
	{
		MsgAssert("���� ������Ʈ ������ �����߽��ϴ�.");
	}
}