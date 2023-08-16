#pragma once

//�ش� ������Ʈ�� �� Ŭ���̾�Ʈ�� �����ϴ��� ����
enum class NetControllType
{
	None,
	UserControll,		//���� �����ϴ� ������Ʈ
	ServerControll,	//��Ŷ�� �޾� �����޴� ������Ʈ
};

class GameEngineNetObject
{
public:
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


	//������ ����ȭ�� ������Ʈ�� ������ �� ȣ���ų �ʱ�ȭ �Լ�
	void InitServerObject();

	//Ŭ�� ��Ŷ�� �޾� ����ȭ�� ������Ʈ�� ������ �� ȣ���ų �ʱ�ȭ �Լ�
	void InitClientObject(int _ObjectID);

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

protected:

private:
	static std::atomic<int> AtomicObjectID;
	static std::mutex ObjectLock;
	static std::map<int, GameEngineNetObject*> AllNetObjects;

	//������ �� ������Ʈ�� �����ϴ��� ����
	NetControllType ControllType = NetControllType::None;

	//������ ������ ID
	int ObjectID = -1;
};

