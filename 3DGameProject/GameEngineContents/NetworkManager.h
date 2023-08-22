#pragma once
#include <GameEngineBase/GameEngineNetServer.h>
#include <GameEngineBase/GameEngineNetClient.h>

class GameEngineLevel;
class GameEngineNetObject;

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
		NetInst->IsNet();
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

	

protected:



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
	static GameEngineLevel* CurLevel;

	inline static GameEngineLevel* GetLevel()
	{
		if (nullptr == CurLevel)
		{
			MsgAssert("NetwortManager의 CurLevel 포인터가 nullptr입니다");
		}

		return CurLevel;
	}

	//ObjectUpdatePacket에서 해당 아이디의 엑터가 없다면 만드는 함수
	static std::shared_ptr<GameEngineNetObject> CreateNetActor(std::shared_ptr<class ObjectUpdatePacket> _Packet);

	NetworkManager(){}
	virtual ~NetworkManager() = 0{}

	NetworkManager(const NetworkManager& _Other) = delete;
	NetworkManager(NetworkManager&& _Other) noexcept = delete;
	NetworkManager& operator=(const NetworkManager& _Other) = delete;
	NetworkManager& operator=(const NetworkManager&& _Other) noexcept = delete;
};

