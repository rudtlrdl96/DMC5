#pragma once

#include "GameEngineComponent.h"
#include "PhysXDefault.h"

// Ό³Έν :
class PhysXActor
{
public:
	// constrcuter destructer
	PhysXActor();
	~PhysXActor();

	// delete Function
	PhysXActor(const PhysXActor& _Other) = delete;
	PhysXActor(PhysXActor&& _Other) noexcept = delete;
	PhysXActor& operator=(const PhysXActor& _Other) = delete;
	PhysXActor& operator=(PhysXActor&& _Other) noexcept = delete;

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

protected:

private:
	std::shared_ptr<GameEngineComponent> PhysXComponent = nullptr;

};

