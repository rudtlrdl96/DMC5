#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include "NetworkGUI.h"
#include "ContentsEnum.h"

#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"
#include "MessageChatPacket.h"
#include "LinkObjectPacket.h"

////////
//		클라 패킷 초기화
////////


void NetworkManager::ClientPacketInit()
{
	//ConnectIDPacket 처리
	NetInst->Dispatcher.AddHandler<ConnectIDPacket>
		([](std::shared_ptr<ConnectIDPacket> _Packet)
	{
		unsigned int ID = _Packet->GetObjectID();
		NetID = ID;
		NetworkGUI::GetInst()->SetClientTitle(ID);
	});


	//ObjectUpdatePacket 처리
	NetInst->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
		//해당 NetObejctID의 객체가 존재하지 않다면 만든다
		if (false == GameEngineNetObject::IsNetObject(_Packet->GetObjectID()))
		{
			std::shared_ptr<GameEngineNetObject> NewNetObj = nullptr;
			NewNetObj = NetworkManager::CreateNetActor(_Packet->ActorType, _Packet->GetObjectID());
			NewNetObj->SetControll(NetControllType::NetControll);
		}

		//Player가 스스로 처리할 수 있게 자료구조에 저장
		GameEngineNetObject::PushNetObjectPacket(_Packet);
	});



	//MessageChatPacket 처리
	NetInst->Dispatcher.AddHandler<MessageChatPacket>(
		[](std::shared_ptr<MessageChatPacket> _Packet)
	{
		NetworkGUI::GetInst()->PrintLog(_Packet->Message);
	});



	//LinkObjectPacket 처리
	NetInst->Dispatcher.AddHandler<LinkObjectPacket>(
		[](std::shared_ptr<LinkObjectPacket> _Packet)
	{
		GameEngineNetObject* ObjPtr = reinterpret_cast<GameEngineNetObject*>(_Packet->Ptr);
		ObjPtr->InitNetObject(_Packet->GetObjectID(), NetInst);
		ObjPtr->SetUserControllType();
	});
}