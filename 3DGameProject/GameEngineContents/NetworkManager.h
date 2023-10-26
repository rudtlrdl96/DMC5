#pragma once
#include <GameEngineBase/GameEngineNetServer.h>
#include <GameEngineBase/GameEngineNetClient.h>
#include "ContentsEnum.h"
#include "ObjectUpdatePacket.h"

class BaseLevel;
class NetworkObjectBase;
class GameEngineActor;
class GameEngineLevel;
enum class PacketEnum;
class BasePlayerActor;

class NetworkManager
{
public:
	static const float PacketFlushTime;
	
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

	static inline bool IsSingle()
	{
		return (NetworkState::None == NowState);
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


	template <typename ActorPtr>
	static std::shared_ptr<ActorPtr> CreateNetworkActor(class GameEngineLevel* _Level, int _Order = 0)
	{
		//�÷��̾ ������ ��� ������ ����(ȣ��Ʈ)������ �̷�� ���ϴ�.
		//�׷��� ������ �ƴϸ� nullptr�� ���ϵǴ��� ���Ǻ�Ź�帳�ϴ�.
		if (true == IsClient())
			return nullptr;

		std::shared_ptr<NetworkObjectBase> NetObjPtr = nullptr;
		NetObjPtr = _Level->CreateActor<ActorPtr>(_Order);
		NetObjPtr->InitNetObject(GameEngineNetObject::CreateServerID(), NetInst);
		NetObjPtr->SetUserControllType();

		return std::dynamic_pointer_cast<ActorPtr>(NetObjPtr);
	}

	static void PushChatPacket(const std::string_view& _Msg, const float4& _Color = float4::WHITE);

	//Update��Ŷ�� ������ �� �������̽��� �̿��ؼ� �����ֽø� �˴ϴ�.
	static void PushUpdatePacket(
		NetworkObjectBase* _ObjPtr, 
		GameEngineActor* _ActorPtr,
		float _TimeScale,
		const std::vector<int*>& _IntDatas,
		const std::vector<float*>& _FloatDatas,
		const std::vector<bool*>& _BoolDatas);
	
	//FsmChange��Ŷ�� ������ �� �������̽��� �̿��ؼ� �����ֽø� �˴ϴ�.(��� ��Ŷ�� �ٷ� �����ϴ�)
	static void SendFsmChangePacket(NetworkObjectBase* _NetObjPtr, int _FsmState, NetworkObjectBase* _Attacker = nullptr);



	//�̹� �����Ǿ� �ִ� ������Ʈ�� ������ ������ŵ�ϴ�.
	static void LinkNetwork(NetworkObjectBase* _NetObjPtr, BaseLevel* _Level = nullptr);

	//�׿��ִ� ��� ��Ŷ�� �����ϴ� �κ�
	static void FlushPackets();

	//�ش� enum���� �ش�Ǵ� �̺�Ʈ�� ����� �� �ֽ��ϴ�. �̺�Ʈ�� �ߺ��Ͽ� ����� �� �ֱ� ������ �����ؾ� �մϴ�.
	static void PushNetworkEvent(Net_EventType _Type, const std::function<void()>& _Event);

	/// <summary>
	/// ȣ��Ʈ�� �ٸ� Ŭ���̾�Ʈ�鿡�� ���ڷ� ���� �̺�Ʈ�� ȣ���Ű���� �䱸�մϴ�
	/// </summary>
	/// <param name="Type"> : �ش� Ÿ���� �̺�Ʈ�� Ŭ���̾�Ʈ�鿡�� �䱸�մϴ�</param>
	/// <returns> : �۾��� ������ ��� �α׸� ����� false�� ��ȯ�մϴ�</returns>
	static bool ExcuteNetworkEvent(Net_EventType _Type);

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
	static GameEngineSerializer ChunkPacketBytes;


	static BaseLevel* CurLevel;
	static Net_LevelType CurLevelType;

	static std::function<void(unsigned int)> ConnectCallBack;

	//ä�� �޼����� �ö� ���ʿ� �� ���ڰ� ��������� System�޼����̴�
	static const std::string SystemChatCheck;

	//��Ʈ��ũ ���̵𺰷� ������Ʈ�� �з��ؼ� ����
	static std::map<unsigned int, std::vector<unsigned int>> AllNetID;

	//��Ʈ��ũ�� ���� ���޵� ������ �̺�Ʈ�� ��Ƴ��� �� map
	static std::vector<std::vector<std::function<void()>>> AllNetEvent;

	void DisconnectObjects(unsigned int _NetID);



	//���� ����
	static std::shared_ptr<NetworkObjectBase> CreateNetActor(unsigned int _ActorType, BaseLevel* _Level = nullptr, int _ObjectID = -1)
	{
		Net_ActorType ActorType = static_cast<Net_ActorType>(_ActorType);
		return CreateNetActor(ActorType, _Level, _ObjectID);
	}

	static std::shared_ptr<NetworkObjectBase> CreateNetActor(Net_ActorType _ActorType, BaseLevel* _Level = nullptr, int _ObjectID = -1);


	template <typename DataType>
	static void ArrDataCopy(std::vector<DataType>& _Left, const std::vector<DataType*>& _Right)
	{
		if (_Left.size() != _Right.size())
		{
			_Left.clear();
			_Left.resize(_Right.size());
		}

		for (size_t i = 0; i < _Right.size(); ++i)
		{
			_Left[i] = *_Right[i];
		}
	}

	NetworkManager(){}
	virtual ~NetworkManager() = 0{}

	NetworkManager(const NetworkManager& _Other) = delete;
	NetworkManager(NetworkManager&& _Other) noexcept = delete;
	NetworkManager& operator=(const NetworkManager& _Other) = delete;
	NetworkManager& operator=(const NetworkManager&& _Other) noexcept = delete;
};

