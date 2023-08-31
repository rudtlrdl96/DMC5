#pragma once
#include <GameEngineBase/GameEngineNetServer.h>
#include <GameEngineBase/GameEngineNetClient.h>
#include "ContentsEnum.h"

class GameEngineLevel;
class GameEngineNetObject;
class GameEngineActor;
enum class PacketEnum;

class NetworkManager
{
public:
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

	//서버를 오픈하거나 연결되었는지 여부
	static inline bool IsNet()
	{
		return NetInst->IsNet();
	}

	static unsigned int ServerOpen(int _Port);

	static void ConnectServer(PlayerType _CharacterType);

	//전송받은 패킷을 처리하는 부분
	static void Update_PacketProcess(GameEngineLevel* _CurrentLevel)
	{
		//레벨 저장(패킷 처리할때 사용됨)
		CurLevel = _CurrentLevel;

		if (nullptr == NetInst)
			return;

		NetInst->UpdatePacket();
	}

	//네트워크용 엑터를 생성합니다. 클라이언트의 경우엔 nullptr을 반환하니 꼭 nullptr를 해주세요
	template <typename ActorPtr>
	static std::shared_ptr<ActorPtr> CreateNetworkActor(Net_ActorType _NetActorType)
	{
		if (false == IsServer())
			return nullptr;

		std::shared_ptr<GameEngineNetObject> NetObjPtr = nullptr;
		NetObjPtr = CreateNetActor(_NetActorType);
		NetObjPtr->SetUserControllType();

		return std::dynamic_pointer_cast<ActorPtr>(NetObjPtr);
	}


	static void PushChat(const std::string_view& _Msg);


	//Update패킷을 보낼때 이 인터페이스를 이용해서 보내주시면 됩니다.
	static void PushUpdatePacket(GameEngineNetObject* _NetObj, GameEngineActor* _ActorPtr, float _TimeScale = 1.f);

	//실제 업데이트 패킷을 처리하는 부분
	static void FlushUpdatePacket();

protected:
	static GameEngineNet* GetNetInst()
	{
		return NetInst;
	}

private:
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
	static std::map<PacketEnum, GameEngineSerializer> ChunkPackets;
	static GameEngineSerializer ChunkBytes;


	static std::vector<class GameEngineLevel*> AllBattleLevels;

	static GameEngineLevel* CurLevel;

	static PlayerType CharacterType;

	



	//엑터 생성
	static std::shared_ptr<GameEngineNetObject> CreateNetActor(unsigned int _ActorType, int _ObjectID = -1)
	{
		Net_ActorType ActorType = static_cast<Net_ActorType>(_ActorType);
		return CreateNetActor(ActorType, _ObjectID);
	}

	static std::shared_ptr<GameEngineNetObject> CreateNetActor(Net_ActorType _ActorType, int _ObjectID = -1);


	static void CreateLocalPlayer(class GameEngineLevel* _Level, int _ObjectID);


	//vector 형태의 패킷들을 _Ser인자에 직렬화 시킴
	static void SerializePackets(const std::vector<std::shared_ptr<class GameEnginePacket>>& _Packets, GameEngineSerializer& _Ser);


	NetworkManager(){}
	virtual ~NetworkManager() = 0{}

	NetworkManager(const NetworkManager& _Other) = delete;
	NetworkManager(NetworkManager&& _Other) noexcept = delete;
	NetworkManager& operator=(const NetworkManager& _Other) = delete;
	NetworkManager& operator=(const NetworkManager&& _Other) noexcept = delete;
};

