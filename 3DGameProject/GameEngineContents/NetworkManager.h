#pragma once
#include <GameEngineBase/GameEngineNetServer.h>
#include <GameEngineBase/GameEngineNetClient.h>
#include "ContentsEnum.h"

class BaseLevel;
class GameEngineNetObject;
class GameEngineActor;
enum class PacketEnum;

struct UpdatePacketParameter
{
	GameEngineNetObject* ObjPtr = nullptr;
	unsigned int FsmState = -1;
	bool IsFsmForce = false;
	float TimeScale = 1.f;
};



class NetworkManager
{
public:
	enum class NetworkState
	{
		None,
		Server,
		Client,
	};

	//���� �����尡 �������������� ����
	static inline bool IsServer()
	{
		return (NetworkState::Server == NowState);
	}

	//���� �����尡 Ŭ�󽺷������� ����
	static inline bool IsClient()
	{
		return (NetworkState::Client == NowState);
	}

	//������ �����ϰų� ����Ǿ����� ����
	static inline bool IsNet()
	{
		return NetInst->IsNet();
	}

	static unsigned int ServerOpen(int _Port);

	static void ConnectServer(const std::string& _IP, int PortNum, std::function<void(unsigned int)> _ConnectCallBack);


	//���۹��� ��Ŷ�� ó���ϴ� �κ�
	static void Update_PacketProcess(BaseLevel* _CurrentLevel);




	//��Ʈ��ũ�� ���͸� �����մϴ�. Ŭ���̾�Ʈ�� ��쿣 nullptr�� ��ȯ�ϴ� �� nullptr�� ���ּ���
	template <typename ActorPtr>
	static std::shared_ptr<ActorPtr> CreateNetworkActor(Net_ActorType _NetActorType)
	{
		//�÷��̾ ������ ��� ������ ����(ȣ��Ʈ)������ �̷�� ���ϴ�.
		//�׷��� ������ �ƴϸ� nullptr�� ���ϵǴ��� ���Ǻ�Ź�帳�ϴ�.
		if (false == IsServer())
			return nullptr;

		std::shared_ptr<GameEngineNetObject> NetObjPtr = nullptr;
		NetObjPtr = CreateNetActor(_NetActorType);
		NetObjPtr->SetUserControllType();

		return std::dynamic_pointer_cast<ActorPtr>(NetObjPtr);
	}



	static void PushChatPacket(const std::string_view& _Msg);

	//Update��Ŷ�� ������ �� �������̽��� �̿��ؼ� �����ֽø� �˴ϴ�.
	static void PushUpdatePacket(const UpdatePacketParameter& _Param);

	//�̹� �����Ǿ� �ִ� ������Ʈ�� ������ ������ŵ�ϴ�.
	static void LinkNetwork(GameEngineNetObject* _NetObjPtr);

	//�׿��ִ� ��� ��Ŷ�� �����ϴ� �κ�
	static void FlushPackets();

protected:
	static GameEngineNet* NetInst;
	static GameEngineNetServer ServerInst;
	static GameEngineNetClient ClientInst;

	//�� ���μ����� ȣ��Ʈ(����)���� Ŭ���̾�Ʈ����, �ƴϸ� �ƿ� ��Ʈ��ũ�� ���ӵ��� �ʾҴ��� ����
	static NetworkState NowState;

	//Ŭ���̾�Ʈ�� Accept�� �� ó���ؾ� �� �۾�
	static void AcceptCallback(SOCKET _Socket, GameEngineNetServer* _Server);
	
	//��Ŷ���� ������ Ŭ�󿡼� ��� ó���� �� ���
	static void ServerPacketInit();
	static void ClientPacketInit();

private:
	//��Ʈ��ũ���� �� ���μ��� ������ ��ȣ
	static unsigned int NetID;

	//������Ʈ ��Ŷ�� ������ �ߺ��� ���ϱ� ���� �� <������Ʈ ���̵�, ������Ʈ ������Ʈ ��Ŷ ������>
	static std::map<unsigned int, std::shared_ptr<class ObjectUpdatePacket>> AllUpdatePacket;

	static std::map<PacketEnum, std::vector<std::shared_ptr<class GameEnginePacket>>> AllPacket;
	static std::map<PacketEnum, GameEngineSerializer> ChunkPackets;
	static GameEngineSerializer ChunkBytes;


	static BaseLevel* CurLevel;
	static Net_LevelType CurLevelType;

	static std::function<void(unsigned int)> ConnectCallBack;



	//���� ����
	static std::shared_ptr<GameEngineNetObject> CreateNetActor(unsigned int _ActorType, int _ObjectID = -1)
	{
		Net_ActorType ActorType = static_cast<Net_ActorType>(_ActorType);
		return CreateNetActor(ActorType, _ObjectID);
	}

	static std::shared_ptr<GameEngineNetObject> CreateNetActor(Net_ActorType _ActorType, int _ObjectID = -1);


	//vector ������ ��Ŷ���� _Ser���ڿ� ����ȭ ��Ŵ
	static void SerializePackets(const std::vector<std::shared_ptr<class GameEnginePacket>>& _Packets, GameEngineSerializer& _Ser);


	NetworkManager(){}
	virtual ~NetworkManager() = 0{}

	NetworkManager(const NetworkManager& _Other) = delete;
	NetworkManager(NetworkManager&& _Other) noexcept = delete;
	NetworkManager& operator=(const NetworkManager& _Other) = delete;
	NetworkManager& operator=(const NetworkManager&& _Other) noexcept = delete;
};

