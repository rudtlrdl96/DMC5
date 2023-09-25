#pragma once
#include "GameEngineActor.h"

// Ό³Έν :
class GameEngineComponent : public GameEngineObject
{
	friend class GameEngineActor;

public:
	// constrcuter destructer
	GameEngineComponent();
	~GameEngineComponent();

	// delete Function
	GameEngineComponent(const GameEngineComponent& _Other) = delete;
	GameEngineComponent(GameEngineComponent&& _Other) noexcept = delete;
	GameEngineComponent& operator=(const GameEngineComponent& _Other) = delete;
	GameEngineComponent& operator=(GameEngineComponent&& _Other) noexcept = delete;

	class GameEngineActor* GetActor() 
	{
		return Actor;
	}

protected:
	void SetPhysicsActor() 
	{
		Actor->SetPhysicsActor();
	}

private:
	class GameEngineActor* Actor;
	void SetActor(class GameEngineActor* _Actor);

};

