#include "PrecompileHeader.h"
#include "GameEngineActor.h"
#include "GameEngineComponent.h"
#include "GameEngineLevel.h"

GameEngineActor::GameEngineActor() 
{
}

GameEngineActor::~GameEngineActor() 
{
}

void GameEngineActor::ComponentInit(std::shared_ptr<class GameEngineComponent> _Component, int _Order)
{
	_Component->Actor = this;
	_Component->Level = GetLevel();
	_Component->GetTransform()->SetParent(GetTransform(), false);
	_Component->SetOrder(_Order);
	_Component->Start();

	// PushChild(_Component); << SetParent 문제로 child가 두 번 들어가서 뺌
}

void GameEngineActor::SetOrder(int _Order)
{

	std::shared_ptr<GameEngineActor> ConThis = DynamicThis<GameEngineActor>();

	// 기존의 그룹에서 나를 지우고
	std::list<std::shared_ptr<GameEngineActor>>& Group = GetLevel()->Actors[GetOrder()];
	Group.remove(ConThis);

	GameEngineObjectBase::SetOrder(_Order);

	// 새로운 그룹에 나를 추가한다.
	GetLevel()->Actors[GetOrder()].push_back(ConThis);

}