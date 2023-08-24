#pragma once

//�ش� ������Ʈ�� �� Ŭ���̾�Ʈ�� �����ϴ��� ����
enum class NetControllType
{
	NetControll,		//������ ���� ��Ŷ�� �޾� �������ϴ� ������Ʈ
	UserControll,		//���� �����ϴ� ������Ʈ
};

class GameEngineNet;
class GameEnginePacket;

class GameEngineNetObject
{
public:
	//��Ŷ���κ��� ������Ʈ ���̵� ���� ��, �ش� ������Ʈ�� �ڷᱸ���� ��Ŷ�� ����ִ´�
	static void PushNetObjectPacket(std::shared_ptr<GameEnginePacket> _Packet);

	//�� ID�� ������Ʈ�� ��Ʈ��ũ ����ȭ ������Ʈ���� ����
	static bool IsNetObject(int _Id)
	{
		return AllNetObjects.contains(_Id);
	}

	//Deathó���� NetObeject���� AllNetObjects���� �����մϴ�
	static void Update_ProcessPackets();

	//��� Actor���� Update�� ���� ���� ��Ŷ�� �����ϴ� �κ�
	static void Update_SendPackets(float _DeltaTime);

	//ID�� GameEngineNetObject*�� �޾ƿ�
	static GameEngineNetObject* GetNetObject(int _ID);

	static void ReleaseNetObject();

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

	//��Ʈ��ũ�� ����
	void InitNetObject(int _ObjectID, GameEngineNet* _Net);


	// ��Ʈ��ũ �󿡼� ��Ʈ���� Ŭ���̾�Ʈ�� ����� ������, ������ ����� ������ ����
	inline void SetControll(NetControllType _ControllType)
	{
		ControllType = _ControllType;
	}

	inline NetControllType  GetControllType() const
	{
		return ControllType;
	}

	//������ ���� �����ϴ� ��ü�� ����
	inline void SetUserControllType()
	{
		ControllType = NetControllType::UserControll;
	}

	//�� ��ü�� ���� ��Ʈ��ũ ���̵�(��ȣ) �˷��ֱ�
	inline int GetNetObjectID() const
	{
		return ObjectID;
	}

	//��Ʈ��ũ�� ����� ��ü���� ����
	inline bool IsNet() const
	{
		return nullptr != Net;
	}

	inline GameEngineNet* GetNet() const
	{
		return Net;
	}

	//�ڷᱸ���� ��Ŷ�� ����
	void PushPacket(std::shared_ptr<class GameEnginePacket> _Packet);

	//ó���ؾ��� ��Ŷ�� �ִ��� ����
	inline bool IsPacket() const
	{
		return Packets.size();
	}

	//�� ������ ��Ŷ�� ������ Pop�Ѵ�
	template<typename PacketType>
	std::shared_ptr<PacketType> PopFirstPacket()
	{
		std::shared_ptr<PacketType> PacketReturn = std::dynamic_pointer_cast<PacketType>(Packets.front());
		Packets.pop_front();
		return PacketReturn;
	}

	//�� ������ ��Ŷ Ÿ���� �˾ƿ´�
	template<typename EnumType>
	EnumType GetFirstPacketType()
	{
		return static_cast<EnumType>(Packets.front()->GetPacketID());
	}

	unsigned int GetNetObjectType() const
	{
		return ObjectType;
	}

	//�� ������Ʈ�� ������ ���� �����ϴ�.(ȣ���� �ڷδ� Update��Ŷ ����/������ �ȵ˴ϴ�)
	inline void NetDisconnect()
	{
		IsDisconnect = true;
	}


protected:
	//���Ź��� ��Ŷ�� ó���ϴ� �κ��Դϴ�.(���� ������Ʈ �� ó���� ȣ��˴ϴ�)
	virtual void Update_ProcessPacket(){}

	//������ ��Ŷ ���� ó���ϴ� �κ��Դϴ�.(��� ������Ʈ �� �������� ȣ��˴ϴ�)
	virtual void Update_SendPacket(float _DeltaTime) {}

	template <typename EnumType>
	inline void SetNetObjectType(EnumType _ActorType)
	{
		ObjectType = static_cast<unsigned int>(_ActorType);
	}

	


private:
	static std::atomic<int> AtomicObjectID;
	static std::mutex ObjectLock;
	static std::map<int, GameEngineNetObject*> AllNetObjects;
	static std::map<int, GameEngineNetObject*> MainThreadNetObjects;

	//������ �� ������Ʈ�� �����ϴ��� ����
	NetControllType ControllType = NetControllType::NetControll;

	//������ ������ ID
	int ObjectID = -1;

	//� Ŭ������ ������Ʈ���� ǥ��
	unsigned int ObjectType = -1;

	GameEngineNet* Net = nullptr;

	//Release������ ��ȯ�Ҷ� AllNetObjects���� �� ������Ʈ�� ������ ���� bool
	bool IsDisconnect = false;

	//���Ź��� ��Ŷ���� ������ ��ü�� ó���ϱ� ���� ����Ʈ�� ����(��ġ ť ó�� ���)
	std::list<std::shared_ptr<GameEnginePacket>> Packets;
};

