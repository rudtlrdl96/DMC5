#pragma once

//�ش� ������Ʈ�� �� Ŭ���̾�Ʈ�� �����ϴ��� ����
enum class NetControllType
{
	None,
	UserControll,		//���� �����ϴ� ������Ʈ
	ServerControll,	//��Ŷ�� �޾� �����޴� ������Ʈ
};

class GameEngineNet;

class GameEngineNetObject
{
public:
	//�� ID�� ������Ʈ�� ��Ʈ��ũ ����ȭ ������Ʈ���� ����
	static bool IsNetObject(int _Id)
	{
		return AllNetObjects.contains(_Id);
	}

	GameEngineNetObject();
	~GameEngineNetObject();

	GameEngineNetObject(const GameEngineNetObject& _Other) = delete;
	GameEngineNetObject(GameEngineNetObject&& _Other) noexcept = delete;
	GameEngineNetObject& operator=(const GameEngineNetObject& _Other) = delete;
	GameEngineNetObject& operator=(const GameEngineNetObject&& _Other) noexcept = delete;

	//AtomicObjectID �� ���� ������ �� ���
	static int CreateServerID()
	{
		return ++AtomicObjectID;
	}


	void InitNetObject(int _ObjectID, GameEngineNet* _Net);



	inline void SetControll(NetControllType _ControllType)
	{
		ControllType = _ControllType;
	}

	inline NetControllType  GetControllType() const
	{
		return ControllType;
	}

	inline int GetNetObjectID() const
	{
		return ObjectID;
	}

	inline bool IsNet() const
	{
		return nullptr != Net;
	}

	inline GameEngineNet* GetNet() const
	{
		return Net;
	}

	//
	void PushPacket(std::shared_ptr<class GameEnginePacket> _Packet);

protected:

private:
	static std::atomic<int> AtomicObjectID;
	static std::mutex ObjectLock;
	static std::map<int, GameEngineNetObject*> AllNetObjects;

	//������ �� ������Ʈ�� �����ϴ��� ����
	NetControllType ControllType = NetControllType::None;

	//������ ������ ID
	int ObjectID = -1;

	GameEngineNet* Net = nullptr;

	//
	std::list<std::shared_ptr<GameEnginePacket>> Packets;
};

