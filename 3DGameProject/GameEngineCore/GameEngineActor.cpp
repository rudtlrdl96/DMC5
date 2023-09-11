#include "PrecompileHeader.h"
#include "GameEngineActor.h"
#include "GameEngineComponent.h"
#include "GameEngineLevel.h"

GameEngineActor::GameEngineActor()
{
	SetName("GameEngineActor");
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
}

void GameEngineActor::SetOrder(int _Order)
{
	std::shared_ptr<GameEngineActor> ConThis = DynamicThis<GameEngineActor>();

	std::list<std::shared_ptr<GameEngineActor>>& Group = GetLevel()->Actors[GetOrder()];
	Group.remove(ConThis);

	GameEngineObjectBase::SetOrder(_Order);

	GetLevel()->Actors[GetOrder()].push_back(ConThis);
}