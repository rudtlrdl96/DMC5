#include "PrecompileHeader.h"
#include "GameEngineVertexBuffer.h"

GameEngineVertexBuffer::GameEngineVertexBuffer() 
{
}

GameEngineVertexBuffer::~GameEngineVertexBuffer() 
{
}


void GameEngineVertexBuffer::ResCreate(const void* _Data, UINT _VertexSize, UINT _VertexCount)
{
	VertexSize = _VertexSize;
	VertexCount = _VertexCount;

	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = _Data;

	BufferInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferInfo.ByteWidth = VertexSize * VertexCount;

	// �޸� ��� �Ѱž�? 
	//	D3D11_USAGE_DEFAULT
	//	�� : 0
	//	GPU���� �б� �� ���� �׼����� �ʿ��� ���ҽ��Դϴ�.�̴� ���� �Ϲ����� ��� ������ �� �ֽ��ϴ�.
	//  cpu���� ���ٸ���.
	//	D3D11_USAGE_IMMUTABLE
	//	�� : 1
	//	GPU������ ���� �� �ִ� ���ҽ��Դϴ�.GPU���� �� �� ������ CPU���� ���� �׼����� �� �����ϴ�.���� �Ŀ��� ������ �� �����Ƿ� �� ������ ���ҽ��� ������ �� �ʱ�ȭ�ؾ� �մϴ�.
	//	D3D11_USAGE_DYNAMIC
	//	�� : 2
	//	GPU(�б� ����)�� CPU(���� ����)���� ��� �׼����� �� �ִ� ���ҽ��Դϴ�.���� ���ҽ��� CPU���� �����Ӵ� �� �� �̻� ������Ʈ�Ǵ� ���ҽ��� �����մϴ�.���� ���ҽ��� ������Ʈ�Ϸ��� Map �޼��带 ����մϴ�.
	//	���� ���ҽ��� ����ϴ� ����� ���� �ڼ��� ������ ��� : ���� ���ҽ� ��� ����� �����ϼ���.
	//	D3D11_USAGE_STAGING
	//	�� : 3
	//	GPU���� CPU���� ������ ����(����)�� �����ϴ� ���ҽ��Դϴ�. ��Ʈ����


	//D3D11_CPU_ACCESS_WRITE
	//	�� : 0x10000L
	//	CPU�� ������ ������ �� �ֵ��� ���ҽ��� ������ �� �ֽ��ϴ�.�� �÷��׷� ������ ���ҽ��� ������������ ������� ������ �� ������ ���� �Ǵ� ������¡ ������� �����Ǿ�� �մϴ�(D3D11_USAGE ����).
	//	D3D11_CPU_ACCESS_READ
	//	�� : 0x20000L
	//	CPU�� ������ ���� �� �ֵ��� ���ҽ��� ������ �� �ֽ��ϴ�.�� �÷��׷� ������ ���ҽ��� ���������ο� ���� �Է� �Ǵ� ������� ������ �� ������ ������¡ �뵵�� �����ؾ� �մϴ�(D3D11_USAGE ����).

	BufferInfo.CPUAccessFlags = 0;
	if (0 == BufferInfo.CPUAccessFlags)
	{
		BufferInfo.Usage = D3D11_USAGE_DEFAULT;
	}
	else {
		BufferInfo.Usage = D3D11_USAGE_DYNAMIC;
	}

	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(&BufferInfo, &Data, &Buffer))
	{
		MsgAssert("���ؽ� ���� ������ �����߽��ϴ�.");
	}

}

void GameEngineVertexBuffer::Setting()
{
	if (nullptr == Buffer)
	{
		MsgAssert("ID3DBuffer�� ��������� ���� ���ؽ� ���� �Դϴ�.");
		return;
	}
	// UINT StartSlot, ���ؽ� ���۸� ������ ���������� �� �������� ���° ���� �����ؼ� �����ҷ�
	// UINT NumBuffers, ���ؽ� ���۸� �ܰ� �����߾�?
	// ID3D11Buffer* const* ppVertexBuffers, ������ �迭 �迭�� 1��
	// const UINT* pStrides, ���ؽ� 1���� ũ�Ⱑ �󸶾�
	// const UINT* pOffsets ���ؽ� ���° ���� �ұ�?

	
	GameEngineDevice::GetContext()->IASetVertexBuffers(0, 1, &Buffer, &VertexSize, &Offset);
}