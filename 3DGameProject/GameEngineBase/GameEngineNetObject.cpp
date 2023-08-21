#include "PrecompileHeader.h"
#include "GameEngineNetObject.h"

#include "GameEnginePacket.h"

std::atomic<int> GameEngineNetObject::AtomicObjectID;
std::mutex GameEngineNetObject::ObjectLock;
std::map<int, GameEngineNetObject*> GameEngineNetObject::AllNetObjects;
std::map<int, GameEngineNetObject*> GameEngineNetObject::MainThreadNetObjects;

void GameEngineNetObject::PushNetObjectPacket(std::shared_ptr<GameEnginePacket> _Packet)
{
	//패킷으로부터 오브젝트 아이디를 얻어온 후
	int Id = _Packet->GetObjectID();
	if (false == AllNetObjects.contains(Id))
	{
		MsgAssert("존재하지 않는 오브젝트에 패킷이 날아왔습니다.");
		return;
	}

	//해당 오브젝트의 자료구조에 패킷을 집어넣는다
	AllNetObjects[Id]->PushPacket(_Packet);
}


void GameEngineNetObject::Update_SendPackets(float _DeltaTime)
{
	ObjectLock.lock();
	MainThreadNetObjects = AllNetObjects;
	ObjectLock.unlock();

	
	for (const std::pair<int, GameEngineNetObject*>& Pair : MainThreadNetObjects)
	{
		if (false == Pair.second->IsNet())
			continue;

		Pair.second->Update_SendPacket(_DeltaTime);
	}

	MainThreadNetObjects.clear();
}




GameEngineNetObject::GameEngineNetObject()
{

}

GameEngineNetObject::~GameEngineNetObject()
{

}

void GameEngineNetObject::InitNetObject(int _ObjectID, GameEngineNet* _Net)
{
	ObjectID = _ObjectID;

	ObjectLock.lock();
	AllNetObjects.insert(std::pair<int, GameEngineNetObject*>(ObjectID, this));
	ObjectLock.unlock();

	Net = _Net;
}

void GameEngineNetObject::PushPacket(std::shared_ptr<GameEnginePacket> _Packet)
{
	Packets.push_back(_Packet);
}