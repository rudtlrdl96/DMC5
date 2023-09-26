#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include <GameEngineCore/GameEngineActor.h>

#include "ContentsEnum.h"

#include "NetworkGUI.h"
#include "BaseLevel.h"
#include "NetworkObjectBase.h"

#include "PacketEnum.h"
#include "MessageChatPacket.h"
#include "LinkObjectPacket.h"
#include "FsmChangePacket.h"

#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"
#include "Enemy_HellCaina.h"
#include "Player_MirageBlade.h"

const float NetworkManager::PacketFlushTime = 0.01f;

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
GameEngineSerializer NetworkManager::ChunkPacketBytes;

std::map<GameEngineLevel*, std::vector<BasePlayerActor*>> NetworkManager::AllPlayerActors;


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


void NetworkManager::PushChatPacket(const std::string_view& _Msg, const float4& _Color /*= float4::WHITE*/)
{
	std::shared_ptr<MessageChatPacket> ChatPacket = std::make_shared<MessageChatPacket>();
	ChatPacket->SetPacketID(PacketEnum::MessageChatPacket);
	ChatPacket->SetObjectID(NetID);
	ChatPacket->Message = _Msg;
	ChatPacket->Color = _Color;
	AllPacket[PacketEnum::MessageChatPacket].push_back(ChatPacket);
}



void NetworkManager::PushUpdatePacket(
	NetworkObjectBase* _ObjPtr, 
	float _TimeScale,
	const std::vector<int*>& _IntDatas,
	const std::vector<float*>& _FloatDatas,
	const std::vector<bool*>& _BoolDatas)
{
	if (nullptr == _ObjPtr)
	{
		MsgAssert("업데이트 패킷을 전송할땐 파라미터 인자의 ObjPtr의 값은 반드시 넣어주어야 합니다.");
		return;
	}

	GameEngineActor* ActorPtr = dynamic_cast<GameEngineActor*>(_ObjPtr);
	if (nullptr == ActorPtr)
	{
		MsgAssert("Update패킷을 전송할때 인자로 받은 ObjPtr이 GameEngineActor를 상속받지 않았습니다");
		return;
	}

	//네트워크 연결 상태가 아닌경우
	if (NetworkState::None == NowState)
		return;

	
	//현재 진행중인 레벨의 엑터들만 실행
	if (ActorPtr->GetLevel() != CurLevel)
		return;

	//인자로 받은 오브젝트의 네트워크용 오브젝트 아이디
	unsigned int ObjectID = _ObjPtr->GetNetObjectID();
	
	//아직 Init처리되지 않은 경우
	if (-1 == ObjectID)
		return;

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
	Net_ActorType NetType = _ObjPtr->GetNetObjectType();
	if (Net_ActorType::UNKNOWN == NetType)
	{
		MsgAssert("ObjectUpdate패킷을 보내려는 객체의 NetObjectType을 설정해주지 않았습니다");
		return;
	}

	UpdatePacket->ActorType = static_cast<unsigned int>(NetType);
	

	//레벨 타입
	BaseLevel* Level = dynamic_cast<BaseLevel*>(ActorPtr->GetLevel());
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

	GameEngineTransform* TransPtr = ActorPtr->GetTransform();

	//위치
	UpdatePacket->Rotation = TransPtr->GetWorldRotation();
	//위치
	UpdatePacket->Position = TransPtr->GetWorldPosition();
	//타임 크기
	UpdatePacket->TimeScale = _TimeScale;

	//파괴 여부
	UpdatePacket->IsDeath = ActorPtr->IsDeath();
	if (true == UpdatePacket->IsDeath)
	{
		//이 NetObject는 이제부터 전송/수신을 받지 않음
		_ObjPtr->NetDisconnect();
	}

	//On/Off 여부
	UpdatePacket->IsUpdate = ActorPtr->IsUpdate();

	ArrDataCopy(UpdatePacket->IntDatas, _IntDatas);
	ArrDataCopy(UpdatePacket->FloatDatas, _FloatDatas);
	ArrDataCopy(UpdatePacket->BoolDatas, _BoolDatas);
}


void NetworkManager::SendFsmChangePacket(NetworkObjectBase* _NetObjPtr, int _FsmState)
{
	if (nullptr == _NetObjPtr)
	{
		MsgAssert("FSM 변환 패킷을 전송할땐 파라미터 인자의 _NetObjPtr의 값은 반드시 넣어주어야 합니다.");
		return;
	}

	GameEngineActor* ActorPtr = dynamic_cast<GameEngineActor*>(_NetObjPtr);
	if (nullptr == ActorPtr)
	{
		MsgAssert("FSM 변환 패킷을 전송할땐 인자로 받은 _NetObjPtr이 GameEngineActor를 상속받지 않았습니다");
		return;
	}

	//네트워크 연결 상태가 아닌경우
	if (NetworkState::None == NowState)
		return;


	//현재 진행중인 레벨의 엑터들만 실행
	if (ActorPtr->GetLevel() != CurLevel)
		return;

	//인자로 받은 오브젝트의 네트워크용 오브젝트 아이디
	unsigned int ObjectID = _NetObjPtr->GetNetObjectID();

	//아직 Init처리되지 않은 경우
	if (-1 == ObjectID)
		return;

	if (false == GameEngineNetObject::IsNetObject(ObjectID))
	{
		MsgAssert(GameEngineString::ToString(ObjectID) + " ID를 가진 오브젝트가 존재하지 않는데, Fsm 변환 패킷을 사용하려고 했습니다");
		return;
	}

	if (NetControllType::PassiveControll == _NetObjPtr->GetControllType())
	{
		MsgAssert("패킷을 받아 처리되는 오브젝트가 Fsm변경 패킷을 보낼려고 했습니다");
		return;
	}

	//이미 Death처리되어서 동작되지 않는 오브젝트인 경우
	if (true == _NetObjPtr->IsNetDisconnected())
		return;

	
	std::shared_ptr<FsmChangePacket> Packet = nullptr;
	Packet = std::make_shared<FsmChangePacket>();

	Packet->SetObjectID(ObjectID);
	Packet->FsmState = _FsmState;
	Packet->NetID = NetID;

	//레벨 타입
	BaseLevel* Level = dynamic_cast<BaseLevel*>(ActorPtr->GetLevel());
	if (nullptr == Level)
	{
		MsgAssert("FSM 변환 패킷을 전송하려는 Actor가 BaseLevel을 상속받은 레벨에 존재하지 않습니다");
		return;
	}

	Net_LevelType LevelType = Level->GetNetLevelType();
	if (Net_LevelType::UNKNOWN == LevelType)
	{
		MsgAssert("FSM 변환 패킷을 전송하려는 Actor가 알 수 없는 레벨에 존재합니다.");
		return;
	}

	Packet->LevelType = LevelType;

	NetInst->SendPacket(Packet);
}


