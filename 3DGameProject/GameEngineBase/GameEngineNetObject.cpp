#include "PrecompileHeader.h"
#include "GameEngineNetObject.h"

#include "GameEnginePacket.h"

std::atomic<int> GameEngineNetObject::AtomicObjectID;
std::mutex GameEngineNetObject::ObjectLock;
std::map<int, GameEngineNetObject*> GameEngineNetObject::AllNetObjects;

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