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

	physx::PxRigidDynamic* CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRotation = { 0.0f , 0.0f });

	physx::PxRigidDynamic* GetDynamic()
	{
		return m_pDynamic;
	}

	void SetMoveSpeed(float4 _MoveSpeed);

	void SetMoveJump();
	void SetMoveDive(float _Rot);

	// RigidDynamic�� CCT���� �����ϴ� �Լ�
	void SetDynamicIdle();

	inline physx::PxVec3 GetLinearVelocity()
	{
		return m_pDynamic->getLinearVelocity();
	}

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

	inline float4 GetWorldPosition()
	{
		return float4(m_pDynamic->getGlobalPose().p.x, m_pDynamic->getGlobalPose().p.y, m_pDynamic->getGlobalPose().p.z);
	}

	void SetPlayerStartPos(float4 _Pos);

	void PushImpulse(float4 _ImpulsePower);
	void PushImpulseAtLocalPos(float4 _ImpulsePower, float4 _Pos);

	bool PlayerStandUp(float _DeltaTime, bool _IsXAixisRotReady);
	bool StandUp2(float _DeltaTime, bool _IsXAixisRotReady);

	void TurnOffSpeedLimit()
	{
		IsSpeedLimit = false;
	}

	void TurnOnSpeedLimit()
	{
		IsSpeedLimit = true;
	}

	void SwitchSpeedLimit()
	{
		IsSpeedLimit = !IsSpeedLimit;
	}

	float4 GetDynamicVelocity()
	{
		physx::PxVec3 Vec3 = m_pDynamic->getLinearVelocity();
		return float4{ Vec3.x, Vec3.y, Vec3.z };
	}

	//ȸ�� ������ �Լ�
	void SetChangedRot(float4 _Rot);

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

	void LockAxis();

	inline void SetIsMain(bool _Flag)
	{
		IsMain = _Flag;
	}

	//�÷��̾� ���ߴ� �Լ�
	void FreezeDynamic();

	//�÷��̾� ���ߴ°� Ǫ�� �Լ�
	void WakeUpDynamic();

	//Reset �Լ�
	void ResetDynamic();

	//�Ͼ�� ��ǥ �������ϴ� �Լ�
	void InitializeStandUp2();

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
	// Phys���� ������ �ʿ��� ����
	physx::PxScene* m_pScene = nullptr;
	physx::PxPhysics* m_pPhysics = nullptr;
	physx::PxControllerManager* m_pCtrManager = nullptr;

	physx::PxMaterial* m_pMaterial = nullptr;
	physx::PxShape* m_pShape = nullptr;
	physx::PxRigidDynamic* m_pDynamic = nullptr;

	bool IsSpeedLimit = false;

	// �� ������Ʈ�� ������ �ִ� Parent�� ���� ����
	std::weak_ptr<GameEngineActor> ParentActor;
	physx::PxVec3 GeoMetryScale;

	//�ӵ����� �Լ�
	void SpeedLimit();

	float StandUpTargetYAxisAngle = 0.0f;
	float StandUpStartYAxisAngle = 0.0f;
	float StandUpProgressYAxisAngle = 0.0f;

	physx::PxVec3 InitVec3;
	physx::PxVec3 TargetVec3;
	physx::PxTransform RecentTransform;

	// �����÷��̾� �÷���
	bool IsMain = false;
};

