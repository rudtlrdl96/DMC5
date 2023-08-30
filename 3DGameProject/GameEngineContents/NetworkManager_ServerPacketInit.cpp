#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include <GameEngineBase/GameEngineNetObject.h>

#include "NetworkGUI.h"

#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"
#include "MessageChatPacket.h"

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

	//�÷��̾� ĳ���͸� ������ų ������Ʈ ���̵� ����
	Packet->AllObjectID.resize(AllBattleLevels.size(), 0);
	for (size_t i = 0; i < AllBattleLevels.size(); ++i)
	{
		Packet->AllObjectID[i] = GameEngineNetObject::CreateServerID();
	}

	//������ ID�� ������ �������� ����
	_Server->AddUser(ClientID, _Socket);

	//��� ���� Ŭ���̾�Ʈ���Ը� Ŀ��Ʈ ��Ŷ ������
	GameEngineSerializer Ser;
	Packet->SerializePacket(Ser);
	GameEngineNet::Send(_Socket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());


	////GUI�� �α� ����
	//std::shared_ptr<MessageChatPacket> MsgPacket = std::make_shared<MessageChatPacket>();
	//MsgPacket->Message = GameEngineString::ToString(ID) + " Client Connect";
	////NetworkGUI::GetInst()->PrintLog(MsgPacket->Message);

	////�ٸ� Ŭ��鿡�� �޼��� ����
	//_Server->SendPacket(MsgPacket, ID);
}


void NetworkManager::ServerPacketInit()
{
	//ObjectUpdatePacket ó��
	NetInst->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
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
		}
		
		//Player�� ������ ó���� �� �ְ� �ڷᱸ���� ����
		GameEngineNetObject::PushNetObjectPacket(_Packet);

		//������ ��쿣 ���Ź��� Ư�� ������Ʈ�� ��Ŷ�� �ٸ� Ŭ�� �� �ѷ��� �Ѵ�
		NetInst->SendPacket(_Packet, _Packet->NetID);
	});


	//MessageChatPacket ó��
	NetInst->Dispatcher.AddHandler<MessageChatPacket>(
		[=](std::shared_ptr<MessageChatPacket> _Packet)
	{
		//NetworkGUI::GetInst()->PrintLog(_Packet->Message);

		//������ ��쿣 ���Ź��� Ư�� ������Ʈ�� ��Ŷ�� �ٸ� Ŭ�� �� �ѷ��� �Ѵ�
		//NetInst->SendPacket(_Packet, _Packet->GetNetID());
	});
}

