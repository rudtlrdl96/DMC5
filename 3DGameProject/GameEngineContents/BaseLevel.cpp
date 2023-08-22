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

	//수신받은 패킷을 처리하는 부분입니다.
	//네트워크 매니저에서 패킷처리
	NetworkManager::Update_PacketProcess(this);
	//각각의 GameEngineNetObject에서 패킷처리
	GameEngineNetObject::Update_ProcessPackets();
}


void BaseLevel::NetworkUpdate(float _DeltaTime)
{
	GameEngineLevel::NetworkUpdate(_DeltaTime);

	//GameEngineNetObject의 Update_PushPacket를 호출
	GameEngineNetObject::Update_SendPackets(_DeltaTime);

	Timer += _DeltaTime;
	if (Timer < WaitTime)
		return;

	NetworkManager::FlushUpdatePacket();
	Timer -= WaitTime;
}