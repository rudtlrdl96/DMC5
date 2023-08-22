#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include <GameEngineCore/GameEngineLevel.h>

#include "ContentsEnum.h"
#include "PacketEnum.h"

#include "ObjectUpdatePacket.h"
#include "MessageChatPacket.h"

GameEngineNet* NetworkManager::NetInst = nullptr;
GameEngineNetServer NetworkManager::ServerInst;
GameEngineNetClient NetworkManager::ClientInst;

bool NetworkManager::IsServerValue = false;
bool NetworkManager::IsClientValue = false;

unsigned int NetworkManager::NetID = 0;
GameEngineLevel* NetworkManager::CurLevel = nullptr;



void NetworkManager::ServerOpen(int _Port)
{
	//Thread 이름 설정
	SetThreadDescription(GetCurrentThread(), L"Server Main Thread");

	NetInst = &ServerInst;
	
	//클라이언트가 Accept될 때 처리해야 할 작업 등록
	ServerInst.SetAcceptCallBack(AcceptCallback);
	
	//서버용 패킷 처리 콜백 등록
	ServerPacketInit();

	//서버 오픈
	ServerInst.ServerOpen(static_cast<unsigned short>(_Port));
	NetID = GameEngineNetObject::CreateServerID();
	IsServerValue = true;
}


bool NetworkManager::ConnectServer(const std::string_view& _IP, int _Port)
{
	//Thread 이름 설정
	SetThreadDescription(GetCurrentThread(), L"Client Main Thread");

	NetInst = &ClientInst;

	//클라용 패킷 처리 콜백 등록
	ClientPacketInit();

	//서버 접속
	IsClientValue = ClientInst.Connect(_IP.data(), static_cast<unsigned short>(_Port));
	return IsClientValue;
}

//#include <GameEngineBase/GameEngineNetObject.h>
#include "Player.h"

std::shared_ptr<GameEngineNetObject> NetworkManager::CreateNetActor(std::shared_ptr<ObjectUpdatePacket> _Packet)
{
	bool Filter = false;
	PacketEnum PacketType = static_cast<PacketEnum>(_Packet->Type);
	Filter |= (IsServerValue && (PacketEnum::CreateObjectPacket == PacketType));
	Filter |= (IsClientValue && (PacketEnum::ObjectUpdatePacket== PacketType));
	if (false == Filter)
	{
		std::shared_ptr<MessageChatPacket> MsgPacket = std::make_shared<MessageChatPacket>();
		MsgPacket->SetObjectID(NetID);
		MsgPacket->Message = "Server Receive error packet";
		NetInst->SendPacket(MsgPacket);
	}



	Net_ActorType ActorType = static_cast<Net_ActorType>(_Packet->ActorType);
	std::shared_ptr<GameEngineNetObject> NetObject = nullptr;
	switch (ActorType)
	{
	case Net_ActorType::Nero:
		NetObject = GetLevel()->CreateActor<Player>();	//이거 나중에 꼭 바꿀것
		break;
	case Net_ActorType::Vergil:
		NetObject = GetLevel()->CreateActor<Player>();	//이거 나중에 꼭 바꿀것
		break;
	default:
	{
		MsgAssert("NetworkManager에서 생성할 수 없는 ActorType이 들어왔습니다");
		return nullptr;
	}
	}
	
	//민아ㅠㅣㅏㅇㄴㄹ
	/*
	
		클라는 서버가 허락하기 전까지 Actor를 만들수 없다고 했던거 같은데,
		Spawn 패킷을 만들어야 하나?
	
	
	*/

	//서버가 클라의 패킷을 받아 새로운 엑터를 생성하는 경우
	if (true == IsServerValue)
	{
		int ID = GameEngineNetObject::CreateServerID();
		NetObject->InitNetObject(ID, NetInst);
		_Packet->SetObjectID(ID);
	}
	//클라가 서버의 패킷을 받아 새로운 엑터를 생성하는 경우
	else if (true == IsClientValue)
	{
		NetObject->InitNetObject(_Packet->GetObjectID(), NetInst);
	}

	NetObject->SetControll(NetControllType::NetControll);
	return NetObject;
}
