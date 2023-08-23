#pragma once
#include <GameEngineBase/GameEngineNetServer.h>
#include <GameEngineBase/GameEngineNetClient.h>
#include "ContentsEnum.h"

class GameEngineLevel;
class GameEngineNetObject;
class GameEngineTransform;

class NetworkManager
{
public:
	//현재 스레드가 서버스레드인지 여부
	static inline bool IsServer()
	{
		return IsServerValue;
	}

	//현재 스레드가 클라스레드인지 여부
	static inline bool IsClient()
	{
		return IsClientValue;
	}

	//서버를 오픈하거나 연결되었는지 여부
	static inline bool IsNet()
	{
		return NetInst->IsNet();
	}

	static void ServerOpen(int _Port);
	static bool ConnectServer(const std::string_view& _IP, int _Port);

	//전송받은 패킷을 처리하는 부분
	static void Update_PacketProcess(GameEngineLevel* _CurrentLevel)
	{
		//레벨 저장(패킷 처리할때 사용됨)
		CurLevel = _CurrentLevel;

		if (nullptr == NetInst)
			return;

		NetInst->UpdatePacket();
	}

	static unsigned int GetNetID()
	{
		return NetID;
	}

	//Update패킷을 보낼때 이 인터페이스를 이용해서 보내주시면 됩니다.
	static void SendUpdatePacket(GameEngineNetObject* _NetObj, GameEngineTransform* TransPtr, float _TimeScale);

	//실제 업데이트 패킷을 처리하는 부분
	static void FlushUpdatePacket();

	//Net_ActorType 타입의 엑터를 생성하는 부분입니다.
	static void SendCreatePacket(Net_ActorType _Type, const float4& _Position = float4::ZERO, const float4& _Rotation = float4::ZERO);

	//이미 생성되어 있는 오브젝트를 서버와 연동시킵니다.
	static void LinkNetwork(GameEngineNetObject* _NetObjPtr);

protected:
	friend class NetTestPlayer;

	static GameEngineNet* GetNetInst()
	{
		return NetInst;
	}

private:
	static GameEngineNet* NetInst;
	static GameEngineNetServer ServerInst;
	static GameEngineNetClient ClientInst;



	//현재 스레드가 서버스레드인지 여부
	static bool IsServerValue;
	//현재 스레드가 클라스레드인지 여부
	static bool IsClientValue;

	static unsigned int NetID;


	//클라이언트가 Accept될 때 처리해야 할 작업
	static void AcceptCallback(SOCKET _Socket, GameEngineNetServer* _Server);
	
	//패킷들을 서버와 클라에서 어떻게 처리할 지 등록
	static void ServerPacketInit();
	static void ClientPacketInit();

	

private:
	//업데이트 패킷을 보낼때 중복을 피하기 위한 맵 <오브젝트 아이디, 오브젝트 업데이트 패킷 포인터>
	static std::map<unsigned int, std::shared_ptr<class ObjectUpdatePacket>> AllUpdatePacket;

	static GameEngineLevel* CurLevel;

	inline static GameEngineLevel* GetLevel()
	{
		if (nullptr == CurLevel)
		{
			MsgAssert("NetwortManager의 CurLevel 포인터가 nullptr입니다");
		}

		return CurLevel;
	}

	//엑터 생성
	static std::shared_ptr<GameEngineNetObject> CreateNetActor(unsigned int _ActorType, int _ObjectID = -1)
	{
		Net_ActorType ActorType = static_cast<Net_ActorType>(_ActorType);
		return CreateNetActor(ActorType, _ObjectID);
	}

	static std::shared_ptr<GameEngineNetObject> CreateNetActor(Net_ActorType _ActorType, int _ObjectID = -1);



	NetworkManager(){}
	virtual ~NetworkManager() = 0{}

	NetworkManager(const NetworkManager& _Other) = delete;
	NetworkManager(NetworkManager&& _Other) noexcept = delete;
	NetworkManager& operator=(const NetworkManager& _Other) = delete;
	NetworkManager& operator=(const NetworkManager&& _Other) noexcept = delete;
};

