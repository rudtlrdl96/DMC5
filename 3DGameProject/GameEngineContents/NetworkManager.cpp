#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include <GameEngineCore/GameEngineLevel.h>

#include "ContentsEnum.h"
#include "PacketEnum.h"

#include "ObjectUpdatePacket.h"
#include "CreateObjectPacket.h"

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




void NetworkManager::SendUpdatePacket(GameEngineNetObject* _NetObj, GameEngineTransform* TransPtr, float _TimeScale)
{
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

	//위치
	UpdatePacket->Rotation = TransPtr->GetWorldRotation();
	//위치
	UpdatePacket->Position = TransPtr->GetWorldPosition();
	//타임 크기
	UpdatePacket->TimeScale = _TimeScale;

	//자료구조에 저장
	AllUpdatePacket[ObjectID] = UpdatePacket;
}


void NetworkManager::FlushUpdatePacket()
{
	for (const std::pair<unsigned int, std::shared_ptr<ObjectUpdatePacket>>& Pair : AllUpdatePacket)
	{
		std::shared_ptr<ObjectUpdatePacket> UpdatePacket = Pair.second;
		NetInst->SendPacket(UpdatePacket);
	}

	AllUpdatePacket.clear();
}


void NetworkManager::SendCreatePacket(Net_ActorType _Type, const float4& _Position /*= float4::ZERO*/, const float4& _Rotation /*= float4::ZERO*/)
{
	std::shared_ptr<GameEngineNetObject> NewObject = nullptr;
	if (true == IsServerValue)
	{
		NewObject = CreateNetActor(_Type);
		return;
	}

	std::shared_ptr<CreateObjectPacket> CreatePacket = std::make_shared<CreateObjectPacket>();
	CreatePacket->ActorType = static_cast<unsigned int>(_Type);
	CreatePacket->Position = _Position;
	CreatePacket->Rotation = _Rotation;

	NetInst->SendPacket(CreatePacket);
}



#include "Player.h"
#include "Enemy_Empusa.h"

std::shared_ptr<GameEngineNetObject> NetworkManager::CreateNetActor(Net_ActorType _ActorType, int _ObjectID /*= -1*/)
{
	std::shared_ptr<GameEngineNetObject> NetObject = nullptr;
	switch (_ActorType)
	{
	case Net_ActorType::Nero:
		NetObject = GetLevel()->CreateActor<Enemy_Empusa>();	//이거 나중에 꼭 바꿀것
		break;
	case Net_ActorType::Vergil:
		NetObject = GetLevel()->CreateActor<Enemy_Empusa>();	//이거 나중에 꼭 바꿀것
		break;
	default:
	{
		MsgAssert("NetworkManager에서 생성할 수 없는 ActorType이 들어왔습니다");
		return nullptr;
	}
	}


	//서버쪽에서 오브젝트아이디를 직접 만드는 경우
	if ((true == IsServerValue) && (-1 == _ObjectID))
	{
		int ID = GameEngineNetObject::CreateServerID();
		NetObject->InitNetObject(ID, NetInst);
	}

	//클라쪽에서 오브젝트 아이디를 직접 만드는 경우
	else if ((true == IsClientValue) && (-1 != _ObjectID))
	{
		NetObject->InitNetObject(_ObjectID, NetInst);
	}

	return NetObject;
}