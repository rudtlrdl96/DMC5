#pragma once

//해당 오브젝트를 이 클라이언트가 조종하는지 여부
enum class NetControllType
{
	None,
	UserControll,		//내가 조종하는 오브젝트
	ServerControll,	//패킷을 받아 조종받는 오브젝트
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

	//서버가 동기화할 오브젝트를 생성할 때 호출시킬 초기화 함수
	void InitServerObject();

	//클라가 패킷을 받아 동기화할 오브젝트를 생성할 때 호출시킬 초기화 함수
	void InitClientObject(int _ObjectID);

	inline void SetControll(NetControllType _ControllType)
	{
		ControllType = _ControllType;
	}

	inline NetControllType  GetControllType() const
	{
		return ControllType;
	}

protected:

private:
	static std::atomic<int> AtomicObjectID;
	static std::mutex ObjectLock;
	static std::map<int, GameEngineNetObject*> AllNetObjects;

	//유저가 이 오브젝트를 조종하는지 여부
	NetControllType ControllType = NetControllType::None;

	//서버가 지정한 ID
	int ObjectID = -1;
};

