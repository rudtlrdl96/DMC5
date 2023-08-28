#pragma once

#include "GameEngineComponent.h"
#include "PhysXDefault.h"

// 설명 :
class PhysXCapsuleComponent : public GameEngineComponent, public PhysXDefault
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
	void CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRotation = { 0.0f , 0.0f });

	physx::PxRigidDynamic* GetDynamic()
	{
		return m_pDynamic;
	}

	void SetclearForce()
	{
		m_pDynamic->clearForce();
	}

	void SetWorldPosition(float4 _Value);

	void SetWorldRotation(float4 _Value);

	inline float4 GetWorldPosition()
	{
		return float4(m_pDynamic->getGlobalPose().p.x, m_pDynamic->getGlobalPose().p.y, m_pDynamic->getGlobalPose().p.z);
	}

	inline float4 GetWorldRotation()
	{
		return float4(m_pDynamic->getGlobalPose().q.x, m_pDynamic->getGlobalPose().q.y, m_pDynamic->getGlobalPose().q.z);
	}

	void SetJump(float _JumpPower);
	void SetMove(float4 _MoveSpeed);
	void SetPush(float4 _Push);

	void SetSpeedLimitValue(float _Value)
	{
		SpeedLimitValue = _Value;
	}

	inline physx::PxVec3 GetLinearVelocity()
	{
		return m_pDynamic->getLinearVelocity();
	}

	float4 GetDynamicVelocity()
	{
		physx::PxVec3 Vec3 = m_pDynamic->getLinearVelocity();
		return float4{ Vec3.x, Vec3.y, Vec3.z };
	}

	//플레이어 멈추는 함수
	void FreezeDynamic();

	//플레이어 멈추는거 푸는 함수
	void WakeUpDynamic();

	//Reset 함수
	void ResetDynamic();

	inline void SetlockAxis()
	{
		m_pDynamic->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	}

	inline void SetUnlockAxis()
	{
		// 고정된 축을 해제
		m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
		m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

		m_pDynamic->addForce(physx::PxVec3(0.0f, 0.01f, 0.0f), physx::PxForceMode::eIMPULSE);
	}

	void SetPlayerStartPos(float4 _Pos);
	
	void PushImpulseAtLocalPos(float4 _ImpulsePower, float4 _Pos);

	//중력끄기
	void TurnOffGravity()
	{
		m_pDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	}

	//중력키기
	void TurnOnGravity()
	{
		m_pDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
	}

	void SetMainPlayerFlags()
	{
		m_pShape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic),
			static_cast<physx::PxU32>(PhysXFilterGroup::Ground),
			static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle), 0));
		//m_pInstshape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::Player), 0, 0, 0));
		//m_pFaceshape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::PlayerFace), 0, 0, 0));
		//m_pHeadshape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::PlayerHead), 0, 0, 0));
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	//void Render() override {}

private:
	// Phys액터 생성에 필요한 정보
	physx::PxScene* m_pScene = nullptr;
	physx::PxPhysics* m_pPhysics = nullptr;
	physx::PxControllerManager* m_pCtrManager = nullptr;

	physx::PxMaterial* m_pMaterial = nullptr;
	physx::PxShape* m_pShape = nullptr;
	physx::PxRigidDynamic* m_pDynamic = nullptr;

	// 이 컴포넌트를 가지고 있는 Parent에 대한 정보
	std::weak_ptr<class GameEngineActor> ParentActor;
	physx::PxVec3 GeoMetryScale;

	//속도제한 함수
	void SpeedLimit();

	float SpeedLimitValue = 150.0f;

	float StandUpTargetYAxisAngle = 0.0f;
	float StandUpStartYAxisAngle = 0.0f;
	float StandUpProgressYAxisAngle = 0.0f;

	physx::PxVec3 InitVec3;
	physx::PxVec3 TargetVec3;
	physx::PxTransform RecentTransform;

};

