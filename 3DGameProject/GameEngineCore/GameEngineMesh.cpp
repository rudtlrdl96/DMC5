#include "PrecompileHeader.h"
#include "GameEngineMesh.h"

GameEngineMesh::GameEngineMesh() 
{
}

GameEngineMesh::~GameEngineMesh() 
{
}

void GameEngineMesh::InputAssembler1()
{
	if (nullptr == VertexBufferPtr)
	{
		MsgAssert("버텍스 버퍼가 존재하지 않아서 인풋어셈블러1 과정을 실행할 수 없습니다.");
		return;
	}

	VertexBufferPtr->Setting();

	GameEngineDevice::GetContext()->IASetPrimitiveTopology(TOPOLOGY);
}

void GameEngineMesh::InputAssembler2()
{
	// 그리는 순서에 대한 데이터를 넣어준다 // 012023
	if (nullptr == IndexBufferPtr)
	{
		MsgAssert("인덱스 버퍼가 존재하지 않아서 인풋 어셈블러2 과정을 실행할 수 없습니다.");
		return;
	}

	IndexBufferPtr->Setting();
}

void GameEngineMesh::Setting()
{
	InputAssembler1();
	InputAssembler2();
}