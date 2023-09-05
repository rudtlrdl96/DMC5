#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include <GameEngineCore/GameEngineActor.h>

#include "NetworkGUI.h"
#include "ContentsEnum.h"
#include "BaseLevel.h"

#include "PacketEnum.h"
#include "ObjectUpdatePacket.h"
#include "MessageChatPacket.h"
#include "LinkObjectPacket.h"

#include "NetworkTestLevel.h"
#include "StartStageLevel.h"
#include "BossStageLevel.h"

#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"
#include "NetTestPlayer.h"


unsigned int NetworkManager::NetID = 0;

GameEngineNet* NetworkManager::NetInst = nullptr;
GameEngineNetServer NetworkManager::ServerInst;
GameEngineNetClient NetworkManager::ClientInst;

NetworkManager::NetworkState NetworkManager::NowState = NetworkManager::NetworkState::None;

std::function<void(unsigned int)> NetworkManager::ConnectCallBack = nullptr;
BaseLevel* NetworkManager::CurLevel = nullptr;
Net_LevelType NetworkManager::CurLevelType = Net_LevelType::UNKNOWN;

std::map<unsigned int, std::shared_ptr<ObjectUpdatePacket>> NetworkManager::AllUpdatePacket;

std::map<PacketEnum, std::vector<std::shared_ptr<GameEnginePacket>>> NetworkManager::AllPacket;
std::map<PacketEnum, GameEngineSerializer> NetworkManager::ChunkPackets;
GameEngineSerializer NetworkManager::ChunkBytes;




unsigned int NetworkManager::ServerOpen(int _Port)
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

	return NetID;
}




void NetworkManager::ConnectServer(const std::string& _IP, int PortNum, std::function<void(unsigned int)> _ConnectCallBack)
{
	//ConnectID 패킷을 받았을때 처리할 콜백함수 등록
	ConnectCallBack = _ConnectCallBack;

	//Thread 이름 설정
	SetThreadDescription(GetCurrentThread(), L"Client Main Thread");
	NetInst = &ClientInst;

	//클라용 패킷 처리 콜백 등록
	ClientPacketInit();

	//서버 접속
	bool IsResult = ClientInst.Connect(_IP, static_cast<unsigned short>(PortNum));
	if (true == IsResult)
	{
		NowState = NetworkState::Client;
		return;
	}

	//서버 접속 실패한 경우
	std::string ErrorIP = _IP.data();
	MsgAssert("이 클라이언트가 서버 접속에 실패하였습니다.\n접속을 시도한 IP : " + ErrorIP);
}



void NetworkManager::Update_PacketProcess(BaseLevel* _CurrentLevel)
{
	//레벨 저장(패킷 처리할때 사용됨)
	CurLevel = _CurrentLevel;
	CurLevelType = CurLevel->GetNetLevelType();

	if (nullptr == NetInst)
		return;

	NetInst->UpdatePacket();
}


