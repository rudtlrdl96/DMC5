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
	//Thread �̸� ����
	SetThreadDescription(GetCurrentThread(), L"Server Main Thread");

	NetInst = &ServerInst;
	
	//Ŭ���̾�Ʈ�� Accept�� �� ó���ؾ� �� �۾� ���
	ServerInst.SetAcceptCallBack(AcceptCallback);
	
	//������ ��Ŷ ó�� �ݹ� ���
	ServerPacketInit();

	//���� ����
	ServerInst.ServerOpen(static_cast<unsigned short>(_Port));
	NetID = GameEngineNetObject::CreateServerID();
	IsServerValue = true;
}


bool NetworkManager::ConnectServer(const std::string_view& _IP, int _Port)
{
	//Thread �̸� ����
	SetThreadDescription(GetCurrentThread(), L"Client Main Thread");

	NetInst = &ClientInst;

	//Ŭ��� ��Ŷ ó�� �ݹ� ���
	ClientPacketInit();

	//���� ����
	IsClientValue = ClientInst.Connect(_IP.data(), static_cast<unsigned short>(_Port));
	return IsClientValue;
}




void NetworkManager::SendUpdatePacket(GameEngineNetObject* _NetObj, GameEngineTransform* TransPtr, float _TimeScale)
{
	unsigned int ObjectID = _NetObj->GetNetObjectID();
	if (false == GameEngineNetObject::IsNetObject(ObjectID))
	{
		MsgAssert(GameEngineString::ToString(ObjectID) + " ID�� ���� ������Ʈ�� �������� �ʴµ�, UpdatePacket�� ����Ϸ��� �߽��ϴ�");
		return;
	}
	
	std::shared_ptr<ObjectUpdatePacket> UpdatePacket = std::make_shared<ObjectUpdatePacket>();

	//��Ŷ ���̵�
	//������

	//������Ʈ ���̵�
	UpdatePacket->SetObjectID(ObjectID);
	//������Ʈ Ÿ��
	UpdatePacket->ActorType = _NetObj->GetNetObjectType();

	//��ġ
	UpdatePacket->Rotation = TransPtr->GetWorldRotation();
	//��ġ
	UpdatePacket->Position = TransPtr->GetWorldPosition();
	//Ÿ�� ũ��
	UpdatePacket->TimeScale = _TimeScale;

	//�ڷᱸ���� ����
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

std::shared_ptr<GameEngineNetObject> NetworkManager::CreateNetActor(Net_ActorType _ActorType, int _ObjectID /*= -1*/)
{
	std::shared_ptr<GameEngineNetObject> NetObject = nullptr;
	switch (_ActorType)
	{
	case Net_ActorType::Nero:
		NetObject = GetLevel()->CreateActor<Player>();	//�̰� ���߿� �� �ٲܰ�
		break;
	case Net_ActorType::Vergil:
		NetObject = GetLevel()->CreateActor<Player>();	//�̰� ���߿� �� �ٲܰ�
		break;
	default:
	{
		MsgAssert("NetworkManager���� ������ �� ���� ActorType�� ���Խ��ϴ�");
		return nullptr;
	}
	}


	//�����ʿ��� ������Ʈ���̵� ���� ����� ���
	if ((true == IsServerValue) && (-1 == _ObjectID))
	{
		int ID = GameEngineNetObject::CreateServerID();
		NetObject->InitNetObject(ID, NetInst);
	}

	//Ŭ���ʿ��� ������Ʈ ���̵� ���� ����� ���
	else if ((true == IsClientValue) && (-1 != _ObjectID))
	{
		NetObject->InitNetObject(_ObjectID, NetInst);
	}

	return NetObject;
}