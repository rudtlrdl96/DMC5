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
#include "Enemy_Empusa.h"
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

const std::string NetworkManager::SystemChatCheck = "[System] : ";


std::map<unsigned int, std::vector<unsigned int>> NetworkManager::AllNetID;



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



void NetworkManager::PushUpdatePacket(
	NetworkObjectBase* _ObjPtr, 
	GameEngineActor* _ActorPtr,
	float _TimeScale,
	const std::vector<int*>& _IntDatas,
	const std::vector<float*>& _FloatDatas,
	const std::vector<bool*>& _BoolDatas)
{
	if (nullptr == _ObjPtr)
	{
		MsgAssert("������Ʈ ��Ŷ�� �����Ҷ� �Ķ���� ������ ObjPtr�� ���� �ݵ�� �־��־�� �մϴ�.");
		return;
	}

	if (nullptr == _ActorPtr)
	{
		MsgAssert("Update��Ŷ�� �����Ҷ� _ActorPtr���ڰ� nullptr�Դϴ�");
		return;
	}

	//��Ʈ��ũ ���� ���°� �ƴѰ��
	if (NetworkState::None == NowState)
		return;

	
	//���� �������� ������ ���͵鸸 ����
	if (_ActorPtr->GetLevel() != CurLevel)
		return;

	//���ڷ� ���� ������Ʈ�� ��Ʈ��ũ�� ������Ʈ ���̵�
	unsigned int ObjectID = _ObjPtr->GetNetObjectID();
	
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
	Net_ActorType NetType = _ObjPtr->GetNetObjectType();
	if (Net_ActorType::UNKNOWN == NetType)
	{
		MsgAssert("ObjectUpdate��Ŷ�� �������� ��ü�� NetObjectType�� ���������� �ʾҽ��ϴ�");
		return;
	}

	UpdatePacket->ActorType = static_cast<unsigned int>(NetType);
	

	//���� Ÿ��
	BaseLevel* Level = dynamic_cast<BaseLevel*>(_ActorPtr->GetLevel());
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
		_ObjPtr->NetDisconnect();
		NetworkGUI::GetInst()->PrintLog("Disconnect Actor With Network", float4::GREEN);
	}

	//On/Off ����
	UpdatePacket->IsUpdate = _ActorPtr->IsUpdate();

	ArrDataCopy(UpdatePacket->IntDatas, _IntDatas);
	ArrDataCopy(UpdatePacket->FloatDatas, _FloatDatas);
	ArrDataCopy(UpdatePacket->BoolDatas, _BoolDatas);
}


void NetworkManager::SendFsmChangePacket(NetworkObjectBase* _NetObjPtr, int _FsmState, NetworkObjectBase* _Attacker /*= nullptr*/)
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

	NetControllType NetCtrlType = _NetObjPtr->GetControllType();
	if ((nullptr == _Attacker) && (NetControllType::PassiveControll == NetCtrlType))
	{
		MsgAssert("��Ŷ�� �޾� ó���Ǵ� ������Ʈ�� Fsm���� ��Ŷ�� �������� �߽��ϴ�");
		return;
	}
	else if ((nullptr != _Attacker) && (NetControllType::ActiveControll == NetCtrlType))
	{
		MsgAssert("Fsm�� ���� �����ϴ� ��ü�� FSM ���� ��û ��Ŷ�� �������� �߽��ϴ�");
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

	//������ ����� �ִ� ���
	if (nullptr != _Attacker)
	{
		int AttackerID = _Attacker->GetNetObjectID();
		if (false == GameEngineNetObject::IsNetObject(AttackerID))
		{
			MsgAssert("������ �� ��ü�� ��Ʈ��ũ�� ����Ǿ� ���� �ʽ��ϴ�");
			return;
		}

		Packet->AttackerID = AttackerID;
	}

	NetInst->SendPacket(Packet);
}


void NetworkManager::LinkNetwork(NetworkObjectBase* _NetObjPtr, BaseLevel* _Level /*= nullptr*/)
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
	Net_ActorType NetType =  _NetObjPtr->GetNetObjectType();
	if (Net_ActorType::UNKNOWN == NetType)
	{
		MsgAssert("Net_ActorType�� ���������� �ʾƼ� ��Ʈ��ũ�� ������ �� �����ϴ�");
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

	//������Ʈ ��Ŷ ����ȭ
	if (false == AllUpdatePacket.empty())
	{
		for (const std::pair<unsigned int, std::shared_ptr<ObjectUpdatePacket>>& Pair : AllUpdatePacket)
		{
			std::shared_ptr<ObjectUpdatePacket> UpdatePacket = Pair.second;
			UpdatePacket->SerializePacket(ChunkPacketBytes);
		}

		AllUpdatePacket.clear();
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
		MsgAssert("NetwortManager�� CurLevel �����Ͱ� nullptr�Դϴ�");
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
		MsgAssert("������ �ʿ��� Net_ActorType�� ���������� �ʾ� ��ü�� ����� �����ϴ�.");
		break;
	}
	case Net_ActorType::Nero:
		NetObject = Poolable<PlayerActor_Nero>::PopFromPool(CreateLevel, static_cast<int>(ActorOrder::Player));
		break;
	case Net_ActorType::Vergil:
		NetObject = Poolable<PlayerActor_Vergil>::PopFromPool(CreateLevel, static_cast<int>(ActorOrder::Player));
		break;
	case Net_ActorType::MirageBlade:
		NetObject = Poolable<Player_MirageBlade>::PopFromPool(CreateLevel, static_cast<int>(ActorOrder::Player));
		break;
	case Net_ActorType::HellCaina:
		NetObject = Poolable<Enemy_HellCaina>::PopFromPool(CreateLevel, static_cast<int>(ActorOrder::Enemy));
		break;
	case Net_ActorType::Empusa:
		NetObject = Poolable<Enemy_Empusa>::PopFromPool(CreateLevel, static_cast<int>(ActorOrder::Enemy));
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

	const std::function<void()>& DestoryCallBack = NetworkObjectBase::PopReservedDestroyCallback(_ActorType);
	if (nullptr != DestoryCallBack)
	{
		NetObject->PushDestroyCallback(DestoryCallBack);
	}
	

	return NetObject;
}





void NetworkManager::DisconnectObjects(unsigned int _NetID)
{
	if (false == IsServer())
		return;

	const std::vector<unsigned int>& DisconnectNetGroup = AllNetID[_NetID];
	for (unsigned int ObjID : DisconnectNetGroup)
	{
		GameEngineNetObject* NetObjPtr = GameEngineNetObject::GetNetObject(ObjID);
		if (nullptr == NetObjPtr)
			continue;

		GameEngineActor* ObjPtr = dynamic_cast<GameEngineActor*>(NetObjPtr);
		if (nullptr == ObjPtr)
		{
			MsgAssert("GameEngineNetObject�� GameEngineActor�� ��ȯ��Ű�µ� �����Ͽ����ϴ�");
			return;
		}

		ObjPtr->Death();
	}

	AllNetID.erase(_NetID);
}