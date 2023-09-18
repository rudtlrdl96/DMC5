#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include <GameEngineCore/GameEngineActor.h>

#include "NetworkGUI.h"
#include "ContentsEnum.h"
#include "BaseLevel.h"

#include "PacketEnum.h"
#include "MessageChatPacket.h"
#include "LinkObjectPacket.h"
#include "FsmChangePacket.h"

#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"
#include "NetTestPlayer.h"

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
std::map<PacketEnum, GameEngineSerializer> NetworkManager::ChunkPackets;
GameEngineSerializer NetworkManager::ChunkBytes;



unsigned int NetworkManager::ServerOpen(int _Port)
{
	//Thread �̸� ����
	SetThreadDescription(GetCurrentThread(), L"Server Main Thread");

	NetInst = &ServerInst;
	
	//Ŭ���̾�Ʈ�� Accept�� �� ó���ؾ� �� �۾� ���
	ServerInst.SetAcceptCallBack(AcceptCallback);
	
	//������ ��Ŷ ó�� �ݹ� ���
	ServerPacketInit();

	//������ ��� ID�� 1��
	NetID = GameEngineNetObject::CreateServerID();

	//���� ����
	ServerInst.ServerOpen(static_cast<unsigned short>(_Port));
	NowState = NetworkState::Server;

	return NetID;
}




void NetworkManager::ConnectServer(const std::string& _IP, int PortNum, std::function<void(unsigned int)> _ConnectCallBack)
{
	//ConnectID ��Ŷ�� �޾����� ó���� �ݹ��Լ� ���
	ConnectCallBack = _ConnectCallBack;

	//Thread �̸� ����
	SetThreadDescription(GetCurrentThread(), L"Client Main Thread");
	NetInst = &ClientInst;

	//Ŭ��� ��Ŷ ó�� �ݹ� ���
	ClientPacketInit();

	//���� ����
	bool IsResult = ClientInst.Connect(_IP, static_cast<unsigned short>(PortNum));
	if (true == IsResult)
	{
		NowState = NetworkState::Client;
		return;
	}

	//���� ���� ������ ���
	std::string ErrorIP = _IP.data();
	MsgAssert("�� Ŭ���̾�Ʈ�� ���� ���ӿ� �����Ͽ����ϴ�.\n������ �õ��� IP : " + ErrorIP);
}



