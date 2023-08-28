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

unsigned int NetworkManager::LinkID = 0;
std::map<unsigned int, class GameEngineNetObject*> NetworkManager::AllLinkObject;




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




void NetworkManager::SendUpdatePacket(GameEngineNetObject* _NetObj, GameEngineActor* _ActorPtr, float _TimeScale /*= 1.f*/)
{
	//���� �������� ������ ���͵鸸 ����
	if (_ActorPtr->GetLevel() != CurLevel)
		return;

	unsigned int ObjectID = _NetObj->GetNetObjectID();
	if (false == GameEngineNetObject::IsNetObject(ObjectID))
	{
		MsgAssert(GameEngineString::ToString(ObjectID) + " ID�� ���� ������Ʈ�� �������� �ʴµ�, UpdatePacket�� ����Ϸ��� �߽��ϴ�");
		return;
	}


	std::shared_ptr<ObjectUpdatePacket> UpdatePacket = nullptr;
	//�̹� �ش� ID�� UpdatePacket�� �����ߴٸ�
	if (true == AllUpdatePacket.contains(ObjectID))
	{
		UpdatePacket = AllUpdatePacket[ObjectID];
	}
	//ObjectID �̹��� ó�� ����� ObjectUpdatePacket�� ���
	else
	{
		UpdatePacket = std::make_shared<ObjectUpdatePacket>();
		AllUpdatePacket[ObjectID] = UpdatePacket;
	}
	

	//��Ŷ ���̵�
	//������

	//������Ʈ ���̵�
	UpdatePacket->SetObjectID(ObjectID);
	//������Ʈ Ÿ��
	UpdatePacket->ActorType = _NetObj->GetNetObjectType();


	GameEngineTransform* TransPtr = _ActorPtr->GetTransform();

	//��ġ
	UpdatePacket->Rotation = TransPtr->GetWorldRotation();
	//��ġ
	UpdatePacket->Position = TransPtr->GetWorldPosition();
	//Ÿ�� ũ��
	UpdatePacket->TimeScale = _TimeScale;

	//�ı� ����
	UpdatePacket->IsDeath = _ActorPtr->IsDeath();
	if (true == UpdatePacket->IsDeath)
	{
		//�� NetObject�� �������� ����/������ ���� ����
		_NetObj->NetDisconnect();
	}
}


void NetworkManager::FlushUpdatePacket()
{
	if (true == AllUpdatePacket.empty())
		return;

	//static GameEngineSerializer Ser;

	//int PacketSize = 0;
	//int Count = 0;


	//��Ŷ ��� ������
	for (const std::pair<unsigned int, std::shared_ptr<ObjectUpdatePacket>>& Pair : AllUpdatePacket)
	{
		std::shared_ptr<ObjectUpdatePacket> UpdatePacket = Pair.second;
		NetInst->SendPacket(UpdatePacket);
		//UpdatePacket->SerializePacket(Ser); //�� �ڵ��� ������ Size�� ǥ���ϴ� �κ��� �ٲ��� ����
		
		/*if (0 == PacketSize)
		{
			unsigned char* Ptr = Ser.GetDataPtr();
			memcpy_s(&PacketSize, sizeof(int), &Ptr[4], sizeof(int));
			continue;
		}
		
		unsigned char* SizePtr = Ser.GetDataPtr();
		int SizePos = (PacketSize * Count++) + 4;
		memcpy_s(&SizePtr[SizePos], sizeof(int), &PacketSize, sizeof(int));*/
	}

	//NetInst->Send(Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
	//NetInst->Send(reinterpret_cast<const char*>(Ser.GetDataPtr()), Ser.GetWriteOffSet());
	AllUpdatePacket.clear();
	//Ser.Reset();
}


void NetworkManager::SendCreatePacket(Net_ActorType _Type, const float4& _Position /*= float4::ZERO*/, const float4& _Rotation /*= float4::ZERO*/)
{
	std::shared_ptr<GameEngineNetObject> NewObject = nullptr;

	//�����϶� �ٷ� UserControll���� ������Ʈ ����
	if (true == IsServerValue)
	{
		NewObject = CreateNetActor(_Type);
		NewObject->SetUserControllType();
		return;
	}

	//�׷��� �����غ��ϱ� Ŭ�� �������� ���� �����޶�� ��û�� ���� �����?
	//���� ó���� �� ������ ���ٵ�?

	//Ŭ���϶� ������ ��Ź�ϴ� ��Ŷ�� ����� ������ ����
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

	//������ ��� �׳� ����� ��
	if (true == IsServerValue)
	{
		_NetObjPtr->InitNetObject(GameEngineNetObject::CreateServerID(), NetInst);
		_NetObjPtr->SetUserControllType();
		return;
	}

	//Ŭ���� ���
	std::shared_ptr<LinkObjectPacket> LinkPacket = std::make_shared<LinkObjectPacket>();
	//�� Ŭ���� ��Ʈ��ũ ���̵�
	LinkPacket->SetObjectID(NetID);
	//������ ��û�� ��ü�� Ÿ��(���������� �� ��ü�� ������ �ϱ� ����)
	LinkPacket->ActorType = _NetObjPtr->GetNetObjectType();

	//�ڷᱸ���� ���� ��ũ ���̵�(�������� �ٽ� ���Ź����� � ��ü�� InitNetObject�ؾ��ϴ��� �˱�����)
	LinkPacket->LinkID = LinkID;
	//�ڷᱸ���� ����
	AllLinkObject[LinkID++] = _NetObjPtr;

	NetInst->SendPacket(LinkPacket);
}



#include "NetTestPlayer.h"

std::shared_ptr<GameEngineNetObject> NetworkManager::CreateNetActor(Net_ActorType _ActorType, int _ObjectID /*= -1*/)
{
	std::shared_ptr<GameEngineNetObject> NetObject = nullptr;
	switch (_ActorType)
	{
	case Net_ActorType::Nero:
		NetObject = GetLevel()->CreateActor<NetTestPlayer>();	//�̰� ���߿� �� �ٲܰ�
		break;
	case Net_ActorType::Vergil:
		NetObject = GetLevel()->CreateActor<NetTestPlayer>();	//�̰� ���߿� �� �ٲܰ�
		break;
	default:
	{
		MsgAssert("NetworkManager���� ������ �� ���� ActorType�� ���Խ��ϴ�");
		return nullptr;
	}
	}


	//�����ʿ��� ������Ʈ���̵� ���� ����� ���
	if (true == IsServerValue)
	{
		int ID = _ObjectID;
		if (-1 == ID)
		{
			ID = GameEngineNetObject::CreateServerID();
		}

		NetObject->InitNetObject(ID, NetInst);
	}

	//Ŭ���ʿ��� ������Ʈ ���̵� ���� ����� ���
	else if ((true == IsClientValue) && (-1 != _ObjectID))
	{
		NetObject->InitNetObject(_ObjectID, NetInst);
	}

	return NetObject;
}