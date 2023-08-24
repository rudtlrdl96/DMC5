#include "PrecompileHeader.h"
#include "BaseLevel.h"

#include "NetworkManager.h"

BaseLevel::BaseLevel()
{

}

BaseLevel::~BaseLevel()
{

}

void BaseLevel::Start() 
{

}

void BaseLevel::Update(float _DeltaTime)
{
	GameEngineLevel::Update(_DeltaTime);

	//���Ź��� ��Ŷ�� ó���ϴ� �κ��Դϴ�.
	//��Ʈ��ũ �Ŵ������� ��Ŷó��
	NetworkManager::Update_PacketProcess(this);
	//������ GameEngineNetObject���� ��Ŷó��
	GameEngineNetObject::Update_ProcessPackets();
}


void BaseLevel::NetworkUpdate(float _DeltaTime)
{
	GameEngineLevel::NetworkUpdate(_DeltaTime);

	//GameEngineNetObject�� Update_PushPacket�� ȣ��
	GameEngineNetObject::Update_SendPackets(_DeltaTime);
	GameEngineNetObject::ReleaseNetObject();

	Timer += _DeltaTime;
	if (Timer < WaitTime)
		return;

	NetworkManager::FlushUpdatePacket();
	Timer -= WaitTime;
}