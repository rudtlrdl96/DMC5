#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include <GameEngineBase/GameEngineNetObject.h>

#include "NetworkGUI.h"

#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"
#include "MessageChatPacket.h"
#include "LinkObjectPacket.h"

////////
//		���� ��Ŷ �ʱ�ȭ
////////

void NetworkManager::AcceptCallback(SOCKET _Socket, GameEngineNetServer* _Server)
{
	// ������ ������Ը� ����, ConnectIDPacket��Ŷ
	std::shared_ptr<ConnectIDPacket> Packet = std::make_shared<ConnectIDPacket>();

	//��� ���� Ŭ���̾�Ʈ�� ��Ʈ��ũ ���̵� ����
	int ClientID = GameEngineNetObject::CreateServerID();
	Packet->SetObjectID(ClientID);

	//������ ID�� ������ �������� ����
	_Server->AddUser(ClientID, _Socket);

	//��� ���� Ŭ���̾�Ʈ���Ը� Ŀ��Ʈ ��Ŷ ������
	GameEngineSerializer Ser;
	Packet->SerializePacket(Ser);
	GameEngineNet::Send(_Socket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
}


void NetworkManager::ServerPacketInit()
{
	//ObjectUpdatePacket ó��
	NetInst->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
		//Ŭ��� ���� ���� ��Ŷ�� ���� ������ �ٸ� ���
		if (CurLevelType != _Packet->LevelType)
			return;

		unsigned int ObjID = _Packet->GetObjectID();

		//�ش� NetObejctID�� ��ü�� �������� �ʴ� ��� ���⼭ ����������
		if (false == GameEngineNetObject::IsNetObject(ObjID))
		{
			std::shared_ptr<GameEngineNetObject> NewNetObj = nullptr;
			NewNetObj = NetworkManager::CreateNetActor(_Packet->ActorType, ObjID);
			NewNetObj->SetControll(NetControllType::NetControll);
		}

		//��Ŷ�� Deathó�� �� ���
		if (true == _Packet->IsDeath)
		{
			//��Ʈ��ũ�� ���� ����
			GameEngineNetObject* NetObject = nullptr;
			NetObject = GameEngineNetObject::GetNetObject(ObjID);
			NetObject->NetDisconnect();

			GameEngineActor* ActorPtr = dynamic_cast<GameEngineActor*>(NetObject);
			if (nullptr == ActorPtr)
			{
				MsgAssert("�ش� NetObejct�� GameEngineActorŸ������ ĳ�������� ���ؼ� Deathó���� �� �� �����ϴ�.");
				return;
			}
			ActorPtr->Death();
		}

		//��Ŷ�� Deathó�� ���� ���� ��쿡�� �����ʿ� ��Ŷ ����
		else
		{
			//Player�� ������ ó���� �� �ְ� �ڷᱸ���� ����
			GameEngineNetObject::PushNetObjectPacket(_Packet);
		}

		//������ ��쿣 ���Ź��� Ư�� ������Ʈ�� ��Ŷ�� �ٸ� Ŭ�� �� �ѷ��� �Ѵ�
		NetInst->SendPacket(_Packet, _Packet->NetID);
	});


	//MessageChatPacket ó��
	NetInst->Dispatcher.AddHandler<MessageChatPacket>(
		[=](std::shared_ptr<MessageChatPacket> _Packet)
	{
		NetworkGUI::GetInst()->PrintLog(_Packet->Message);

		//������ ��쿣 ���Ź��� Ư�� ������Ʈ�� ��Ŷ�� �ٸ� Ŭ�� �� �ѷ��� �Ѵ�
		NetInst->SendPacket(_Packet, _Packet->GetObjectID());
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
		ReplyLinkPacket->Ptr = _Packet->Ptr;

		//��Ŷ����ȭ
		GameEngineSerializer Ser;
		ReplyLinkPacket->SerializePacket(Ser);

		//������ ������ �������׸� ������Ŷ�� ������
		SOCKET ClientSocket = ServerInst.GetUser(CliendID);
		GameEngineNet::Send(ClientSocket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
	});
}

