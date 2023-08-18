#pragma once

#include "GameEngineComponent.h"
#include "PhysXDefault.h"

// ���� :
class PhysXBoxGeometryComponent : public GameEngineComponent, public PhysXDefault
{
public:
	// constrcuter destructer
	PhysXBoxGeometryComponent();
	~PhysXBoxGeometryComponent();

	// delete Function
	PhysXBoxGeometryComponent(const PhysXBoxGeometryComponent& _Other) = delete;
	PhysXBoxGeometryComponent(PhysXBoxGeometryComponent&& _Other) noexcept = delete;
	PhysXBoxGeometryComponent& operator=(const PhysXBoxGeometryComponent& _Other) = delete;
	PhysXBoxGeometryComponent& operator=(PhysXBoxGeometryComponent&& _Other) noexcept = delete;

	void CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRot = float4::ZERO);

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

	void SetIsGravity(bool _Is)
	{
		IsGravity = _Is;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	bool IsGravity = false;

	// Phys���� ������ �ʿ��� ����
	//physx::PxPhysics* physics_;
	//physx::PxScene* scene_;

	physx::PxMaterial* m_pMaterial;
	physx::PxShape* m_pShape;
	physx::PxRigidDynamic* m_pRigidDynamic;

	// �� ������Ʈ�� ������ �ִ� Parent�� ���� ����
	std::weak_ptr<GameEngineActor> ParentActor;

	bool PositionSetFromParentFlag = false;
};

