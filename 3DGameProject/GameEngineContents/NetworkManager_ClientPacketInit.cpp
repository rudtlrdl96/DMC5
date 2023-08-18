#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include "NetworkGUI.h"

#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"
#include "MessageChatPacket.h"

////////
//		클라 패킷 초기화
////////


void NetworkManager::ClientPacketInit()
{
	//ConnectIDPacket 처리
	NetInst->Dispatcher.AddHandler<ConnectIDPacket>
		([](std::shared_ptr<ConnectIDPacket> _Packet)
	{
		//처음 접속 했다면 Player를 서버와 연동 시작
		//Player::MainPlayer->InitNetObject(_Packet->GetObjectID(), ServerWindow::NetInst);
	});


	//ObjectUpdatePacket 처리
	NetInst->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
		//해당 NetObejctID의 객체가 존재하지 않다면 만든다
		if (false == GameEngineNetObject::IsNetObject(_Packet->GetObjectID()))
		{
			//std::shared_ptr<Player> NewPlayer = NowLevel->CreateActor<Player>();
			//NewPlayer->InitNetObject(_Packet->GetObjectID(), _Net);
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
}