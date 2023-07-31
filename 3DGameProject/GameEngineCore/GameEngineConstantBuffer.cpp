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
	// 머티리얼들은 상수버퍼나 이런걸 통일해 놓은 것이다.
	if (nullptr == _Data)
	{
		std::string Name = GetName().data();
		MsgAssert(Name + "에 nullptr인 데이터를 세팅하려고 했습니다.");
		return;
	}

	if (BufferInfo.ByteWidth != _Size)
	{
		std::string Name = GetName().data();
		MsgAssert(Name + "크기가 다른 데이터가 들어왔습니다.");
		return;
	}

	D3D11_MAPPED_SUBRESOURCE SettingResources = {0,};

	// 그래픽카드야 너한테 보낼께있어 잠깐 멈춰봐 
	// D3D11_MAP_WRITE_DISCARD 최대한 빠르게 처리하는 
	GameEngineDevice::GetContext()->Map(Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SettingResources);

	if (SettingResources.pData == nullptr)
	{
		std::string Name = GetName().data();
		MsgAssert(Name + " 그래픽카드에게 메모리 접근을 허가받지 못했습니다.");
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
		MsgAssert("버텍스 버퍼 생성에 실패했습니다.");
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