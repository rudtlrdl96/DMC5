#include "PrecompileHeader.h"
#include "GameEngineInputLayOut.h"
#include "GameEngineVertex.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"

GameEngineInputLayOut::GameEngineInputLayOut() 
{
}

GameEngineInputLayOut::~GameEngineInputLayOut() 
{
	Relase();
}

void GameEngineInputLayOut::Relase()
{
	if (nullptr != InputLayOut)
	{
		InputLayOut->Release();
		InputLayOut = nullptr;
	}
}

void GameEngineInputLayOut::ResCreate(std::shared_ptr<class GameEngineVertexBuffer> _Info, std::shared_ptr<class GameEngineVertexShader> _Shader)
{
	Relase();

	if (nullptr == _Info->LayOutInfo)
	{
		MsgAssert("���̾ƿ� ������ ����� ���� ���ؽ� ���� �Դϴ�.");
	}

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& LayOutInfos = _Info->LayOutInfo->GetDescs();

	HRESULT Result = GameEngineDevice::GetDevice()->CreateInputLayout(
		&LayOutInfos[0],
		static_cast<UINT>(LayOutInfos.size()),
		_Shader->BinaryCode->GetBufferPointer(),
		_Shader->BinaryCode->GetBufferSize(),
		&InputLayOut);

	if (S_OK != Result)
	{
		MsgAssert("��ǲ ���̾ƿ� ������ �����߽��ϴ�.");
	}

}

void GameEngineInputLayOut::Setting()
{
	if (nullptr == InputLayOut)
	{
		MsgAssert("�������� ���� ��ǲ���̾ƿ��� �����Ϸ��� �߽��ϴ�.");
	}

	GameEngineDevice::GetContext()->IASetInputLayout(InputLayOut);
}