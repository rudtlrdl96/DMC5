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
			//GUI에 로그 띄우고 나머지 클라에도 오류 메세지 보내기
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



	//CreateObjectPacket 처리
	NetInst->Dispatcher.AddHandler<CreateObjectPacket>(
		[](std::shared_ptr<CreateObjectPacket> _Packet)
	{
		int ID = GameEngineNetObject::CreateServerID();

		std::shared_ptr<GameEngineNetObject> NewNetObj = nullptr;
		NewNetObj = NetworkManager::CreateNetActor(_Packet->ActorType, ID);
		NewNetObj->SetControll(NetControllType::UserControll);
		_Packet->SetObjectID(ID);

		//Player가 스스로 처리할 수 있게 자료구조에 저장
		GameEngineNetObject::PushNetObjectPacket(_Packet);
	});



	//LinkObjectPacket 처리
	NetInst->Dispatcher.AddHandler<LinkObjectPacket>(
		[=](std::shared_ptr<LinkObjectPacket> _Packet)
	{
		unsigned int CliendID = _Packet->GetObjectID();
		unsigned int NewID = GameEngineNetObject::CreateServerID();

		//수신받은 오브젝트 생성
		std::shared_ptr<GameEngineNetObject> NewNetObj = nullptr;
		NewNetObj = NetworkManager::CreateNetActor(_Packet->ActorType, NewID);
		//서버가 컨트롤 하지 않음
		NewNetObj->SetControll(NetControllType::NetControll);


		//답장용 새 패킷
		std::shared_ptr<LinkObjectPacket> ReplyLinkPacket = std::make_shared<LinkObjectPacket>();

		//생성했을때 사용한 새 오브젝트 ID 넣음
		ReplyLinkPacket->SetObjectID(NewID);
		ReplyLinkPacket->Ptr = _Packet->Ptr;

		//패킷직렬화
		GameEngineSerializer Ser;
		ReplyLinkPacket->SerializePacket(Ser);

		//나에게 전송한 유저한테만 패킷을 보낸다
		SOCKET ClientSocket = ServerInst.GetUser(CliendID);
		GameEngineNet::Send(ClientSocket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
	});
}