void NetworkManager::Update_PacketProcess(BaseLevel* _CurrentLevel)
{
	//���� ����(��Ŷ ó���Ҷ� ����)
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



void NetworkManager::PushUpdatePacket(const UpdatePacketParameter& _Param)
{
	GameEngineNetObject* NetPtr = _Param.ObjPtr;
	if (nullptr == NetPtr)
	{
		MsgAssert("������Ʈ ��Ŷ�� �����Ҷ� �Ķ���� ������ ObjPtr�� ���� �ݵ�� �־��־�� �մϴ�.");
		return;
	}

	GameEngineActor* ActorPtr = dynamic_cast<GameEngineActor*>(NetPtr);
	if (nullptr == ActorPtr)
	{
		MsgAssert("Update��Ŷ�� �����Ҷ� ���ڷ� ���� ObjPtr�� GameEngineActor�� ��ӹ��� �ʾҽ��ϴ�");
		return;
	}

	//��Ʈ��ũ ���� ���°� �ƴѰ��
	if (NetworkState::None == NowState)
		return;

	
	//���� �������� ������ ���͵鸸 ����
	if (ActorPtr->GetLevel() != CurLevel)
		return;

	//���ڷ� ���� ������Ʈ�� ��Ʈ��ũ�� ������Ʈ ���̵�
	unsigned int ObjectID = NetPtr->GetNetObjectID();
	
	//���� Initó������ ���� ���
	if (-1 == ObjectID)
		return;

	if (false == GameEngineNetObject::IsNetObject(ObjectID))
	{
		MsgAssert(GameEngineString::ToString(ObjectID) + " ID�� ���� ������Ʈ�� �������� �ʴµ�, UpdatePacket�� ����Ϸ��� �߽��ϴ�");
		return;
	}

	//������Ʈ ��Ŷ �����
	std::shared_ptr<ObjectUpdatePacket> UpdatePacket = nullptr;

	//�̹� �ش� ID�� UpdatePacket�� �����ߴٸ� ��Ȱ��
	if (true == AllUpdatePacket.contains(ObjectID))
	{
		UpdatePacket = AllUpdatePacket[ObjectID];
	}

	//ObjectID �̹��� ó�� ����� ObjectUpdatePacket�� ��� ���� �����Ҵ�
	else
	{
		UpdatePacket = std::make_shared<ObjectUpdatePacket>();
		AllUpdatePacket[ObjectID] = UpdatePacket;
	}
	

	//��Ŷ ���̵�
	//������

	//������Ʈ ���̵�
	UpdatePacket->SetObjectID(ObjectID);
	//��Ʈ��ũ ���̵�
	UpdatePacket->NetID = NetID;

	//������Ʈ Ÿ��
	UpdatePacket->ActorType = NetPtr->GetNetObjectType();
	if (-1 == UpdatePacket->ActorType)
	{
		MsgAssert("ObjectUpdate��Ŷ�� �������� ��ü�� NetObjectType�� ���������� �ʾҽ��ϴ�");
		return;
	}

	//���� Ÿ��
	BaseLevel* Level = dynamic_cast<BaseLevel*>(ActorPtr->GetLevel());
	if (nullptr == Level)
	{
		MsgAssert("ObjectUpdate��Ŷ�� �����Ϸ��� Actor�� BaseLevel�� ��ӹ��� ������ �������� �ʽ��ϴ�");
		return;
	}

	Net_LevelType LevelType = Level->GetNetLevelType();
	if (Net_LevelType::UNKNOWN == LevelType)
	{
		MsgAssert("ObjectUpdate��Ŷ�� �����Ϸ��� Actor�� �� �� ���� ������ �����մϴ�.");
		return;
	}

	UpdatePacket->LevelType = LevelType;

	GameEngineTransform* TransPtr = ActorPtr->GetTransform();

	//��ġ
	UpdatePacket->Rotation = TransPtr->GetWorldRotation();
	//��ġ
	UpdatePacket->Position = TransPtr->GetWorldPosition();
	//Ÿ�� ũ��
	UpdatePacket->TimeScale = _Param.TimeScale;

	//�ı� ����
	UpdatePacket->IsDeath = ActorPtr->IsDeath();
	if (true == UpdatePacket->IsDeath)
	{
		//�� NetObject�� �������� ����/������ ���� ����
		NetPtr->NetDisconnect();
	}

	UpdatePacket->CopyUnionData(_Param.UnionData);
}


void NetworkManager::SendFsmChangePacket(GameEngineNetObject* _NetObjPtr, int _FsmState)
{
	if (nullptr == _NetObjPtr)
	{
		MsgAssert("FSM ��ȯ ��Ŷ�� �����Ҷ� �Ķ���� ������ _NetObjPtr�� ���� �ݵ�� �־��־�� �մϴ�.");
		return;
	}

	GameEngineActor* ActorPtr = dynamic_cast<GameEngineActor*>(_NetObjPtr);
	if (nullptr == ActorPtr)
	{
		MsgAssert("FSM ��ȯ ��Ŷ�� �����Ҷ� ���ڷ� ���� _NetObjPtr�� GameEngineActor�� ��ӹ��� �ʾҽ��ϴ�");
		return;
	}

	//��Ʈ��ũ ���� ���°� �ƴѰ��
	if (NetworkState::None == NowState)
		return;


	//���� �������� ������ ���͵鸸 ����
	if (ActorPtr->GetLevel() != CurLevel)
		return;

	//���ڷ� ���� ������Ʈ�� ��Ʈ��ũ�� ������Ʈ ���̵�
	unsigned int ObjectID = _NetObjPtr->GetNetObjectID();

	//���� Initó������ ���� ���
	if (-1 == ObjectID)
		return;

	if (false == GameEngineNetObject::IsNetObject(ObjectID))
	{
		MsgAssert(GameEngineString::ToString(ObjectID) + " ID�� ���� ������Ʈ�� �������� �ʴµ�, Fsm ��ȯ ��Ŷ�� ����Ϸ��� �߽��ϴ�");
		return;
	}

	if (NetControllType::NetControll == _NetObjPtr->GetControllType())
	{
		MsgAssert("��Ŷ�� �޾� ó���Ǵ� ������Ʈ�� Fsm���� ��Ŷ�� �������� �߽��ϴ�");
		return;
	}

	//�̹� Deathó���Ǿ ���۵��� �ʴ� ������Ʈ�� ���
	if (true == _NetObjPtr->IsNetDisconnected())
		return;

	
	std::shared_ptr<FsmChangePacket> Packet = nullptr;
	Packet = std::make_shared<FsmChangePacket>();

	Packet->SetObjectID(ObjectID);
	Packet->FsmState = _FsmState;
	Packet->NetID = NetID;

	//���� Ÿ��
	BaseLevel* Level = dynamic_cast<BaseLevel*>(ActorPtr->GetLevel());
	if (nullptr == Level)
	{
		MsgAssert("FSM ��ȯ ��Ŷ�� �����Ϸ��� Actor�� BaseLevel�� ��ӹ��� ������ �������� �ʽ��ϴ�");
		return;
	}

	Net_LevelType LevelType = Level->GetNetLevelType();
	if (Net_LevelType::UNKNOWN == LevelType)
	{
		MsgAssert("FSM ��ȯ ��Ŷ�� �����Ϸ��� Actor�� �� �� ���� ������ �����մϴ�.");
		return;
	}

	Packet->LevelType = LevelType;

	NetInst->SendPacket(Packet);
}


void NetworkManager::LinkNetwork(GameEngineNetObject* _NetObjPtr)
{
	//�̹� ������ ������ ��쿡�� �Ʒ��� �����Ű�� ����
	if (-1 != _NetObjPtr->GetNetObjectID())
		return;

	//�̱۸�� �϶�
	if (nullptr == NetInst)
	{
		_NetObjPtr->SetUserControllType();
		return;
	}

	//������ ��쿣 �ش� ���͸� �ٷ� ���� ���� �� ���� ��Ʈ��
	if (true == IsServer())
	{
		_NetObjPtr->InitNetObject(GameEngineNetObject::CreateServerID(), NetInst);
		_NetObjPtr->SetUserControllType();
		return;
	}

	//Ŭ���� ��쿣 ������ ���� ��û ��Ŷ�� ����
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

	//������Ʈ ��Ŷ ����ȭ
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


	//��� ��Ŷ ����ȭ, ����ȭ ����� ChunkPackets�� ���̰� ��
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


	
	//�� ��Ŷ�� ����ȭ ����� ChunkPackets���� �ϳ��� ����Ʈ�� ChunkBytes�� �ű��
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

		//ó������ ��Ŷ ����� ã����
		if (0 == PacketSize)
		{
			unsigned char* Ptr = _Ser.GetDataPtr();
			memcpy_s(&PacketSize, sizeof(int), &Ptr[4], sizeof(int));
			continue;
		}

		//��Ŷ�� ����ȭ �� �� ���� Size��ġ�� ���� ����
		unsigned char* SizePtr = _Ser.GetDataPtr();
		size_t SizePos = (static_cast<size_t>(PacketSize) * i) + 4;
		memcpy_s(&SizePtr[SizePos], sizeof(int), &PacketSize, sizeof(int));
	}
}







std::shared_ptr<GameEngineNetObject> NetworkManager::CreateNetActor(Net_ActorType _ActorType, int _ObjectID /*= -1*/)
{
	if (nullptr == CurLevel)
	{
		MsgAssert("NetwortManager�� CurLevel �����Ͱ� nullptr�Դϴ�");
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
		MsgAssert("NetworkManager���� ������ �� ���� ActorType�� ���Խ��ϴ�");
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
