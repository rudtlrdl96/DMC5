#pragma once
#include <GameEngineCore/GameEngineComponent.h>
#include "PhysicsActor.h"

// 설명 :
class PhysXSphereComponent : public GameEngineComponent, public PhysicsActor
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

	void CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRotation = { 0.0f , 0.0f });

	void On() override
	{
		GameEngineObjectBase::On();
		m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		m_pDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
		SetMove(VelocityStack);
	}

	void Off() override
	{
		GameEngineObjectBase::Off();
		m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		VelocityStack = GetLinearVelocity();
		m_pDynamic->setLinearVelocity({ 0.0f, 0.0f, 0.0f });
		m_pDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	}

	physx::PxRigidDynamic* GetDynamic()
	{
		return m_pDynamic;
	}

	inline float4 GetWorldPosition()
	{
		return float4(m_pDynamic->getGlobalPose().p.x, m_pDynamic->getGlobalPose().p.y, m_pDynamic->getGlobalPose().p.z);
	}

	inline float4 GetWorldRotation()
	{
		return float4(m_pDynamic->getGlobalPose().q.x, m_pDynamic->getGlobalPose().q.y, m_pDynamic->getGlobalPose().q.z);
	}

	void SetWorldPosition(float4 _Value);
	void AddWorldPosition(float4 _Value);
	void SetWorldRotation(float4 _Value);
	void AddWorldRotation(float4 _Value);

	void SetJump(float _JumpPower);
	void SetMove(float4 _MoveSpeed);
	void SetForce(float4 _MoveSpeed);
	void SetPush(float4 _Push);
	void SetAirState(float _Power); // 몬스터용

	void SetclearForce()
	{
		m_pDynamic->clearForce();
	}

	float4 GetLinearVelocity()
	{
		physx::PxVec3 Vec3 = m_pDynamic->getLinearVelocity();
		return float4{ Vec3.x, Vec3.y, Vec3.z };
	}

	void SetLinearVelocityZero()
	{
		m_pDynamic->setLinearVelocity({ 0.0f, 0.0f, 0.0f });
	}

	//Reset 함수
	void ResetDynamic();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	// Phys액터 생성에 필요한 정보
	physx::PxScene* m_pScene = nullptr;
	physx::PxPhysics* m_pPhysics = nullptr;
	physx::PxMaterial* m_pMaterial = nullptr;

	physx::PxVec3 GeoMetryScale;
	physx::PxVec3 InitVec3;
	physx::PxTransform RecentTransform;

	// 메인플레이어 플래그
	bool IsMain = false;
	bool m_bSpeedLimit = false;

	float4 VelocityStack = float4::ZERO;
};

