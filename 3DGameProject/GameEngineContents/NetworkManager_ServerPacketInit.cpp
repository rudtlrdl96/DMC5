#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include <GameEngineBase/GameEngineNetObject.h>

#include "NetworkGUI.h"

#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"
#include "MessageChatPacket.h"
#include "CreateObjectPacket.h"
#include "LinkObjectPacket.h"

////////
//		���� ��Ŷ �ʱ�ȭ
////////

void NetworkManager::AcceptCallback(SOCKET _Socket, GameEngineNetServer* _Server)
{
	// ������ ������Ը� ����, ConnectIDPacket��Ŷ
	std::shared_ptr<ConnectIDPacket> Packet = std::make_shared<ConnectIDPacket>();

	int ID = GameEngineNetObject::CreateServerID();
	Packet->SetObjectID(ID);

	GameEngineSerializer Ser;
	Packet->SerializePacket(Ser);

	//������ ID�� ������ �������� ����
	_Server->AddUser(ID, _Socket);

	// �����ϰ� �ѹ� �� ���� ������ �Ἥ �������Ҷ�.
	GameEngineNet::Send(_Socket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());


	//GUI�� �α� ����
	std::shared_ptr<MessageChatPacket> MsgPacket = std::make_shared<MessageChatPacket>();
	MsgPacket->Message = GameEngineString::ToString(ID) + " Client Connect";
	NetworkGUI::GetInst()->PrintLog(MsgPacket->Message);

	//�ٸ� Ŭ��鿡�� �޼��� ����
	_Server->SendPacket(MsgPacket, ID);
}


void NetworkManager::ServerPacketInit()
{
	//ObjectUpdatePacket ó��
	NetInst->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
		//�ش� NetObejctID�� ��ü�� �������� �ʴ� ���
		if (false == GameEngineNetObject::IsNetObject(_Packet->GetObjectID()))
		{
			//GUI�� �α� ���� ������ Ŭ�󿡵� ���� �޼��� ������
			std::shared_ptr<MessageChatPacket> MsgPacket = std::make_shared<MessageChatPacket>();
			MsgPacket->Message = "An object created without permission from the server has been detected";
			NetworkGUI::GetInst()->PrintLog(MsgPacket->Message);
			NetInst->SendPacket(MsgPacket);
			return;
		}

		if (true == _Packet->IsDeath)
		{
			GameEngineNetObject* NetObject = nullptr;
			NetObject = GameEngineNetObject::GetNetObject(_Packet->GetObjectID());
			NetObject->NetDisconnect();
		}
		
		//Player�� ������ ó���� �� �ְ� �ڷᱸ���� ����
		GameEngineNetObject::PushNetObjectPacket(_Packet);

		//������ ��쿣 ���Ź��� Ư�� ������Ʈ�� ��Ŷ�� �ٸ� Ŭ�� �� �ѷ��� �Ѵ�
		NetInst->SendPacket(_Packet, _Packet->GetObjectID());
	});


	//MessageChatPacket ó��
	NetInst->Dispatcher.AddHandler<MessageChatPacket>(
		[=](std::shared_ptr<MessageChatPacket> _Packet)
	{
		NetworkGUI::GetInst()->PrintLog(_Packet->Message);

		//������ ��쿣 ���Ź��� Ư�� ������Ʈ�� ��Ŷ�� �ٸ� Ŭ�� �� �ѷ��� �Ѵ�
		NetInst->SendPacket(_Packet, _Packet->GetObjectID());
	});



	//CreateObjectPacket ó��
	NetInst->Dispatcher.AddHandler<CreateObjectPacket>(
		[](std::shared_ptr<CreateObjectPacket> _Packet)
	{
		int ID = GameEngineNetObject::CreateServerID();

		std::shared_ptr<GameEngineNetObject> NewNetObj = nullptr;
		NewNetObj = NetworkManager::CreateNetActor(_Packet->ActorType, ID);
		NewNetObj->SetControll(NetControllType::UserControll);
		_Packet->SetObjectID(ID);

		//Player�� ������ ó���� �� �ְ� �ڷᱸ���� ����
		GameEngineNetObject::PushNetObjectPacket(_Packet);
	});



	//LinkObjectPacket ó��
	NetInst->Dispatcher.AddHandler<LinkObjectPacket>(
		[=](std::shared_ptr<LinkObjectPacket> _Packet)
	{
		unsigned int CliendID = _Packet->GetObjectID();
		unsigned int NewID = GameEngineNetObject::CreateServerID();

		//���Ź��� ������Ʈ ����
		std::shared_ptr<GameEngineNetObject> NewNetObj = nullptr;
		NewNetObj = NetworkManager::CreateNetActor(_Packet->ActorType, NewID);
		//������ ��Ʈ�� ���� ����
		NewNetObj->SetControll(NetControllType::NetControll);


		//����� �� ��Ŷ
		std::shared_ptr<LinkObjectPacket> ReplyLinkPacket = std::make_shared<LinkObjectPacket>();

		//���������� ����� �� ������Ʈ ID ����
		ReplyLinkPacket->SetObjectID(NewID);
		ReplyLinkPacket->LinkID = _Packet->LinkID;
		ReplyLinkPacket->ActorType = -1;

		AllLinkPacket_TEST.push_back(std::make_pair(CliendID, ReplyLinkPacket));

		////��Ŷ����ȭ
		//GameEngineSerializer Ser;
		//ReplyLinkPacket->SerializePacket(Ser);

		////������ ������ �������׸� ��Ŷ�� ������
		//SOCKET ClientSocket = ServerInst.GetUser(CliendID);
		//GameEngineNet::Send(ClientSocket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
	});
}

