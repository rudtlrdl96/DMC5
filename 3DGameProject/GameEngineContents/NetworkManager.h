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

	//현재 스레드가 서버스레드인지 여부
	static inline bool IsServer()
	{
		return (NetworkState::Server == NowState);
	}

	//현재 스레드가 클라스레드인지 여부
	static inline bool IsClient()
	{
		return (NetworkState::Client == NowState);
	}

	static inline bool IsSingle()
	{
		return (NetworkState::None == NowState);
	}

	//서버를 오픈하거나 연결되었는지 여부
	static inline bool IsNet()
	{
		return NetInst->IsNet();
	}

	static unsigned int ServerOpen(int _Port);

	static void ConnectServer(const std::string& _IP, int PortNum, std::function<void(unsigned int)> _ConnectCallBack);


	//전송받은 패킷을 처리하는 부분
	static void Update_PacketProcess(BaseLevel* _CurrentLevel);


	//네트워크용 엑터를 생성합니다. 클라이언트의 경우엔 nullptr을 반환하니 꼭 nullptr를 해주세요
	template <typename ActorPtr, typename OrderType>
	static std::shared_ptr<ActorPtr> CreateNetworkActor(class GameEngineLevel* _Level, OrderType _Order)
	{
		return CreateNetworkActor(_Level, static_cast<int>(_Order));
	}

	template <typename ActorPtr>
	static std::shared_ptr<ActorPtr> CreateNetworkActor(class GameEngineLevel* _Level, int _Order = 0)
	{
		//플레이어를 제외한 모든 생성은 서버(호스트)에서만 이루어 집니다.
		//그러니 서버가 아니면 nullptr가 리턴되는점 주의부탁드립니다.
		if (true == IsClient())
			return nullptr;

		std::shared_ptr<NetworkObjectBase> NetObjPtr = nullptr;
		NetObjPtr = _Level->CreateActor<ActorPtr>(_Order);
		NetObjPtr->InitNetObject(GameEngineNetObject::CreateServerID(), NetInst);
		NetObjPtr->SetUserControllType();

		return std::dynamic_pointer_cast<ActorPtr>(NetObjPtr);
	}

	static void PushChatPacket(const std::string_view& _Msg, const float4& _Color = float4::WHITE);

	//Update패킷을 보낼때 이 인터페이스를 이용해서 보내주시면 됩니다.
	static void PushUpdatePacket(
		NetworkObjectBase* _ObjPtr, 
		GameEngineActor* _ActorPtr,
		float _TimeScale,
		const std::vector<int*>& _IntDatas,
		const std::vector<float*>& _FloatDatas,
		const std::vector<bool*>& _BoolDatas);
	
	//FsmChange패킷을 보낼때 이 인터페이스를 이용해서 보내주시면 됩니다.(얘는 패킷을 바로 보냅니다)
	static void SendFsmChangePacket(NetworkObjectBase* _NetObjPtr, int _FsmState, bool _IsPassiveRequest = false);



	//이미 생성되어 있는 오브젝트를 서버와 연동시킵니다.
	static void LinkNetwork(NetworkObjectBase* _NetObjPtr, BaseLevel* _Level = nullptr);

	//쌓여있던 모든 패킷을 전송하는 부분
	static void FlushPackets();

	//해당 레벨의 플레이어 그룹을 반환합니다.(LevelChangeStart시점에서 사용하실땐 Level 인자를 넣어주세요)
	static const std::vector<BasePlayerActor*>& GetPlayers(GameEngineLevel* _Level = nullptr);
	//이거 나중까지 안 쓰면 코드 정리를 위해 지우자


protected:
	static GameEngineNet* NetInst;
	static GameEngineNetServer ServerInst;
	static GameEngineNetClient ClientInst;

	//이 프로세스가 호스트(서버)인지 클라이언트인지, 아니면 아예 네트워크에 접속되지 않았는지 여부
	static NetworkState NowState;

	//클라이언트가 Accept될 때 처리해야 할 작업
	static void AcceptCallback(SOCKET _Socket, GameEngineNetServer* _Server);
	
	//패킷들을 서버와 클라에서 어떻게 처리할 지 등록
	static void ServerPacketInit();
	static void ClientPacketInit();

private:
	//네트워크에서 이 프로세스 고유의 번호
	static unsigned int NetID;

	//업데이트 패킷을 보낼때 중복을 피하기 위한 맵 <오브젝트 아이디, 오브젝트 업데이트 패킷 포인터>
	static std::map<unsigned int, std::shared_ptr<class ObjectUpdatePacket>> AllUpdatePacket;
	static std::map<PacketEnum, std::vector<std::shared_ptr<class GameEnginePacket>>> AllPacket;
	static GameEngineSerializer ChunkPacketBytes;


	static BaseLevel* CurLevel;
	static Net_LevelType CurLevelType;

	static std::function<void(unsigned int)> ConnectCallBack;

	//PlayerActor만을 담아놓는 자료구조
	static std::map<GameEngineLevel*, std::vector<BasePlayerActor*>> AllPlayerActors;


	//엑터 생성
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

	//Actor를 생성할 때 플레이어라면 별도의 자료구조에 저장
	static void RegistPlayer(NetworkObjectBase* _NetObjPtr, GameEngineLevel*  _Level);


	NetworkManager(){}
	virtual ~NetworkManager() = 0{}

	NetworkManager(const NetworkManager& _Other) = delete;
	NetworkManager(NetworkManager&& _Other) noexcept = delete;
	NetworkManager& operator=(const NetworkManager& _Other) = delete;
	NetworkManager& operator=(const NetworkManager&& _Other) noexcept = delete;
};

