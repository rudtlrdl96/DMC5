#pragma once
#include <GameEngineBase/GameEngineNetServer.h>
#include <GameEngineBase/GameEngineNetClient.h>

class GameEngineLevel;
class GameEngineNetObject;

class NetworkManager
{
public:
	//���� �����尡 �������������� ����
	static inline bool IsServer()
	{
		return IsServerValue;
	}

	//���� �����尡 Ŭ�󽺷������� ����
	static inline bool IsClient()
	{
		return IsClientValue;
	}

	//������ �����ϰų� ����Ǿ����� ����
	static inline bool IsNet()
	{
		NetInst->IsNet();
	}

	static void ServerOpen(int _Port);
	static bool ConnectServer(const std::string_view& _IP, int _Port);

	//���۹��� ��Ŷ�� ó���ϴ� �κ�
	static void Update_PacketProcess(GameEngineLevel* _CurrentLevel)
	{
		//���� ����(��Ŷ ó���Ҷ� ����)
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



	//���� �����尡 �������������� ����
	static bool IsServerValue;
	//���� �����尡 Ŭ�󽺷������� ����
	static bool IsClientValue;

	static unsigned int NetID;


	//Ŭ���̾�Ʈ�� Accept�� �� ó���ؾ� �� �۾�
	static void AcceptCallback(SOCKET _Socket, GameEngineNetServer* _Server);
	
	//��Ŷ���� ������ Ŭ�󿡼� ��� ó���� �� ���
	static void ServerPacketInit();
	static void ClientPacketInit();

	

private:
	static GameEngineLevel* CurLevel;

	inline static GameEngineLevel* GetLevel()
	{
		if (nullptr == CurLevel)
		{
			MsgAssert("NetwortManager�� CurLevel �����Ͱ� nullptr�Դϴ�");
		}

		return CurLevel;
	}

	//ObjectUpdatePacket���� �ش� ���̵��� ���Ͱ� ���ٸ� ����� �Լ�
	static std::shared_ptr<GameEngineNetObject> CreateNetActor(std::shared_ptr<class ObjectUpdatePacket> _Packet);

	NetworkManager(){}
	virtual ~NetworkManager() = 0{}

	NetworkManager(const NetworkManager& _Other) = delete;
	NetworkManager(NetworkManager&& _Other) noexcept = delete;
	NetworkManager& operator=(const NetworkManager& _Other) = delete;
	NetworkManager& operator=(const NetworkManager&& _Other) noexcept = delete;
};

