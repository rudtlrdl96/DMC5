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
		MsgAssert("���ؽ� ���۰� �������� �ʾƼ� ��ǲ�����1 ������ ������ �� �����ϴ�.");
		return;
	}

	VertexBufferPtr->Setting();

	GameEngineDevice::GetContext()->IASetPrimitiveTopology(TOPOLOGY);
}

void GameEngineMesh::InputAssembler2()
{
	// �׸��� ������ ���� �����͸� �־��ش� // 012023
	if (nullptr == IndexBufferPtr)
	{
		MsgAssert("�ε��� ���۰� �������� �ʾƼ� ��ǲ �����2 ������ ������ �� �����ϴ�.");
		return;
	}

	IndexBufferPtr->Setting();
}

void GameEngineMesh::Setting()
{
	InputAssembler1();
	InputAssembler2();
}