#pragma once
#include <GameEngineBase/GameEngineNetServer.h>
#include <GameEngineBase/GameEngineNetClient.h>

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

	static void Update_PacketProcess()
	{
		if (nullptr == NetInst)
			return;

		NetInst->UpdatePacket();
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



	//Ŭ���̾�Ʈ�� Accept�� �� ó���ؾ� �� �۾�
	static void AcceptCallback(SOCKET _Socket, GameEngineNetServer* _Server);
	
	//��Ŷ���� ������ Ŭ�󿡼� ��� ó���� �� ���
	static void ServerPacketInit();
	static void ClientPacketInit();

	

private:
	NetworkManager(){}
	virtual ~NetworkManager() = 0{}

	NetworkManager(const NetworkManager& _Other) = delete;
	NetworkManager(NetworkManager&& _Other) noexcept = delete;
	NetworkManager& operator=(const NetworkManager& _Other) = delete;
	NetworkManager& operator=(const NetworkManager&& _Other) noexcept = delete;
};