void NetworkManager::PushChatPacket(const std::string_view& _Msg)
{
	std::shared_ptr<MessageChatPacket> ChatPacket = std::make_shared<MessageChatPacket>();
	ChatPacket->SetPacketID(PacketEnum::MessageChatPacket);
	ChatPacket->SetObjectID(NetID);
	ChatPacket->Message = _Msg;
	AllPacket[PacketEnum::MessageChatPacket].push_back(ChatPacket);
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
	if (-1 == UpdatePacket->ActorType)
	{
		MsgAssert("ObjectUpdate패킷을 보내려는 객체의 NetObjectType을 설정해주지 않았습니다");
		return;
	}

	//레벨 타입
	BaseLevel* Level = dynamic_cast<BaseLevel*>(_ActorPtr->GetLevel());
	if (nullptr == Level)
	{
		MsgAssert("ObjectUpdate패킷을 전송하려는 Actor가 BaseLevel을 상속받은 레벨에 존재하지 않습니다");
		return;
	}

	Net_LevelType LevelType = Level->GetNetLevelType();
	if (Net_LevelType::UNKNOWN == LevelType)
	{
		MsgAssert("ObjectUpdate패킷을 전송하려는 Actor가 알 수 없는 레벨에 존재합니다.");
		return;
	}

	UpdatePacket->LevelType = LevelType;

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


void NetworkManager::LinkNetwork(GameEngineNetObject* _NetObjPtr)
{
	//이미 서버와 연동된 경우에는 아래를 실행시키지 않음
	if (-1 != _NetObjPtr->GetNetObjectID())
		return;

	//싱글모드 일때
	if (nullptr == NetInst)
	{
		_NetObjPtr->SetUserControllType();
		return;
	}

	//서버인 경우엔 해당 엑터를 바로 서버 연동 및 유저 컨트롤
	if (true == IsServer())
	{
		_NetObjPtr->InitNetObject(GameEngineNetObject::CreateServerID(), NetInst);
		_NetObjPtr->SetUserControllType();
		return;
	}

	//클라인 경우엔 서버에 생성 요청 패킷을 생성
	std::shared_ptr<LinkObjectPacket> LinkPacket = std::make_shared<LinkObjectPacket>();
	LinkPacket->SetObjectID(NetID);
	LinkPacket->ActorType = _NetObjPtr->GetNetObjectType();
	LinkPacket->Ptr = reinterpret_cast<unsigned __int64>(_NetObjPtr);

	AllPacket[PacketEnum::LinkObjectPacket].push_back(LinkPacket);
}


void NetworkManager::FlushPackets()
{
	if (nullptr == NetInst)
		return;

	//업데이트 패킷 직렬화
	if (false == AllUpdatePacket.empty())
	{
		std::vector <std::shared_ptr<GameEnginePacket>>& UpdataPackets = AllPacket[PacketEnum::ObjectUpdatePacket];
		UpdataPackets.reserve(AllUpdatePacket.size());

		for (const std::pair<unsigned int, std::shared_ptr<ObjectUpdatePacket>>& Pair : AllUpdatePacket)
		{
			UpdataPackets.push_back(Pair.second);
		}

		SerializePackets(UpdataPackets, ChunkPackets[PacketEnum::ObjectUpdatePacket]);
		AllUpdatePacket.clear();
		UpdataPackets.clear();
	}


	//모든 패킷 직렬화, 직렬화 결과는 ChunkPackets에 쌓이게 됨
	auto PacketGroupStartIter = AllPacket.begin();
	auto PacketGroupEndIter = AllPacket.end();
	while (PacketGroupStartIter != PacketGroupEndIter)
	{
		PacketEnum Type = PacketGroupStartIter->first;
		std::vector<std::shared_ptr<GameEnginePacket>>& Packets = PacketGroupStartIter->second;

		if (false == Packets.empty())
		{
			SerializePackets(Packets, ChunkPackets[Type]);
			Packets.clear();
		}

		++PacketGroupStartIter;
	}


	
	//각 패킷의 직렬화 결과인 ChunkPackets들을 하나의 바이트인 ChunkBytes에 옮긴다
	auto PacketByteStartIter = ChunkPackets.begin();
	auto PacketByteEndIter = ChunkPackets.end();
	while (PacketByteStartIter != PacketByteEndIter)
	{
		GameEngineSerializer& Ser = PacketByteStartIter->second;
		if (0 < Ser.GetWriteOffSet())
		{
			ChunkBytes << Ser;
			Ser.Reset();
		}

		++PacketByteStartIter;
	}


	if (0 == ChunkBytes.GetWriteOffSet())
		return;

	NetInst->Send(ChunkBytes.GetConstCharPtr(), ChunkBytes.GetWriteOffSet());
	ChunkBytes.Reset();
}



void NetworkManager::SerializePackets(const std::vector<std::shared_ptr<GameEnginePacket>>& _Packets, GameEngineSerializer& _Ser)
{
	_Ser.Reset();
	int PacketSize = 0;

	for (size_t i = 0; i < _Packets.size(); ++i)
	{
		_Packets[i]->SerializePacket(_Ser);

		//처음으로 패킷 사이즈를 찾을때
		if (0 == PacketSize)
		{
			unsigned char* Ptr = _Ser.GetDataPtr();
			memcpy_s(&PacketSize, sizeof(int), &Ptr[4], sizeof(int));
			continue;
		}

		//패킷을 직렬화 할 때 마다 Size위치의 값을 수정
		unsigned char* SizePtr = _Ser.GetDataPtr();
		size_t SizePos = (static_cast<size_t>(PacketSize) * i) + 4;
		memcpy_s(&SizePtr[SizePos], sizeof(int), &PacketSize, sizeof(int));
	}
}







std::shared_ptr<GameEngineNetObject> NetworkManager::CreateNetActor(Net_ActorType _ActorType, int _ObjectID /*= -1*/)
{
	if (nullptr == CurLevel)
	{
		MsgAssert("NetwortManager의 CurLevel 포인터가 nullptr입니다");
	}

	std::shared_ptr<GameEngineNetObject> NetObject = nullptr;
	switch (_ActorType)
	{
	case Net_ActorType::TestPlayer:
		NetObject = CurLevel->CreateActor<NetTestPlayer>();
		break;
	case Net_ActorType::Nero:
		NetObject = CurLevel->CreateActor<PlayerActor_Nero>();
		break;
	case Net_ActorType::Vergil:
		NetObject = CurLevel->CreateActor<PlayerActor_Vergil>();
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
