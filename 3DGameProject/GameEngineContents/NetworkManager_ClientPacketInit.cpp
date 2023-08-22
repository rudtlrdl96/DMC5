#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include "NetworkGUI.h"
#include "ContentsEnum.h"

#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"
#include "MessageChatPacket.h"

////////
//		Ŭ�� ��Ŷ �ʱ�ȭ
////////


void NetworkManager::ClientPacketInit()
{
	//ConnectIDPacket ó��
	NetInst->Dispatcher.AddHandler<ConnectIDPacket>
		([](std::shared_ptr<ConnectIDPacket> _Packet)
	{
		unsigned int ID = _Packet->GetObjectID();
		NetID = ID;
		NetworkGUI::GetInst()->SetClientTitle(ID);
	});


	//ObjectUpdatePacket ó��
	NetInst->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
		//�ش� NetObejctID�� ��ü�� �������� �ʴٸ� �����
		if (false == GameEngineNetObject::IsNetObject(_Packet->GetObjectID()))
		{
			std::shared_ptr<GameEngineNetObject> NewNetObj = nullptr;
			NewNetObj = NetworkManager::CreateNetActor(_Packet->ActorType, _Packet->GetObjectID());
			//NewNetObj->SetControll(NetControllType::NetControll);
		}

		//Player�� ������ ó���� �� �ְ� �ڷᱸ���� ����
		GameEngineNetObject::PushNetObjectPacket(_Packet);
	});



	//MessageChatPacket ó��
	NetInst->Dispatcher.AddHandler<MessageChatPacket>(
		[](std::shared_ptr<MessageChatPacket> _Packet)
	{
		NetworkGUI::GetInst()->PrintLog(_Packet->Message);
	});
}