#pragma once

#include "GameEngineComponent.h"
#include "PhysicsActor.h"

// 설명 :
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

	inline float4 GetWorldPosition()
	{
		return float4(m_pStatic->getGlobalPose().p.x, m_pStatic->getGlobalPose().p.y, m_pStatic->getGlobalPose().p.z);
	}

	inline float4 GetWorldRotation()
	{
		return float4(m_pStatic->getGlobalPose().q.x, m_pStatic->getGlobalPose().q.y, m_pStatic->getGlobalPose().q.z);
	}

	void SetWorldPosition(float4 _Value);
	void AddWorldPosition(float4 _Value);
	void SetWorldRotation(float4 _Value);
	void AddWorldRotation(float4 _Value);

	inline void SetPositionSetFromParentFlag(bool _Flag)
	{
		PositionSetFromParentFlag = _Flag;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	// Phys액터 생성에 필요한 정보
	physx::PxPhysics* m_pPhysics = nullptr;
	physx::PxScene* m_pScene = nullptr;

	physx::PxMaterial* m_pMaterial = nullptr;
	physx::PxShape* m_pShape = nullptr;

	bool PositionSetFromParentFlag = false;

};

