#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include "NetworkGUI.h"
#include "NetworkObjectBase.h"

#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"
#include "MessageChatPacket.h"
#include "LinkObjectPacket.h"
#include "FsmChangePacket.h"
#include "NetEventPacket.h"

////////
//		서버 패킷 초기화
////////

void NetworkManager::AcceptCallback(SOCKET _Socket, GameEngineNetServer* _Server)
{
	// 접속한 사람에게만 보냄, ConnectIDPacket패킷
	std::shared_ptr<ConnectIDPacket> Packet = std::make_shared<ConnectIDPacket>();
	++ClientCount;
	Packet->IsObserver = (1 < ClientCount);

	//방금 들어온 클라이언트의 네트워크 아이디 지정
	int ClientID = GameEngineNetObject::CreateServerID();
	Packet->SetObjectID(ClientID);

	//상대방의 ID와 소켓을 연관지어 저장
	_Server->AddUser(ClientID, _Socket);

	//방금 들어온 클라이언트에게만 커넥트 패킷 보내기
	GameEngineSerializer Ser;
	Packet->SerializePacket(Ser);
	GameEngineNet::Send(_Socket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
}


void NetworkManager::ServerPacketInit()
{
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	//ObjectUpdatePacket 처리
	//----------------------------------------------------------------------------------------------------------------------------------------------------

	NetInst->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
		if (Net_LevelType::UNKNOWN == _Packet->LevelType)
		{
			MsgAssert("수신된 UpdatePacket의 레벨 타입이 UNKNOWN입니다.");
			return;
		}

		//클라로 부터 받은 패킷이 현재 레벨과 다른 경우
		if (CurLevelType != _Packet->LevelType)
		{
			NetworkGUI::GetInst()->PrintLog("UpdatePacket Receive To Other Level", float4::RED);
			return;
		}

		unsigned int ObjID = _Packet->GetObjectID();

		//해당 NetObejctID의 객체가 존재하지 않는 경우 여기서 만들어버리기
		if (false == GameEngineNetObject::IsNetObject(ObjID))
		{
			std::shared_ptr<NetworkObjectBase> NewNetObj = nullptr;
			NewNetObj = NetworkManager::CreateNetActor(_Packet->ActorType, nullptr, ObjID);
			NewNetObj->SetControll(NetControllType::PassiveControll);

			//디버그용 타겟 설정
			if (NetworkObjectBase::DebugType == NewNetObj->GetNetObjectType())
			{
				NetworkObjectBase::DebugTarget = NewNetObj.get();
			}


			//어떤 유저로부터 어떤 타입의 엑터가 생성되었는지 GUI에 출력
			NetworkGUI::GetInst()->PrintLog("Create Object From UpdatePacket", float4::GREEN);
		}


		NetworkObjectBase* NetObject = nullptr;
		NetObject = NetworkObjectBase::GetNetObj(ObjID);

		//패킷이 Death처리 된 경우
		if (true == _Packet->IsDeath)
		{
			//네트워크와 연결 끊기
			NetObject->NetDisconnect();

			GameEngineActor* ActorPtr = dynamic_cast<GameEngineActor*>(NetObject);
			if (nullptr == ActorPtr)
			{
				MsgAssert("해당 NetObejct가 GameEngineActor타입으로 캐스팅하지 못해서 Death처리를 할 수 없습니다.");
				return;
			}
			ActorPtr->Death();
			NetworkGUI::GetInst()->PrintLog("Destroy Actor From UpdatePacket", float4::GREEN);
		}

		//패킷이 Death처리 되지 않은 경우에만 엑터쪽에 패킷 전달
		else
		{
			NetObject->SetNetwortTransData(_Packet->Position, _Packet->Rotation);
			NetObject->ActorTimeScale = _Packet->TimeScale;
			NetObject->SetOnOffState(_Packet->IsUpdate);
			NetObject->SetUpdateArrData(_Packet);

			//Player가 스스로 처리할 수 있게 자료구조에 저장
			//GameEngineNetObject::PushNetObjectPacket(_Packet);
		}

		if (NetworkObjectBase::GetDebugType() == static_cast<Net_ActorType>(_Packet->ActorType))
		{
			NetworkGUI::GetInst()->PrintLog("Object with selected DebugType has successfully received UpdatePacket", float4::GREEN);
		}

		//서버의 경우엔 수신받은 특정 오브젝트의 패킷을 다른 클라에 다 뿌려야 한다
		NetInst->SendPacket(_Packet, _Packet->NetID);
	});



	//----------------------------------------------------------------------------------------------------------------------------------------------------
	//MessageChatPacket 처리
	//----------------------------------------------------------------------------------------------------------------------------------------------------

	NetInst->Dispatcher.AddHandler<MessageChatPacket>(
		[=](std::shared_ptr<MessageChatPacket> _Packet)
	{
		const std::string& Msg = _Packet->Message;
		NetGUI_MsgType MsgType = NetGUI_MsgType::Chat;
		if (SystemChatCheck.size() < Msg.size() && SystemChatCheck == Msg.substr(0, SystemChatCheck.size()))
		{
			MsgType = NetGUI_MsgType::System;
		}

		NetworkGUI::GetInst()->PrintLog(_Packet->Message, _Packet->Color, MsgType);

		//서버의 경우엔 수신받은 특정 오브젝트의 패킷을 다른 클라에 다 뿌려야 한다
		NetInst->SendPacket(_Packet, _Packet->GetObjectID());
	});



	//----------------------------------------------------------------------------------------------------------------------------------------------------
	//LinkObjectPacket 처리
	//----------------------------------------------------------------------------------------------------------------------------------------------------

	NetInst->Dispatcher.AddHandler<LinkObjectPacket>(
		[=](std::shared_ptr<LinkObjectPacket> _Packet)
	{
		unsigned int CliendID = _Packet->GetObjectID();
		unsigned int NewID = GameEngineNetObject::CreateServerID();

		//수신받은 오브젝트 생성
		//std::shared_ptr<GameEngineNetObject> NewNetObj = nullptr;
		//NewNetObj = NetworkManager::CreateNetActor(_Packet->ActorType, NewID);
		////서버가 컨트롤 하지 않음
		//NewNetObj->SetControll(NetControllType::NetControll);


		//답장용 새 패킷
		std::shared_ptr<LinkObjectPacket> ReplyLinkPacket = std::make_shared<LinkObjectPacket>();
		//생성했을때 사용한 새 오브젝트 ID 넣음
		ReplyLinkPacket->SetObjectID(NewID);
		ReplyLinkPacket->Ptr = _Packet->Ptr;

		//패킷직렬화
		GameEngineSerializer Ser;
		ReplyLinkPacket->SerializePacket(Ser);

		//나에게 전송한 유저한테만 답장패킷을 보낸다
		SOCKET ClientSocket = ServerInst.GetUser(CliendID);
		GameEngineNet::Send(ClientSocket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
	});



	//----------------------------------------------------------------------------------------------------------------------------------------------------
	//FsmChangePacket 처리
	//----------------------------------------------------------------------------------------------------------------------------------------------------

	NetInst->Dispatcher.AddHandler<FsmChangePacket>(
		[](std::shared_ptr<FsmChangePacket> _Packet)
	{
		unsigned int ObjID = _Packet->GetObjectID();

		//해당 NetObejctID의 객체가 존재하지 않는 수행하지 않음
		if (false == GameEngineNetObject::IsNetObject(ObjID))
			return;


		//클라로 부터 받은 패킷이 현재 레벨과 다른 경우
		if (CurLevelType != _Packet->LevelType)
			return;

		//네트워크 연결이 끊긴 경우에도 수행하지 않음
		GameEngineNetObject* NetObj = nullptr;
		NetObj = GameEngineNetObject::GetNetObject(ObjID);
		if (true == NetObj->IsNetDisconnected())
			return;

		//Passive에서 넘어온 패킷인 경우엔
		if (0 != _Packet->AttackerID)
		{
			//Active컨트롤 객체도 패킷을 처리할 수 있게함
			_Packet->SetActiveRecv();
		}

		//각자 스스로 처리할 수 있게 자료구조에 저장
		GameEngineNetObject::PushNetObjectPacket(_Packet);

		//서버의 경우엔 수신받은 특정 오브젝트의 패킷을 다른 클라에 다 뿌려야 한다(뿌려야 하나?)
		NetInst->SendPacket(_Packet, _Packet->NetID);
	});




	//----------------------------------------------------------------------------------------------------------------------------------------------------
	//NetEventPacket처리
	//----------------------------------------------------------------------------------------------------------------------------------------------------

	NetInst->Dispatcher.AddHandler<NetEventPacket>(
		[](std::shared_ptr<NetEventPacket> _Packet)
	{
		unsigned int ObjID = _Packet->GetObjectID();
		int EventType = _Packet->EventType;

		//게임 오브젝트의 네트워크 이벤트인 경우
		if (-1 != ObjID)
		{
			//Active여도 받을수 있게 처리
			_Packet->SetActiveRecv();

			//각자 스스로 처리할 수 있게 자료구조에 저장
			GameEngineNetObject::PushNetObjectPacket(_Packet);
			return;
		}

		//게임의 네트워크 이벤트인 경우
		if (true == AllNetEvent.empty() || true == AllNetEvent[EventType].empty())
		{
			NetworkGUI::GetInst()->PrintLog(GameEngineString::ToString(EventType) + " Event Not Exist!", float4::RED);
			return;
		}

		NetworkGUI* NetGUI = NetworkGUI::GetInst();
		std::string LogValue = GameEngineString::ToString(EventType) + " NetworkEvent be executed";

		const std::vector<std::function<void()>>& Events = AllNetEvent[EventType];
		for (const std::function<void()>& Event : Events)
		{
			Event();
			NetGUI->PrintLog(LogValue, float4::GREEN);
		}
	});
}

