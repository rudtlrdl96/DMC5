#pragma once

#include "GameEngineComponent.h"
#include "PhysXDefault.h"

// 설명 :
class PhysXSphereComponent : public GameEngineComponent, public PhysXDefault
{
public:
	// constrcuter destructer
	PhysXSphereComponent();
	~PhysXSphereComponent();

	// delete Function
	PhysXSphereComponent(const PhysXSphereComponent& _Other) = delete;
	PhysXSphereComponent(PhysXSphereComponent&& _Other) noexcept = delete;
	PhysXSphereComponent& operator=(const PhysXSphereComponent& _Other) = delete;
	PhysXSphereComponent& operator=(PhysXSphereComponent&& _Other) noexcept = delete;

	void CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRot = float4::ZERO);

	physx::PxRigidDynamic* GetDynamic()
	{
		return m_pRigidDynamic;
	}

	inline void ReleasePhysX()
	{
		if (true == m_pRigidDynamic->isReleasable())
		{
			m_pRigidDynamic->release();
			ParentActor.lock()->Death();
		}
	}

	inline void SetPositionSetFromParentFlag(bool _Flag)
	{
		PositionSetFromParentFlag = _Flag;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	physx::PxMaterial* m_pMaterial;
	physx::PxShape* m_pShape;
	physx::PxRigidDynamic* m_pRigidDynamic;

	// 이 컴포넌트를 가지고 있는 Parent에 대한 정보
	std::weak_ptr<GameEngineActor> ParentActor;

	bool PositionSetFromParentFlag = true;

};

