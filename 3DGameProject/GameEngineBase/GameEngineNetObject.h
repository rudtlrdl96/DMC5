#pragma once

//해당 오브젝트를 이 클라이언트가 조종하는지 여부
enum class NetControllType
{
	NetControll,		//서버로 부터 패킷을 받아 조종당하는 오브젝트
	UserControll,		//내가 조종하는 오브젝트
};

class GameEngineNet;
class GameEnginePacket;

class GameEngineNetObject
{
public:
	//패킷으로부터 오브젝트 아이디를 얻어온 후, 해당 오브젝트의 자료구조에 패킷을 집어넣는다
	static void PushNetObjectPacket(std::shared_ptr<GameEnginePacket> _Packet);

	//이 ID의 오브젝트가 네트워크 동기화 오브젝트인지 여부
	static bool IsNetObject(int _Id)
	{
		return AllNetObjects.contains(_Id);
	}

	//수신받은 패킷을 처리하는 부분
	static void Update_SendPackets(float _DeltaTime);

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

	//네트워크와 연결
	void InitNetObject(int _ObjectID, GameEngineNet* _Net);


	// 네트워크 상에서 컨트롤을 클라이언트가 담당할 것인지, 서버가 담당할 것인지 여부
	inline void SetControll(NetControllType _ControllType)
	{
		ControllType = _ControllType;
	}

	inline NetControllType  GetControllType() const
	{
		return ControllType;
	}

	//유저가 직접 조종하는 객체로 설정
	inline void SetUserControllType()
	{
		ControllType = NetControllType::UserControll;
	}

	//이 객체의 고유 네트워크 아이디(번호) 알려주기
	inline int GetNetObjectID() const
	{
		return ObjectID;
	}

	//네트워크와 연결된 객체인지 여부
	inline bool IsNet() const
	{
		return nullptr != Net;
	}

	inline GameEngineNet* GetNet() const
	{
		return Net;
	}

	//자료구조에 패킷을 보관
	void PushPacket(std::shared_ptr<class GameEnginePacket> _Packet);

	//처리해야할 패킷이 있는지 여부
	inline bool IsPacket() const
	{
		return Packets.size();
	}

	//맨 앞쪽의 패킷을 꺼내고 Pop한다
	template<typename PacketType>
	std::shared_ptr<PacketType> PopFirstPacket()
	{
		std::shared_ptr<PacketType> PacketReturn = std::dynamic_pointer_cast<PacketType>(Packets.front());
		Packets.pop_front();
		return PacketReturn;
	}

	//맨 앞쪽의 패킷 타입을 알아온다
	template<typename EnumType>
	EnumType GetFirstPacketType()
	{
		return static_cast<EnumType>(Packets.front()->GetPacketID());
	}

protected:
	//서버로 패킷 전송 처리하는 부분입니다.
	virtual void Update_SendPacket(float _DeltaTime) {}

private:
	static std::atomic<int> AtomicObjectID;
	static std::mutex ObjectLock;
	static std::map<int, GameEngineNetObject*> AllNetObjects;
	static std::map<int, GameEngineNetObject*> MainThreadNetObjects;

	//유저가 이 오브젝트를 조종하는지 여부
	NetControllType ControllType = NetControllType::NetControll;

	//서버가 지정한 ID
	int ObjectID = -1;

	GameEngineNet* Net = nullptr;

	//수신받은 패킷들을 각각의 객체가 처리하기 위해 리스트에 보관(마치 큐 처럼 사용)
	std::list<std::shared_ptr<GameEnginePacket>> Packets;
};

