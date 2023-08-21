#include "PrecompileHeader.h"
#include "GameEngineComponent.h"

GameEngineComponent::GameEngineComponent() 
{
	SetName("GameEngineComponent");
}

GameEngineComponent::~GameEngineComponent() 
{
}


void GameEngineComponent::SetActor(GameEngineActor* _Value) 
{
	Actor = _Value;
}