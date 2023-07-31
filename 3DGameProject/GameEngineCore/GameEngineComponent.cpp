#include "PrecompileHeader.h"
#include "GameEngineComponent.h"

GameEngineComponent::GameEngineComponent() 
{
}

GameEngineComponent::~GameEngineComponent() 
{
}


void GameEngineComponent::SetActor(GameEngineActor* _Value) 
{
	Actor = _Value;
}