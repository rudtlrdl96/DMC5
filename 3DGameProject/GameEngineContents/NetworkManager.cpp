#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineActor.h>

#include "ContentsEnum.h"
#include "PacketEnum.h"

#include "ObjectUpdatePacket.h"
#include "CreateObjectPacket.h"
#include "LinkObjectPacket.h"

GameEngineNet* NetworkManager::NetInst = nullptr;
GameEngineNetServer NetworkManager::ServerInst;
GameEngineNetClient NetworkManager::ClientInst;

bool NetworkManager::IsServerValue = false;
bool NetworkManager::IsClientValue = false;

unsigned int NetworkManager::NetID = 0;
GameEngineLevel* NetworkManager::CurLevel = nullptr;

std::map<unsigned int, std::shared_ptr<ObjectUpdatePacket>> NetworkManager::AllUpdatePacket;





void NetworkManager::ServerOpen(int _Port)
{
	//Thread 이름 설정
	SetThreadDescription(GetCurrentThread(), L"Server Main Thread");

	NetInst = &ServerInst;
	
	//클라이언트가 Accept될 때 처리해야 할 작업 등록
	ServerInst.SetAcceptCallBack(AcceptCallback);
	
	//서버용 패킷 처리 콜백 등록
	ServerPacketInit();

	//서버 오픈
	ServerInst.ServerOpen(static_cast<unsigned short>(_Port));
	NetID = GameEngineNetObject::CreateServerID();
	IsServerValue = true;
}


bool NetworkManager::ConnectServer(const std::string_view& _IP, int _Port)
{
	//Thread 이름 설정
	SetThreadDescription(GetCurrentThread(), L"Client Main Thread");

	NetInst = &ClientInst;

	//클라용 패킷 처리 콜백 등록
	ClientPacketInit();

	//서버 접속
	IsClientValue = ClientInst.Connect(_IP.data(), static_cast<unsigned short>(_Port));
	return IsClientValue;
}




void NetworkManager::SendUpdatePacket(GameEngineNetObject* _NetObj, GameEngineActor* _ActorPtr, float _TimeScale /*= 1.f*/)
{
	//현재 진행중인 레벨의 엑터들만 실행
	if (_ActorPtr->GetLevel() != CurLevel)
		return;

	unsigned int ObjectID = _NetObj->GetNetObjectID();
	if (false == GameEngineNetObject::IsNetObject(ObjectID))
	{
		MsgAssert(GameEngineString::ToString(ObjectID) + " ID를 가진 오브젝트가 존재하지 않는데, UpdatePacket을 사용하려고 했습니다");
		return;
	}
	
	std::shared_ptr<ObjectUpdatePacket> UpdatePacket = std::make_shared<ObjectUpdatePacket>();

	//패킷 아이디
	//사이즈

	//오브젝트 아이디
	UpdatePacket->SetObjectID(ObjectID);
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

	//자료구조에 저장
	AllUpdatePacket[ObjectID] = UpdatePacket;
}


void NetworkManager::FlushUpdatePacket()
{
	if (true == AllUpdatePacket.empty())
		return;

	static GameEngineSerializer Ser;

	int PacketSize = 0;
	int Count = 0;


	//패킷 모아 보내기
	for (const std::pair<unsigned int, std::shared_ptr<ObjectUpdatePacket>>& Pair : AllUpdatePacket)
	{
		std::shared_ptr<ObjectUpdatePacket> UpdatePacket = Pair.second;
		//NetInst->SendPacket(UpdatePacket);
		UpdatePacket->SerializePacket(Ser); //이 코드의 문제점 Size를 표현하는 부분을 바꾸지 못함
		
		if (0 == PacketSize)
		{
			unsigned char* Ptr = Ser.GetDataPtr();
			memcpy_s(&PacketSize, sizeof(int), &Ptr[4], sizeof(int));
			continue;
		}
		
		unsigned char* SizePtr = Ser.GetDataPtr();
		int SizePos = (PacketSize * Count++) + 4;
		memcpy_s(&SizePtr[SizePos], sizeof(int), &PacketSize, sizeof(int));
	}

	//NetInst->Send(Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
	NetInst->Send(reinterpret_cast<const char*>(Ser.GetDataPtr()), Ser.GetWriteOffSet());
	AllUpdatePacket.clear();
	Ser.Reset();
}


void NetworkManager::SendCreatePacket(Net_ActorType _Type, const float4& _Position /*= float4::ZERO*/, const float4& _Rotation /*= float4::ZERO*/)
{
	std::shared_ptr<GameEngineNetObject> NewObject = nullptr;

	//서버일땐 바로 UserControll모드로 오브젝트 생성
	if (true == IsServerValue)
	{
		NewObject = CreateNetActor(_Type);
		NewObject->SetUserControllType();
		return;
	}

	//그런데 생각해보니까 클라가 서버한테 뭔가 만들어달라고 요청할 일이 생길까?
	//보통 처리를 다 서버가 할텐데?

	//클라일땐 생성을 부탁하는 패킷을 만들어 서버로 전송
	std::shared_ptr<CreateObjectPacket> CreatePacket = std::make_shared<CreateObjectPacket>();
	CreatePacket->ActorType = static_cast<unsigned int>(_Type);
	CreatePacket->Position = _Position;
	CreatePacket->Rotation = _Rotation;
	NetInst->SendPacket(CreatePacket);
}


void NetworkManager::LinkNetwork(GameEngineNetObject* _NetObjPtr)
{
	if (nullptr == NetInst)
		return;

	if (true == IsServerValue)
	{
		_NetObjPtr->InitNetObject(GameEngineNetObject::CreateServerID(), NetInst);
		_NetObjPtr->SetUserControllType();
		return;
	}

	std::shared_ptr<LinkObjectPacket> LinkPacket = std::make_shared<LinkObjectPacket>();
	LinkPacket->SetObjectID(NetID);
	LinkPacket->ActorType = _NetObjPtr->GetNetObjectType();
	LinkPacket->Ptr = reinterpret_cast<unsigned __int64>(_NetObjPtr);

	NetInst->SendPacket(LinkPacket);
}



#include "NetTestPlayer.h"

std::shared_ptr<GameEngineNetObject> NetworkManager::CreateNetActor(Net_ActorType _ActorType, int _ObjectID /*= -1*/)
{
	std::shared_ptr<GameEngineNetObject> NetObject = nullptr;
	switch (_ActorType)
	{
	case Net_ActorType::Nero:
		NetObject = GetLevel()->CreateActor<NetTestPlayer>();	//이거 나중에 꼭 바꿀것
		break;
	case Net_ActorType::Vergil:
		NetObject = GetLevel()->CreateActor<NetTestPlayer>();	//이거 나중에 꼭 바꿀것
		break;
	default:
	{
		MsgAssert("NetworkManager에서 생성할 수 없는 ActorType이 들어왔습니다");
		return nullptr;
	}
	}


	//서버쪽에서 오브젝트아이디를 직접 만드는 경우
	if (true == IsServerValue)
	{
		int ID = _ObjectID;
		if (-1 == ID)
		{
			ID = GameEngineNetObject::CreateServerID();
		}

		NetObject->InitNetObject(ID, NetInst);
	}

	//클라쪽에서 오브젝트 아이디를 직접 만드는 경우
	else if ((true == IsClientValue) && (-1 != _ObjectID))
	{
		NetObject->InitNetObject(_ObjectID, NetInst);
	}

	return NetObject;
}