#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include "NetworkGUI.h"

#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"
#include "MessageChatPacket.h"

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


	//GUI에 로그 띄우기
	std::shared_ptr<MessageChatPacket> MsgPacket = std::make_shared<MessageChatPacket>();
	MsgPacket->Message = GameEngineString::ToString(ID) + " Client Connect";
	NetworkGUI::GetInst()->PrintLog(MsgPacket->Message);

	//다른 클라들에게 메세지 전달
	_Server->SendPacket(MsgPacket, ID);
}


void NetworkManager::ServerPacketInit()
{
	//ObjectUpdatePacket 처리
	NetInst->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
		//해당 NetObejctID의 객체가 존재하지 않는 경우
		if (false == GameEngineNetObject::IsNetObject(_Packet->GetObjectID()))
		{
			MsgAssert("서버의 허락없이 만들어진 NetObject가 검출되었습니다");
		}

		//Player가 스스로 처리할 수 있게 자료구조에 저장
		GameEngineNetObject::PushNetObjectPacket(_Packet);

		//서버의 경우엔 수신받은 특정 오브젝트의 패킷을 다른 클라에 다 뿌려야 한다
		NetInst->SendPacket(_Packet, _Packet->GetObjectID());
	});


	//MessageChatPacket 처리
	NetInst->Dispatcher.AddHandler<MessageChatPacket>(
		[=](std::shared_ptr<MessageChatPacket> _Packet)
	{
		NetworkGUI::GetInst()->PrintLog(_Packet->Message);

		//서버의 경우엔 수신받은 특정 오브젝트의 패킷을 다른 클라에 다 뿌려야 한다
		NetInst->SendPacket(_Packet, _Packet->GetObjectID());
	});
}

