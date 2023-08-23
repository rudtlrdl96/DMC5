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
		return NetInst->IsNet();
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

	//Update��Ŷ�� ������ �� �������̽��� �̿��ؼ� �����ֽø� �˴ϴ�.
	static void SendUpdatePacket(GameEngineNetObject* _NetObj, GameEngineTransform* TransPtr, float _TimeScale);

	//���� ������Ʈ ��Ŷ�� ó���ϴ� �κ�
	static void FlushUpdatePacket();

	//Net_ActorType Ÿ���� ���͸� �����ϴ� �κ��Դϴ�.
	static void SendCreatePacket(Net_ActorType _Type, const float4& _Position = float4::ZERO, const float4& _Rotation = float4::ZERO);

	//�̹� �����Ǿ� �ִ� ������Ʈ�� ������ ������ŵ�ϴ�.
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
	//������Ʈ ��Ŷ�� ������ �ߺ��� ���ϱ� ���� �� <������Ʈ ���̵�, ������Ʈ ������Ʈ ��Ŷ ������>
	static std::map<unsigned int, std::shared_ptr<class ObjectUpdatePacket>> AllUpdatePacket;

	static GameEngineLevel* CurLevel;

	inline static GameEngineLevel* GetLevel()
	{
		if (nullptr == CurLevel)
		{
			MsgAssert("NetwortManager�� CurLevel �����Ͱ� nullptr�Դϴ�");
		}

		return CurLevel;
	}

	//���� ����
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

