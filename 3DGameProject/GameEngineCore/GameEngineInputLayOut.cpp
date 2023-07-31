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
		MsgAssert("레이아웃 정보를 만들수 없는 버텍스 버퍼 입니다.");
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
		MsgAssert("인풋 레이아웃 생성에 실패했습니다.");
	}

}

void GameEngineInputLayOut::Setting()
{
	if (nullptr == InputLayOut)
	{
		MsgAssert("생성되지 않은 인풋레이아웃을 세팅하려고 했습니다.");
	}

	GameEngineDevice::GetContext()->IASetInputLayout(InputLayOut);
}