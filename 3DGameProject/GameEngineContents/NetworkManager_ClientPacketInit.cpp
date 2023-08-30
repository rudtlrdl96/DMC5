#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include "NetworkGUI.h"
#include "ContentsEnum.h"

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
		([=](std::shared_ptr<ConnectIDPacket> _Packet)
	{
		//이 클라이언트의 네트워크 아이디 지정
		NetID = _Packet->GetObjectID();
		NetworkGUI::GetInst()->InitName(NetID);

		//패킷에 담겨온 오브젝트아이디로 플레이어 캐릭터 생성
		const std::vector<unsigned int>& AllObjectID = _Packet->AllObjectID;
		if (AllObjectID.size() != AllBattleLevels.size())
		{
			MsgAssert("패킷으로 받은 오브젝트ID의 갯수와 전투가 진행되는 레벨의 갯수가 다릅니다");
			return;
		}

		for (size_t i = 0; i < AllObjectID.size(); ++i)
		{
			GameEngineLevel* Level = AllBattleLevels[i];
			int ID = static_cast<int>(AllObjectID[i]);
			CreateLocalPlayer(Level, ID);
		}
	});


	//ObjectUpdatePacket 처리
	NetInst->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
		unsigned int ObjID = _Packet->GetObjectID();

		//해당 NetObejctID의 객체가 존재하지 않다면 만든다면 여기서 만들어버리기
		if (false == GameEngineNetObject::IsNetObject(ObjID))
		{
			std::shared_ptr<GameEngineNetObject> NewNetObj = nullptr;
			NewNetObj = NetworkManager::CreateNetActor(_Packet->ActorType, ObjID);
			NewNetObj->SetControll(NetControllType::NetControll);
		}

		//패킷이 Death처리 된 경우
		if (true == _Packet->IsDeath)
		{
			//네트워크와 연결 끊기
			GameEngineNetObject* NetObject = nullptr;
			NetObject = GameEngineNetObject::GetNetObject(ObjID);
			NetObject->NetDisconnect();

			GameEngineActor* ActorPtr = dynamic_cast<GameEngineActor*>(NetObject);
			if (nullptr == ActorPtr)
			{
				MsgAssert("해당 NetObejct가 GameEngineActor타입으로 캐스팅하지 못해서 Death처리를 할 수 없습니다.");
				return;
			}
			ActorPtr->Death();
		}

		//패킷이 Death처리 되지 않은 경우에만 엑터쪽에 패킷 전달
		else
		{
			//Player가 스스로 처리할 수 있게 자료구조에 저장
			GameEngineNetObject::PushNetObjectPacket(_Packet);
		}
	});



	//MessageChatPacket 처리
	NetInst->Dispatcher.AddHandler<MessageChatPacket>(
		[](std::shared_ptr<MessageChatPacket> _Packet)
	{
		//NetworkGUI::GetInst()->PrintLog(_Packet->Message);
	});
	
}