void NetworkManager::LinkNetwork(NetworkObjectBase* _NetObjPtr, BaseLevel* _Level /*= nullptr*/)
{
	//이미 서버와 연동된 경우에는 아래를 실행시키지 않음
	if (-1 != _NetObjPtr->GetNetObjectID())
		return;

	//연결시키려는 객체가 플레이어라면 자료구조에 보관
	RegistPlayer(_NetObjPtr, _Level);

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
	Net_ActorType NetType =  _NetObjPtr->GetNetObjectType();
	if (Net_ActorType::UNKNOWN == NetType)
	{
		MsgAssert("Net_ActorType를 설정해주지 않아서 네트워크와 연동할 수 없습니다");
		return;
	}

	LinkPacket->ActorType = static_cast<unsigned int>(NetType);
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
		for (const std::pair<unsigned int, std::shared_ptr<ObjectUpdatePacket>>& Pair : AllUpdatePacket)
		{
			std::shared_ptr<ObjectUpdatePacket> UpdatePacket = Pair.second;
			UpdatePacket->SerializePacket(ChunkPacketBytes);
		}

		AllUpdatePacket.clear();
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
			for (std::shared_ptr<GameEnginePacket> Packet : Packets)
			{
				Packet->SerializePacket(ChunkPacketBytes);
			}
			Packets.clear();
		}

		++PacketGroupStartIter;
	}

	
	if (0 == ChunkPacketBytes.GetWriteOffSet())
		return;

	NetInst->Send(ChunkPacketBytes.GetConstCharPtr(), ChunkPacketBytes.GetWriteOffSet());
	ChunkPacketBytes.Reset();
}










std::shared_ptr<NetworkObjectBase> NetworkManager::CreateNetActor(Net_ActorType _ActorType, BaseLevel* _Level /*= nullptr*/, int _ObjectID /*= -1*/)
{
	if (nullptr == CurLevel)
	{
		MsgAssert("NetwortManager의 CurLevel 포인터가 nullptr입니다");
	}

	BaseLevel* CreateLevel = _Level;
	if (nullptr == CreateLevel)
	{
		CreateLevel = CurLevel;
	}

	std::shared_ptr<NetworkObjectBase> NetObject = nullptr;
	switch (_ActorType)
	{
	case Net_ActorType::UNKNOWN:
	{
		MsgAssert("컨텐츠 쪽에서 Net_ActorType을 설정해주지 않아 객체를 만들수 없습니다.");
		break;
	}
	case Net_ActorType::Nero:
		NetObject = CreateLevel->CreateActor<PlayerActor_Nero>();
		break;
	case Net_ActorType::Vergil:
		NetObject = CreateLevel->CreateActor<PlayerActor_Vergil>();
		break;
	case Net_ActorType::MirageBlade:
		NetObject = CreateLevel->CreateActor<Player_MirageBlade>();
		break;
	case Net_ActorType::HellCaina:
		NetObject = CreateLevel->CreateActor<Enemy_HellCaina>();
		break;
	default:
	{
		MsgAssert("NetworkManager에서 생성할 수 없는 ActorType이 들어왔습니다");
		return nullptr;
	}
	}

	//생성한 객체가 플레이어라면 자료구조에 저장
	RegistPlayer(NetObject.get(), CreateLevel);

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


void NetworkManager::RegistPlayer(NetworkObjectBase* _NetObjPtr, GameEngineLevel* _Level)
{
	BasePlayerActor* PlayerPtr = dynamic_cast<BasePlayerActor*>(_NetObjPtr);
	if (nullptr == PlayerPtr)
		return;

	if (nullptr == _Level)
	{
		MsgAssert("[서버 오류] : Player를 알 수 없는 레벨에 생성하려고 했습니다");
		return;
	}

	std::vector<BasePlayerActor*>& PlayerGroup = AllPlayerActors[_Level];
	std::vector<BasePlayerActor*>::iterator FindIter;

	FindIter = std::find(PlayerGroup.begin(), PlayerGroup.end(), PlayerPtr);
	if (PlayerGroup.end() != FindIter)
	{
		MsgAssert("[서버오류] : 해당 그룹에 이미 존재하는 플레이어를 또 넣으려고 했습니다");
		return;
	}

	PlayerGroup.push_back(PlayerPtr);
}

const std::vector<BasePlayerActor*>& NetworkManager::GetPlayers(GameEngineLevel* _Level /*= nullptr*/)
{
	GameEngineLevel* Level = _Level;
	if (nullptr == Level)
	{
		Level = CurLevel;
	}

	return AllPlayerActors[Level];
}