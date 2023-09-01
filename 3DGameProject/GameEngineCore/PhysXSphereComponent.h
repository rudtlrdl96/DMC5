#pragma once
#include <GameEngineCore/GameEngineComponent.h>
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

	physx::PxRigidDynamic* CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRotation = { 0.0f , 0.0f });


	void SetMoveSpeed(float4 _MoveSpeed);


	// RigidDynamic을 CCT에서 해제하는 함수
	void SetDynamicIdle();

	inline physx::PxVec3 GetLinearVelocity()
	{
		return m_pRigidDynamic->getLinearVelocity();
	}

	inline void SetlockAxis()
	{
		m_pRigidDynamic->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	}

	inline void SetUnlockAxis()
	{
		// 고정된 축을 해제
		m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
		m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
		m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

		m_pRigidDynamic->addForce(physx::PxVec3(0.0f, 0.01f, 0.0f), physx::PxForceMode::eIMPULSE);
	}

	inline float4 GetWorldPosition()
	{
		return float4(m_pRigidDynamic->getGlobalPose().p.x, m_pRigidDynamic->getGlobalPose().p.y, m_pRigidDynamic->getGlobalPose().p.z);
	}


	void PushImpulse(float4 _ImpulsePower);
	void PushImpulseAtLocalPos(float4 _ImpulsePower, float4 _Pos);


	void TurnOffSpeedLimit()
	{
		m_bSpeedLimit = false;
	}

	void TurnOnSpeedLimit()
	{
		m_bSpeedLimit = true;
	}

	void SwitchSpeedLimit()
	{
		m_bSpeedLimit = !m_bSpeedLimit;
	}

	float4 GetDynamicVelocity()
	{
		physx::PxVec3 Vec3 = m_pRigidDynamic->getLinearVelocity();
		return float4{ Vec3.x, Vec3.y, Vec3.z };
	}

	//회전 움직임 함수
	void SetChangedRot(float4 _Rot);

	//중력끄기
	void TurnOffGravity()
	{
		m_pRigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	}

	//중력키기
	void TurnOnGravity()
	{
		m_pRigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
	}

	//void LockAxis();


	//플레이어 멈추는 함수
	void FreezeDynamic();

	//플레이어 멈추는거 푸는 함수
	void WakeUpDynamic();

	//Reset 함수
	void ResetDynamic();


	void SetMainPlayerFlags()
	{
		m_pShape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::Player),
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
	physx::PxRigidDynamic* m_pRigidDynamic;

	physx::PxMaterial* m_pMaterial = nullptr;
	physx::PxShape* m_pShape = nullptr;

	bool m_bSpeedLimit = false;

	// 이 컴포넌트를 가지고 있는 Parent에 대한 정보
	std::weak_ptr<class GameEngineActor> ParentActor;
	physx::PxVec3 GeoMetryScale;

	//속도제한 함수
	void SpeedLimit();

	physx::PxVec3 InitVec3;
	physx::PxTransform RecentTransform;

	// 메인플레이어 플래그
	bool IsMain = false;
};

