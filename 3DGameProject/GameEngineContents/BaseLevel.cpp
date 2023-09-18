#include "PrecompileHeader.h"
#include "BaseLevel.h"

#include "NetworkManager.h"

std::map<Net_LevelType, BaseLevel*> BaseLevel::AllNetLevel;


BaseLevel* BaseLevel::GetNetLevel(Net_LevelType _Type)
{
	if (false == AllNetLevel.contains(_Type))
	{
		MsgAssert("지정하지 않은 네트워크 레벨타입으로 레벨포인터를 받아오려고 했습니다");
		return nullptr;
	}

	return AllNetLevel[_Type];
}




BaseLevel::BaseLevel()
{

}

BaseLevel::~BaseLevel()
{

}

void BaseLevel::Start() 
{

}

void BaseLevel::SetNetLevelType(Net_LevelType _Type)
{
	if (Net_LevelType::UNKNOWN == _Type)
	{
		MsgAssert("네트워크 레벨 타입을 UNKNOWN으로 지정할 수는 없습니다");
		return;
	}

	NetLevelType = _Type;

	if (true == AllNetLevel.contains(_Type))
	{
		MsgAssert("이미 지정된 네트워크 레벨 타입입니다");
		return;
	}

	AllNetLevel[_Type] = this;
}

void BaseLevel::LevelChangeStart()
{
	GameEngineLevel::LevelChangeStart();

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
	GameEngineNetObject::ReleaseNetObject();

	NetTimer += _DeltaTime;
	if (NetTimer < NetworkManager::PacketFlushTime)
		return;

	NetworkManager::FlushPackets();
	NetTimer -= NetworkManager::PacketFlushTime;
}