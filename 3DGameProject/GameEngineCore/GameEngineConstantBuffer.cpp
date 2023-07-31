#include "PrecompileHeader.h"
#include "GameEngineConstantBuffer.h"

std::map<int, std::map<std::string, std::shared_ptr<GameEngineConstantBuffer>>> GameEngineConstantBuffer::ConstantBufferRes;

void GameEngineConstantBuffer::ResourcesClear()
{
	GameEngineResource<GameEngineConstantBuffer>::ResourcesClear();

	ConstantBufferRes.clear();
}

GameEngineConstantBuffer::GameEngineConstantBuffer() 
{
}

GameEngineConstantBuffer::~GameEngineConstantBuffer() 
{
}

void GameEngineConstantBuffer::ChangeData(const void* _Data, UINT _Size)
{
	// ��Ƽ������� ������۳� �̷��� ������ ���� ���̴�.
	if (nullptr == _Data)
	{
		std::string Name = GetName().data();
		MsgAssert(Name + "�� nullptr�� �����͸� �����Ϸ��� �߽��ϴ�.");
		return;
	}

	if (BufferInfo.ByteWidth != _Size)
	{
		std::string Name = GetName().data();
		MsgAssert(Name + "ũ�Ⱑ �ٸ� �����Ͱ� ���Խ��ϴ�.");
		return;
	}

	D3D11_MAPPED_SUBRESOURCE SettingResources = {0,};

	// �׷���ī��� ������ �������־� ��� ����� 
	// D3D11_MAP_WRITE_DISCARD �ִ��� ������ ó���ϴ� 
	GameEngineDevice::GetContext()->Map(Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SettingResources);

	if (SettingResources.pData == nullptr)
	{
		std::string Name = GetName().data();
		MsgAssert(Name + " �׷���ī�忡�� �޸� ������ �㰡���� ���߽��ϴ�.");
		return;
	}
	memcpy_s(SettingResources.pData, BufferInfo.ByteWidth, _Data, BufferInfo.ByteWidth);
	
	GameEngineDevice::GetContext()->Unmap(Buffer, 0);
}

void GameEngineConstantBuffer::ResCreate(const D3D11_SHADER_BUFFER_DESC& _BufferDesc) 
{
	BufferInfo.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferInfo.ByteWidth = _BufferDesc.Size;

	BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	if (0 == BufferInfo.CPUAccessFlags)
	{
		BufferInfo.Usage = D3D11_USAGE_DEFAULT;
	}
	else {
		BufferInfo.Usage = D3D11_USAGE_DYNAMIC;
	}

	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(&BufferInfo, nullptr, &Buffer))
	{
		MsgAssert("���ؽ� ���� ������ �����߽��ϴ�.");
	}
}

void GameEngineConstantBuffer::VSSetting(UINT _Slot)
{
	GameEngineDevice::GetContext()->VSSetConstantBuffers(_Slot, 1, &Buffer);
}
void GameEngineConstantBuffer::PSSetting(UINT _Slot)
{
	GameEngineDevice::GetContext()->PSSetConstantBuffers(_Slot, 1, &Buffer);
}