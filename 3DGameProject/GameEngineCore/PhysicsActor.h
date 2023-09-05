#pragma once

#include "GameEngineComponent.h"
#include "PhysXDefault.h"

// Ό³Έν :
class PhysicsActor
{
public:
	// constrcuter destructer
	PhysicsActor();
	~PhysicsActor();

	// delete Function
	PhysicsActor(const PhysicsActor& _Other) = delete;
	PhysicsActor(PhysicsActor&& _Other) noexcept = delete;
	PhysicsActor& operator=(const PhysicsActor& _Other) = delete;
	PhysicsActor& operator=(PhysicsActor&& _Other) noexcept = delete;

	template<typename Type>
	void SetPhysXComponent(std::shared_ptr<Type> _Component)
	{
		PhysXComponent = _Component;
	}

	std::shared_ptr<GameEngineComponent> GetPhysXComponent()
	{
		return PhysXComponent;
	}

	void Release()
	{
		if (PhysXComponent != nullptr)
		{
			PhysXComponent->DynamicThis<PhysXDefault>()->ReleaseRigid();
		}
	}

	std::shared_ptr<GameEngineComponent> PhysXComponent = nullptr;
protected:

private:

};

