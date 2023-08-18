#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"

////////
//		서버 패킷 초기화
////////

void NetworkManager::AcceptCallback(SOCKET _Socket, GameEngineNetServer* _Server)
{
	// 접속한 사람에게만 보냄, ConnectIDPacket패킷
	std::shared_ptr<ConnectIDPacket> Packet = std::make_shared<ConnectIDPacket>();

	int ID = GameEngineNetObject::CreateServerID();
	Packet->SetObjectID(ID);

	GameEngineSerializer Ser;
	Packet->SerializePacket(Ser);

	//상대방의 ID와 소켓을 연관지어 저장
	_Server->AddUser(ID, _Socket);

	// 유일하게 한번 딱 직접 소켓을 써서 보내야할때.
	GameEngineNet::Send(_Socket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
}


void NetworkManager::ServerPacketInit()
{
	//ObjectUpdatePacket 처리
	NetInst->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
		//해당 NetObejctID의 객체가 존재하지 않다면 만든다
		/*if (false == GameEngineNetObject::IsNetObject(_Packet->GetObjectID()))
		{
			std::shared_ptr<Player> NewPlayer = NowLevel->CreateActor<Player>();
			NewPlayer->InitNetObject(_Packet->GetObjectID(), _Net);
		}*/

		//Player가 스스로 처리할 수 있게 자료구조에 저장
		GameEngineNetObject::PushNetObjectPacket(_Packet);

		//서버의 경우엔 수신받은 특정 오브젝트의 패킷을 다른 클라에 다 뿌려야 한다
		NetInst->SendPacket(_Packet, _Packet->GetObjectID());
	});



}

