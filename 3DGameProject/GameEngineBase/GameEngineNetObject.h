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

	//���Ź��� ��Ŷ�� ó���ϴ� �κ�
	static void Update_SendPackets(float _DeltaTime);

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

protected:
	//������ ��Ŷ ���� ó���ϴ� �κ��Դϴ�.
	virtual void Update_SendPacket(float _DeltaTime) {}

private:
	static std::atomic<int> AtomicObjectID;
	static std::mutex ObjectLock;
	static std::map<int, GameEngineNetObject*> AllNetObjects;
	static std::map<int, GameEngineNetObject*> MainThreadNetObjects;

	//������ �� ������Ʈ�� �����ϴ��� ����
	NetControllType ControllType = NetControllType::NetControll;

	//������ ������ ID
	int ObjectID = -1;

	GameEngineNet* Net = nullptr;

	//���Ź��� ��Ŷ���� ������ ��ü�� ó���ϱ� ���� ����Ʈ�� ����(��ġ ť ó�� ���)
	std::list<std::shared_ptr<GameEnginePacket>> Packets;
};

