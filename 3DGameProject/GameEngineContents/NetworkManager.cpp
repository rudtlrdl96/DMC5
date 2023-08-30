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




void NetworkManager::ConnectServer(PlayerType _CharacterType)
{
	if (false == NetworkGUI::GetInst()->ChangeFieldState())
	{
		MsgAssert("NetworkGUI���� ���� ���� �۾��� �������� �ʾҽ��ϴ�");
		return;
	}

	//������ ����Ǵ� ������ ���Ϳ� ����
	if (true == AllBattleLevels.empty())
	{
		AllBattleLevels.push_back(MainLevel::GetInst());
		AllBattleLevels.push_back(StartStageLevel::GetInst());
		AllBattleLevels.push_back(BossStageLevel::GetInst());
	}
	else
	{
		MsgAssert("�̹� ��Ʈ��ũ ����� �����ߴµ�, �ٽ� �ѹ� ��Ʈ��ũ ������ �����Ͽ����ϴ�");
		return;
	}

	CharacterType = _CharacterType;

	//������ ��� �� ���� ������ �÷��̾� ����
	if (NetworkState::Server == NowState)
	{
		for (GameEngineLevel* Level : AllBattleLevels)
		{
			CreateLocalPlayer(Level, GameEngineNetObject::CreateServerID());
		}
		return;
	}

	//Ŭ���̾�Ʈ�� ���, ������ ���� �õ�
	const std::pair<std::string_view, int> NetworkData = NetworkGUI::GetInst()->GetNetworkData();
	const std::string_view IP = NetworkData.first;
	const int PortNum = NetworkData.second;

	//Thread �̸� ����
	SetThreadDescription(GetCurrentThread(), L"Client Main Thread");
	NetInst = &ClientInst;

	//Ŭ��� ��Ŷ ó�� �ݹ� ���
	ClientPacketInit();

	//���� ����
	bool IsResult = ClientInst.Connect(IP.data(), static_cast<unsigned short>(PortNum));
	if (true == IsResult)
	{
		NowState = NetworkState::Client;
		return;
	}

	//���� ���� ������ ���
	std::string ErrorIP = IP.data();
	MsgAssert("�� Ŭ���̾�Ʈ�� ���� ���ӿ� �����Ͽ����ϴ�.\n������ �õ��� IP : " + ErrorIP);
}




void NetworkManager::PushUpdatePacket(GameEngineNetObject* _NetObj, GameEngineActor* _ActorPtr, float _TimeScale /*= 1.f*/)
{
	if (NetworkState::None == NowState)
		return;

	//���� �������� ������ ���͵鸸 ����
	if (_ActorPtr->GetLevel() != CurLevel)
		return;

	//���ڷ� ���� ������Ʈ�� ��Ʈ��ũ�� ������Ʈ ���̵�
	unsigned int ObjectID = _NetObj->GetNetObjectID();
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

	int PacketSize = 0;
	int Count = 0;

	//��Ŷ ��� ������
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

	//�� ����Ʈ�� ���� ��Ŷ ������
	NetInst->Send(ChunkUpdatePackets.GetConstCharPtr(), ChunkUpdatePackets.GetWriteOffSet());

	//�ڷᱸ��, ����ȭ ���� Ŭ����
	AllUpdatePacket.clear();
	ChunkUpdatePackets.Reset();
}


#include "NetTestPlayer.h"

std::shared_ptr<GameEngineNetObject> NetworkManager::CreateNetActor(Net_ActorType _ActorType, int _ObjectID /*= -1*/)
{
	if (nullptr == CurLevel)
	{
		MsgAssert("NetwortManager�� CurLevel �����Ͱ� nullptr�Դϴ�");
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



void NetworkManager::CreateLocalPlayer(class GameEngineLevel* _Level, int _ObjectID)
{
	//std::shared_ptr<BasePlayerActor> Player = nullptr;

	if (PlayerType::None == CharacterType)
	{
		MsgAssert("������ �÷��̾ PlayerType::None�Դϴ�");
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