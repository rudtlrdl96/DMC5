#pragma once

#include <atomic>
#include <mutex>

//�ش� ������Ʈ�� �� Ŭ���̾�Ʈ�� �����ϴ��� ����
enum class NetControllType
{
	PassiveControll,		//������ ���� ��Ŷ�� �޾� �������ϴ� ������Ʈ
	ActiveControll,		//���� �����ϴ� ������Ʈ
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

	//���Ź��� ��Ŷ�� ó���մϴ�
	static void Update_ProcessPackets();

	//��� Actor���� Update�� ���� ���� ��Ŷ�� �����ϴ� �κ�
	static void Update_SendPackets(float _DeltaTime);

	//ID�� GameEngineNetObject*�� �޾ƿ�
	static GameEngineNetObject* GetNetObject(int _ID);

	//ID�� GameEngineNetObject*�� �޾ƿ�
	static std::vector<GameEngineNetObject*> GetNetObjects(const std::vector<int>& _AllID);

	//Deathó���� NetObeject���� AllNetObjects���� �����մϴ�
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

	//NetControllType�� ����� �� ȣ��Ǵ� (��ȸ��)�ݹ��Դϴ�.
	inline void SetControllCallBack(NetControllType _ControllType, std::function<void()> _CallBack)
	{
		ControllCallBack[_ControllType] = _CallBack;
	}

	// ��Ʈ��ũ �󿡼� ��Ʈ���� Ŭ���̾�Ʈ�� ����� ������, ������ ����� ������ ����
	inline void SetControll(NetControllType _ControllType)
	{
		ControllType = _ControllType;

		std::map<NetControllType, std::function<void()>>::iterator FindIter = ControllCallBack.find(_ControllType);
		if (ControllCallBack.end() == FindIter)
			return;
		
		if (nullptr == FindIter->second)
			return;

		FindIter->second();
		FindIter->second = nullptr;
	}

	inline NetControllType  GetControllType() const
	{
		return ControllType;
	}

	//������ ���� �����ϴ� ��ü�� ����
	inline void SetUserControllType()
	{
		SetControll(NetControllType::ActiveControll);
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
	std::shared_ptr<PacketType> PopFirstPacketAsCast()
	{
		std::shared_ptr<PacketType> PacketReturn = std::dynamic_pointer_cast<PacketType>(Packets.front());
		Packets.pop_front();
		return PacketReturn;
	}

	std::shared_ptr<GameEnginePacket> PopFirstPacket()
	{
		std::shared_ptr<GameEnginePacket> PacketReturn = Packets.front();
		Packets.pop_front();
		return PacketReturn;
	}

	//�� ������ ��Ŷ Ÿ���� �˾ƿ´�
	template<typename EnumType>
	EnumType GetFirstPacketType()
	{
		return static_cast<EnumType>(Packets.front()->GetPacketID());
	}

	//�� ������Ʈ�� ������ ���� �����ϴ�.(ȣ���� �ڷδ� Update��Ŷ ����/������ �ȵ˴ϴ�)
	inline void NetDisconnect()
	{
		IsDisconnect = true;
	}

	inline bool IsNetDisconnected() const
	{
		return IsDisconnect;
	}

protected:
	//���Ź��� ��Ŷ�� ó���ϴ� �κ��Դϴ�.(���� ������Ʈ �� ó���� ȣ��˴ϴ�)
	virtual void Update_ProcessPacket() = 0;

	//������ ��Ŷ ���� ó���ϴ� �κ��Դϴ�.(��� ������Ʈ �� �������� ȣ��˴ϴ�)
	virtual void Update_SendPacket(float _DeltaTime) = 0;


private:
	static std::atomic<int> AtomicObjectID;
	static std::mutex ObjectLock;
	static std::map<int, GameEngineNetObject*> AllNetObjects;
	static std::map<int, GameEngineNetObject*> MainThreadNetObjects;

	//������ �� ������Ʈ�� �����ϴ��� ����
	NetControllType ControllType = NetControllType::PassiveControll;

	//������ ������ ID
	int ObjectID = -1;

	GameEngineNet* Net = nullptr;

	//Release������ ��ȯ�Ҷ� AllNetObjects���� �� ������Ʈ�� ������ ���� bool
	bool IsDisconnect = false;

	//���Ź��� ��Ŷ���� ������ ��ü�� ó���ϱ� ���� ����Ʈ�� ����(��ġ ť ó�� ���)
	std::list<std::shared_ptr<GameEnginePacket>> Packets;

	//ControllType�� ����� �� ȣ��Ǵ� �ݹ�
	std::map<NetControllType, std::function<void()>> ControllCallBack;
};

