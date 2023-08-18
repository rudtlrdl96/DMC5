#pragma once

//해당 오브젝트를 이 클라이언트가 조종하는지 여부
enum class NetControllType
{
	None,
	UserControll,		//내가 조종하는 오브젝트
	ServerControll,	//패킷을 받아 조종받는 오브젝트
};

class GameEngineNet;

class GameEngineNetObject
{
public:
	//이 ID의 오브젝트가 네트워크 동기화 오브젝트인지 여부
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

	//AtomicObjectID 를 직접 지정할 때 사용
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

	//유저가 이 오브젝트를 조종하는지 여부
	NetControllType ControllType = NetControllType::None;

	//서버가 지정한 ID
	int ObjectID = -1;

	GameEngineNet* Net = nullptr;

	//
	std::list<std::shared_ptr<GameEnginePacket>> Packets;
};

