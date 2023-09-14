#pragma once

#include "GameEngineComponent.h"
#include "PhysXDefault.h"

// Ό³Έν :
class PhysicsActor : public PhysXDefault
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

	//std::shared_ptr<PhysicsActor> GetPhysXComponent()
	//{
	//	return PhysXComponent;
	//}

	void ReleaseRigid()
	{
		if (m_pStatic != nullptr && m_pStatic->isReleasable())
		{
			m_pStatic->release();
		}
		if (m_pDynamic != nullptr && m_pDynamic->isReleasable())
		{
			m_pDynamic->release();
		}
	}

	physx::PxRigidDynamic* GetDynamic()
	{
		return m_pDynamic;
	}

	physx::PxRigidStatic* GetStatic()
	{
		return m_pStatic;
	}

	physx::PxShape* GetShape()
	{
		return m_pShape;
	}

	void UserDataRelease()
	{
		m_pShape->userData = nullptr;
	}

	void Release()
	{
		PhysicsComponent.lock()->ReleaseRigid();
	}

protected:
	std::weak_ptr<class GameEngineActor> ParentActor;
	std::weak_ptr<class PhysicsActor> PhysicsComponent;

	physx::PxRigidDynamic* m_pDynamic = nullptr;
	physx::PxRigidStatic* m_pStatic = nullptr;
	physx::PxShape* m_pShape = nullptr;

private:

};

