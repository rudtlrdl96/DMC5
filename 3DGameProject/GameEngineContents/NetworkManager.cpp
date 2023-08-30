#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineActor.h>

#include "NetworkGUI.h"

#include "PacketEnum.h"
#include "ContentsEnum.h"
#include "ObjectUpdatePacket.h"

#include "MainLevel.h"
#include "StartStageLevel.h"
#include "BossStageLevel.h"

//#include "PlayerActor_Nero.h"
//#include "PlayerActor_Vergil.h"

unsigned int NetworkManager::NetID = 0;

GameEngineNet* NetworkManager::NetInst = nullptr;
GameEngineNetServer NetworkManager::ServerInst;
GameEngineNetClient NetworkManager::ClientInst;

NetworkManager::NetworkState NetworkManager::NowState = NetworkManager::NetworkState::None;

std::vector<GameEngineLevel*> NetworkManager::AllBattleLevels;
GameEngineLevel* NetworkManager::CurLevel = nullptr;

std::map<unsigned int, std::shared_ptr<ObjectUpdatePacket>> NetworkManager::AllUpdatePacket;
GameEngineSerializer NetworkManager::ChunkUpdatePackets;

PlayerType NetworkManager::CharacterType = PlayerType::None;



void NetworkManager::ServerOpen(int _Port)
{
	//Thread 이름 설정
	SetThreadDescription(GetCurrentThread(), L"Server Main Thread");

	NetInst = &ServerInst;
	
	//클라이언트가 Accept될 때 처리해야 할 작업 등록
	ServerInst.SetAcceptCallBack(AcceptCallback);
	
	//서버용 패킷 처리 콜백 등록
	ServerPacketInit();

	//서버의 경우 ID는 1번
	NetID = GameEngineNetObject::CreateServerID();

	//서버 오픈
	ServerInst.ServerOpen(static_cast<unsigned short>(_Port));
	NowState = NetworkState::Server;
}




void NetworkManager::ConnectServer(PlayerType _CharacterType)
{
	if (false == NetworkGUI::GetInst()->ChangeFieldState())
	{
		MsgAssert("NetworkGUI에서 서버 연동 작업을 실행하지 않았습니다");
		return;
	}

	//전투가 실행되는 레벨을 벡터에 저장
	if (true == AllBattleLevels.empty())
	{
		AllBattleLevels.push_back(MainLevel::GetInst());
		AllBattleLevels.push_back(StartStageLevel::GetInst());
		AllBattleLevels.push_back(BossStageLevel::GetInst());
	}
	else
	{
		MsgAssert("이미 네트워크 기능을 실행했는데, 다시 한번 네트워크 연동을 실행하였습니다");
		return;
	}

	CharacterType = _CharacterType;

	//서버인 경우 각 전투 레벨에 플레이어 생성
	if (NetworkState::Server == NowState)
	{
		for (GameEngineLevel* Level : AllBattleLevels)
		{
			CreateLocalPlayer(Level, GameEngineNetObject::CreateServerID());
		}
		return;
	}

	//클라이언트인 경우, 서버에 연결 시도
	const std::pair<std::string_view, int> NetworkData = NetworkGUI::GetInst()->GetNetworkData();
	const std::string_view IP = NetworkData.first;
	const int PortNum = NetworkData.second;

	//Thread 이름 설정
	SetThreadDescription(GetCurrentThread(), L"Client Main Thread");
	NetInst = &ClientInst;

	//클라용 패킷 처리 콜백 등록
	ClientPacketInit();

	//서버 접속
	bool IsResult = ClientInst.Connect(IP.data(), static_cast<unsigned short>(PortNum));
	if (true == IsResult)
	{
		NowState = NetworkState::Client;
		return;
	}

	//서버 접속 실패한 경우
	std::string ErrorIP = IP.data();
	MsgAssert("이 클라이언트가 서버 접속에 실패하였습니다.\n접속을 시도한 IP : " + ErrorIP);
}




