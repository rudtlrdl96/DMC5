#pragma once

#include "GameEngineComponent.h"
#include "PhysXDefault.h"

// ���� :
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

	// Component ����
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

	//�÷��̾� ���ߴ� �Լ�
	void FreezeDynamic();

	//�÷��̾� ���ߴ°� Ǫ�� �Լ�
	void WakeUpDynamic();

	//Reset �Լ�
	void ResetDynamic();

	inline void SetlockAxis()
	{
		m_pDynamic->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	}

	inline void SetUnlockAxis()
	{
		// ������ ���� ����
		m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
		m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

		m_pDynamic->addForce(physx::PxVec3(0.0f, 0.01f, 0.0f), physx::PxForceMode::eIMPULSE);
	}

	void SetPlayerStartPos(float4 _Pos);
	
	void PushImpulseAtLocalPos(float4 _ImpulsePower, float4 _Pos);

	//�߷²���
	void TurnOffGravity()
	{
		m_pDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	}

	//�߷�Ű��
	void TurnOnGravity()
	{
		m_pDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
	}

	void SetMainPlayerFlags()
	{
		m_pShape->setSimulationFilterData
		(
			physx::PxFilterData
			(
				static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic),
				static_cast<physx::PxU32>(PhysXFilterGroup::Ground),
				static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle), 
				0
			)
		);
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	// Phys���� ������ �ʿ��� ����
	physx::PxScene* m_pScene = nullptr;
	physx::PxPhysics* m_pPhysics = nullptr;
	physx::PxControllerManager* m_pCtrManager = nullptr;

	physx::PxMaterial* m_pMaterial = nullptr;
	physx::PxShape* m_pShape = nullptr;
	physx::PxRigidDynamic* m_pDynamic = nullptr;

	// �� ������Ʈ�� ������ �ִ� Parent�� ���� ����
	std::weak_ptr<class GameEngineActor> ParentActor;
	physx::PxVec3 GeoMetryScale;

	//�ӵ����� �Լ�
	void SpeedLimit();

	float SpeedLimitValue = 150.0f;

	float StandUpTargetYAxisAngle = 0.0f;
	float StandUpStartYAxisAngle = 0.0f;
	float StandUpProgressYAxisAngle = 0.0f;

	physx::PxVec3 InitVec3;
	physx::PxVec3 TargetVec3;
	physx::PxTransform RecentTransform;

};

