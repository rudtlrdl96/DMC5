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

void GameEngineNetObject::Update_ProcessPackets()
{
	ObjectLock.lock();
	MainThreadNetObjects = AllNetObjects;
	ObjectLock.unlock();


	for (const std::pair<int, GameEngineNetObject*>& Pair : MainThreadNetObjects)
	{
		//연결된 적이 없는 경우
		if (false == Pair.second->IsNet())
			continue;

		//처리할 패킷이 없는 경우
		if (true == Pair.second->Packets.empty())
			continue;

		//패킷 처리
		Pair.second->Update_ProcessPacket();
	}

	MainThreadNetObjects.clear();
}


void GameEngineNetObject::Update_SendPackets(float _DeltaTime)
{
	ObjectLock.lock();
	MainThreadNetObjects = AllNetObjects;
	ObjectLock.unlock();

	
	for (const std::pair<int, GameEngineNetObject*>& Pair : MainThreadNetObjects)
	{
		//연결도 안된 경우
		if (false == Pair.second->IsNet())
			continue;

		//패킷을 수신받아 조종당하는 패킷의 경우엔 패킷을 전송하지 않음
		if (NetControllType::PassiveControll == Pair.second->ControllType)
			continue;

		//연결이 끊긴 경우
		if (true == Pair.second->IsDisconnect)
			continue;

		//패킷을 보내기
		Pair.second->Update_SendPacket(_DeltaTime);
	}
	MainThreadNetObjects.clear();
}

GameEngineNetObject* GameEngineNetObject::GetNetObject(int _ID)
{
	std::map<int, GameEngineNetObject*>::iterator FindIter;

	ObjectLock.lock();
	FindIter = AllNetObjects.find(_ID);
	ObjectLock.unlock();

	if (AllNetObjects.end() == FindIter)
	{
		//MsgAssert(std::to_string(_ID) + " ID를 가진 오브젝트는 존재하지 않습니다");
		return nullptr;
	}

	return FindIter->second;
}

std::vector<GameEngineNetObject*> GameEngineNetObject::GetNetObjects(const std::vector<int>& _AllID)
{
	std::map<int, GameEngineNetObject*>::iterator FindIter;
	std::vector<GameEngineNetObject*> Result(_AllID.size(), nullptr);

	ObjectLock.lock();
	for (size_t i = 0; i < _AllID.size(); ++i)
	{
		int ID = _AllID[i];
		FindIter = AllNetObjects.find(ID);
		if (AllNetObjects.end() == FindIter)
			continue;

		Result[i] = FindIter->second;
	}
	ObjectLock.unlock();

	return Result;
}


void GameEngineNetObject::ReleaseNetObject()
{
	ObjectLock.lock();

	std::map<int, GameEngineNetObject*>::iterator ReleaseStartIter = AllNetObjects.begin();
	std::map<int, GameEngineNetObject*>::iterator ReleaseEndIter = AllNetObjects.end();

	while (ReleaseStartIter != ReleaseEndIter)
	{
		GameEngineNetObject* NetObjPtr = ReleaseStartIter->second;
		if (true == NetObjPtr->IsDisconnect || false == NetObjPtr->IsNet())
		{
			ReleaseStartIter = AllNetObjects.erase(ReleaseStartIter);
			continue;
		}

		++ReleaseStartIter;
	}

	ObjectLock.unlock();
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