void NetworkManager::PushUpdatePacket(GameEngineNetObject* _NetObj, GameEngineActor* _ActorPtr, float _TimeScale /*= 1.f*/)
{
	if (NetworkState::None == NowState)
		return;

	//현재 진행중인 레벨의 엑터들만 실행
	if (_ActorPtr->GetLevel() != CurLevel)
		return;

	//인자로 받은 오브젝트의 네트워크용 오브젝트 아이디
	unsigned int ObjectID = _NetObj->GetNetObjectID();
	if (false == GameEngineNetObject::IsNetObject(ObjectID))
	{
		MsgAssert(GameEngineString::ToString(ObjectID) + " ID를 가진 오브젝트가 존재하지 않는데, UpdatePacket을 사용하려고 했습니다");
		return;
	}

	//업데이트 패킷 만들기
	std::shared_ptr<ObjectUpdatePacket> UpdatePacket = nullptr;

	//이미 해당 ID의 UpdatePacket이 존재했다면 재활용
	if (true == AllUpdatePacket.contains(ObjectID))
	{
		UpdatePacket = AllUpdatePacket[ObjectID];
	}

	//ObjectID 이번에 처음 만드는 ObjectUpdatePacket인 경우 새로 동적할당
	else
	{
		UpdatePacket = std::make_shared<ObjectUpdatePacket>();
		AllUpdatePacket[ObjectID] = UpdatePacket;
	}
	

	//패킷 아이디
	//사이즈

	//오브젝트 아이디
	UpdatePacket->SetObjectID(ObjectID);
	//네트워크 아이디
	UpdatePacket->NetID = NetID;

	//오브젝트 타입
	UpdatePacket->ActorType = _NetObj->GetNetObjectType();


	GameEngineTransform* TransPtr = _ActorPtr->GetTransform();

	//위치
	UpdatePacket->Rotation = TransPtr->GetWorldRotation();
	//위치
	UpdatePacket->Position = TransPtr->GetWorldPosition();
	//타임 크기
	UpdatePacket->TimeScale = _TimeScale;

	//파괴 여부
	UpdatePacket->IsDeath = _ActorPtr->IsDeath();
	if (true == UpdatePacket->IsDeath)
	{
		//이 NetObject는 이제부터 전송/수신을 받지 않음
		_NetObj->NetDisconnect();
	}
}


void NetworkManager::FlushUpdatePacket()
{
	if (true == AllUpdatePacket.empty())
		return;

	int PacketSize = 0;
	int Count = 0;

	//패킷 모아 보내기
	for (const std::pair<unsigned int, std::shared_ptr<ObjectUpdatePacket>>& Pair : AllUpdatePacket)
	{
		std::shared_ptr<ObjectUpdatePacket> UpdatePacket = Pair.second;
		UpdatePacket->SerializePacket(ChunkUpdatePackets);
		
		if (0 == PacketSize)
		{
			unsigned char* Ptr = ChunkUpdatePackets.GetDataPtr();
			memcpy_s(&PacketSize, sizeof(int), &Ptr[4], sizeof(int));
			continue;
		}
		
		unsigned char* SizePtr = ChunkUpdatePackets.GetDataPtr();
		int SizePos = (PacketSize * Count++) + 4;
		memcpy_s(&SizePtr[SizePos], sizeof(int), &PacketSize, sizeof(int));
	}

	//한 바이트로 모은 패킷 보내기
	NetInst->Send(ChunkUpdatePackets.GetConstCharPtr(), ChunkUpdatePackets.GetWriteOffSet());

	//자료구조, 직렬화 버퍼 클리어
	AllUpdatePacket.clear();
	ChunkUpdatePackets.Reset();
}


#include "NetTestPlayer.h"

std::shared_ptr<GameEngineNetObject> NetworkManager::CreateNetActor(Net_ActorType _ActorType, int _ObjectID /*= -1*/)
{
	if (nullptr == CurLevel)
	{
		MsgAssert("NetwortManager의 CurLevel 포인터가 nullptr입니다");
	}

	std::shared_ptr<GameEngineNetObject> NetObject = nullptr;
	switch (_ActorType)
	{
	case Net_ActorType::Nero:
		NetObject = CurLevel->CreateActor<NetTestPlayer>();
		break;
	case Net_ActorType::Vergil:
		NetObject = CurLevel->CreateActor<NetTestPlayer>();
		break;
	default:
	{
		MsgAssert("NetworkManager에서 생성할 수 없는 ActorType이 들어왔습니다");
		return nullptr;
	}
	}


	if (-1 == _ObjectID)
	{
		NetObject->InitNetObject(GameEngineNetObject::CreateServerID(), NetInst);
	}
	else
	{
		NetObject->InitNetObject(_ObjectID, NetInst);
	}

	return NetObject;
}



void NetworkManager::CreateLocalPlayer(class GameEngineLevel* _Level, int _ObjectID)
{
	//std::shared_ptr<BasePlayerActor> Player = nullptr;

	if (PlayerType::None == CharacterType)
	{
		MsgAssert("선택한 플레이어가 PlayerType::None입니다");
	}
	/*else if (PlayerType::Nero == CharacterType)
	{
		Player = _Level->CreateActor<PlayerActor_Nero>();
	}
	else if (PlayerType::Vergil == CharacterType)
	{
		Player = _Level->CreateActor<PlayerActor_Vergil>();
	}*/

	std::shared_ptr<NetTestPlayer> Player = nullptr;
	Player = _Level->CreateActor<NetTestPlayer>();
	Player->InitNetObject(_ObjectID, NetInst);
	Player->SetUserControllType();
}