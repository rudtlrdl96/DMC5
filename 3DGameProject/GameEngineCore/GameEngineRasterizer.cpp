#include "PrecompileHeader.h"
#include "GameEngineRasterizer.h"

GameEngineRasterizer::GameEngineRasterizer() 
{
}

GameEngineRasterizer::~GameEngineRasterizer() 
{
	Release();
}

void GameEngineRasterizer::Setting() 
{
	if (nullptr == CurState)
	{
		MsgAssert("�������� ���� �����Ͷ����� ������Ʈ �Դϴ�.");
	}

	GameEngineDevice::GetContext()->RSSetState(CurState);
}

void GameEngineRasterizer::Release()
{
	if (nullptr != SolidState)
	{
		SolidState->Release();
		SolidState = nullptr;
	}

	if (nullptr != WireframeState)
	{
		WireframeState->Release();
		WireframeState = nullptr;
	}
}

void GameEngineRasterizer::ResCreate(const D3D11_RASTERIZER_DESC& _Value) 
{
	Release();
	Desc = _Value;

	Desc.FillMode = D3D11_FILL_WIREFRAME;
	if (S_OK != GameEngineDevice::GetDevice()->CreateRasterizerState(&Desc, &WireframeState))
	{
		MsgAssert("���̾� ������ �����Ͷ����� ������Ʈ ������ �����߽��ϴ�.");
	}

	Desc.FillMode = D3D11_FILL_SOLID;
	if (S_OK != GameEngineDevice::GetDevice()->CreateRasterizerState(&Desc, &SolidState))
	{
		MsgAssert("�ָ��� �����Ͷ����� ������Ʈ ������ �����߽��ϴ�.");
	}

	SetFILL_MODE(D3D11_FILL_SOLID);
}