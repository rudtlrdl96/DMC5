#pragma once

#include "GameEngineComponent.h"
#include "PhysicsActor.h"

// 설명 :
class PhysXCapsuleComponent : public GameEngineComponent, public PhysicsActor
{
public:
	// constrcuter destructer
	PhysXCapsuleComponent();
	~PhysXCapsuleComponent();

	// delete Function
	PhysXCapsuleComponent(const PhysXCapsuleComponent& _Other) = delete;
	PhysXCapsuleComponent(PhysXCapsuleComponent&& _Other) noexcept = delete;
	PhysXCapsuleComponent& operator=(const PhysXCapsuleComponent& _Other) = delete;
	PhysXCapsuleComponent& operator=(PhysXCapsuleComponent&& _Other) noexcept = delete;

	// Component 생성
	void CreatePhysXActors(physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRotation = { 0.0f , 0.0f });

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

	void SetMainPlayerFlags()
	{
		m_pShape->setSimulationFilterData
		(
			physx::PxFilterData
			(
				static_cast<physx::PxU32>(PhysXFilterGroup::Player),
				static_cast<physx::PxU32>(PhysXFilterGroup::Ground),
				static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle), 
				0
			)
		);
	}

	inline bool GetIsPlayerGroundTouch()
	{
		return IsGroundTouch;
	}

	inline bool GetIsPlayerWallTouch()
	{
		return IsWallTouch;
	}

	inline void SetIsPlayerGroundTouch(bool _Is)
	{
		IsGroundTouch = _Is;
	}

	inline void SetIsPlayerWallTouch(bool _Is)
	{
		IsWallTouch = _Is;
	}

	void SetMainPlayer();

	void IsMainPlayerCapsule()
	{
		MainPlayerCapsule = true;
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

	float StandUpTargetYAxisAngle = 0.0f;
	float StandUpStartYAxisAngle = 0.0f;
	float StandUpProgressYAxisAngle = 0.0f;

	physx::PxVec3 InitVec3;
	physx::PxVec3 TargetVec3;
	physx::PxTransform RecentTransform;

	bool IsGroundTouch = false;
	bool IsWallTouch = false;
	bool MainPlayerCapsule = false;

	float4 VelocityStack = float4::ZERO;
};

