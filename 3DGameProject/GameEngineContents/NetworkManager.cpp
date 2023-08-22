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
	//Thread �̸� ����
	SetThreadDescription(GetCurrentThread(), L"Server Main Thread");

	NetInst = &ServerInst;
	
	//Ŭ���̾�Ʈ�� Accept�� �� ó���ؾ� �� �۾� ���
	ServerInst.SetAcceptCallBack(AcceptCallback);
	
	//������ ��Ŷ ó�� �ݹ� ���
	ServerPacketInit();

	//���� ����
	ServerInst.ServerOpen(static_cast<unsigned short>(_Port));
	NetID = GameEngineNetObject::CreateServerID();
	IsServerValue = true;
}


bool NetworkManager::ConnectServer(const std::string_view& _IP, int _Port)
{
	//Thread �̸� ����
	SetThreadDescription(GetCurrentThread(), L"Client Main Thread");

	NetInst = &ClientInst;

	//Ŭ��� ��Ŷ ó�� �ݹ� ���
	ClientPacketInit();

	//���� ����
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
		NetObject = GetLevel()->CreateActor<Player>();	//�̰� ���߿� �� �ٲܰ�
		break;
	case Net_ActorType::Vergil:
		NetObject = GetLevel()->CreateActor<Player>();	//�̰� ���߿� �� �ٲܰ�
		break;
	default:
	{
		MsgAssert("NetworkManager���� ������ �� ���� ActorType�� ���Խ��ϴ�");
		return nullptr;
	}
	}
	
	//�ξƤФӤ�������
	/*
	
		Ŭ��� ������ ����ϱ� ������ Actor�� ����� ���ٰ� �ߴ��� ������,
		Spawn ��Ŷ�� ������ �ϳ�?
	
	
	*/

	//������ Ŭ���� ��Ŷ�� �޾� ���ο� ���͸� �����ϴ� ���
	if (true == IsServerValue)
	{
		int ID = GameEngineNetObject::CreateServerID();
		NetObject->InitNetObject(ID, NetInst);
		_Packet->SetObjectID(ID);
	}
	//Ŭ�� ������ ��Ŷ�� �޾� ���ο� ���͸� �����ϴ� ���
	else if (true == IsClientValue)
	{
		NetObject->InitNetObject(_Packet->GetObjectID(), NetInst);
	}

	NetObject->SetControll(NetControllType::NetControll);
	return NetObject;
}
