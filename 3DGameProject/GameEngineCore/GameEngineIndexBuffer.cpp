#include "PrecompileHeader.h"
#include "GameEngineIndexBuffer.h"

GameEngineIndexBuffer::GameEngineIndexBuffer() 
{
}

GameEngineIndexBuffer::~GameEngineIndexBuffer() 
{
}

void GameEngineIndexBuffer::ResCreate(const void* _Data, UINT _VertexSize, UINT _VertexCount)
{
	IndexSize = _VertexSize;

	switch (IndexSize)
	{
	case 2:
		Format = DXGI_FORMAT_R16_UINT;
		break;
	case 4:
		Format = DXGI_FORMAT_R32_UINT;
		break;
	default:
		break;
	}

	IndexCount = _VertexCount;

	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = _Data;

	BufferInfo.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferInfo.ByteWidth = IndexSize * IndexCount;

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

void GameEngineIndexBuffer::Setting()
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

	
	GameEngineDevice::GetContext()->IASetIndexBuffer(Buffer, Format, Offset);
}