#include "PrecompileHeader.h"
#include "GameEngineNetObject.h"

std::atomic<int> GameEngineNetObject::AtomicObjectID;
std::mutex GameEngineNetObject::ObjectLock;
std::map<int, GameEngineNetObject*> GameEngineNetObject::AllNetObjects;

GameEngineNetObject::GameEngineNetObject()
{

}

GameEngineNetObject::~GameEngineNetObject()
{

}

void GameEngineNetObject::InitServerObject()
{
	ObjectID = ++AtomicObjectID;

	ObjectLock.lock();
	AllNetObjects.insert(std::pair<int, GameEngineNetObject*>(ObjectID, this));
	ObjectLock.unlock();

	//아직 이 부분은 이해 안됨, 오브젝트 아이디를 새로 만드는 애들은 다 UserControll이 될 텐데??
	ControllType = NetControllType::UserControll;
}

void GameEngineNetObject::InitClientObject(int _ObjectID)
{
	ObjectID = _ObjectID;

	ObjectLock.lock();
	AllNetObjects.insert(std::pair<int, GameEngineNetObject*>(ObjectID, this));
	ObjectLock.unlock();

	ControllType = NetControllType::ServerControll;
}