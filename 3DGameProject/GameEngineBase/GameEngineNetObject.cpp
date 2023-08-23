#include "PrecompileHeader.h"
#include "GameEngineNetObject.h"

#include "GameEnginePacket.h"

std::atomic<int> GameEngineNetObject::AtomicObjectID;
std::mutex GameEngineNetObject::ObjectLock;
std::map<int, GameEngineNetObject*> GameEngineNetObject::AllNetObjects;
std::map<int, GameEngineNetObject*> GameEngineNetObject::MainThreadNetObjects;



void GameEngineNetObject::PushNetObjectPacket(std::shared_ptr<GameEnginePacket> _Packet)
{
	//��Ŷ���κ��� ������Ʈ ���̵� ���� ��
	int Id = _Packet->GetObjectID();
	if (false == AllNetObjects.contains(Id))
	{
		MsgAssert("�������� �ʴ� ������Ʈ�� ��Ŷ�� ���ƿԽ��ϴ�.");
		return;
	}

	//�ش� ������Ʈ�� �ڷᱸ���� ��Ŷ�� ����ִ´�
	AllNetObjects[Id]->PushPacket(_Packet);
}

void GameEngineNetObject::Update_ProcessPackets()
{
	ObjectLock.lock();
	MainThreadNetObjects = AllNetObjects;
	ObjectLock.unlock();


	for (const std::pair<int, GameEngineNetObject*>& Pair : MainThreadNetObjects)
	{
		if (false == Pair.second->IsNet())
			continue;

		if (true == Pair.second->IsDeath)
			continue;

		if (true == Pair.second->Packets.empty())
			continue;

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
		if (false == Pair.second->IsNet())
			continue;

		if (true == Pair.second->IsDeath)
			continue;

		Pair.second->Update_SendPacket(_DeltaTime);
	}
	MainThreadNetObjects.clear();
}

void GameEngineNetObject::ReleaseNetObject()
{
	//�ٵ� �̷��� ũ�� Lock�� �����鼭 ���� ����� �� �ʿ䰡 ������?

	ObjectLock.lock();

	std::map<int, GameEngineNetObject*>::iterator ReleaseStartIter = AllNetObjects.begin();
	std::map<int, GameEngineNetObject*>::iterator ReleaseEndIter = AllNetObjects.end();

	while (ReleaseStartIter != ReleaseEndIter)
	{
		GameEngineNetObject* NetObjPtr = ReleaseStartIter->second;
		if (true == NetObjPtr->IsDeath || false == NetObjPtr->IsNet())
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