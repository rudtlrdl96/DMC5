#pragma once

#include "GameEngineComponent.h"
#include "PhysicsActor.h"

// ���� :
class PhysXBoxComponent : public GameEngineComponent, public PhysicsActor
{
public:
	// constrcuter destructer
	PhysXBoxComponent();
	~PhysXBoxComponent();

	// delete Function
	PhysXBoxComponent(const PhysXBoxComponent& _Other) = delete;
	PhysXBoxComponent(PhysXBoxComponent&& _Other) noexcept = delete;
	PhysXBoxComponent& operator=(const PhysXBoxComponent& _Other) = delete;
	PhysXBoxComponent& operator=(PhysXBoxComponent&& _Other) noexcept = delete;

	void CreatePhysXActors(physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRot = float4::ZERO);

	physx::PxRigidStatic* GetStatic()
	{
		return m_pStatic;
	}

	inline void SetPositionSetFromParentFlag(bool _Flag)
	{
		PositionSetFromParentFlag = _Flag;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	// Phys���� ������ �ʿ��� ����
	physx::PxPhysics* m_pPhysics = nullptr;
	physx::PxScene* m_pScene = nullptr;

	physx::PxMaterial* m_pMaterial = nullptr;
	physx::PxShape* m_pShape = nullptr;

	bool PositionSetFromParentFlag = true;

};

