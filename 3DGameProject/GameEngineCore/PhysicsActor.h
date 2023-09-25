#pragma once

#include "GameEngineComponent.h"
#include "PhysXDefault.h"

// 설명 :
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

	void ReleaseRigid()
	{
		UserDataRelease();

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

	// 특정 행위를 가하지 않으면 sleep상태로 진입 (리소스 절약이라고 함)
	void DynamicPutToSleep()
	{
		m_pDynamic->putToSleep();
	}

	// 원할때 시뮬 돌리게 하기
	void DynamicWakeUp()
	{
		m_pDynamic->wakeUp();
	}

	// sleep 상태인지 체크
	bool IsDynamicSleep()
	{
		return m_pDynamic->isSleeping();
	}

protected:
	std::weak_ptr<class GameEngineActor> ParentActor;
	std::weak_ptr<class PhysicsActor> PhysicsComponent;

	physx::PxRigidDynamic* m_pDynamic = nullptr;
	physx::PxRigidStatic* m_pStatic = nullptr;
	physx::PxShape* m_pShape = nullptr;

private:

};

