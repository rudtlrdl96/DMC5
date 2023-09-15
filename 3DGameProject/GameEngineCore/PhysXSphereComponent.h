#pragma once
#include <GameEngineCore/GameEngineComponent.h>
#include "PhysicsActor.h"

// ���� :
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

	void PushImpulse(float4 _ImpulsePower);
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

	//Reset �Լ�
	void ResetDynamic();

	// RigidDynamic�� CCT���� �����ϴ� �Լ�
	void SetDynamicIdle();

	//ȸ�� ������ �Լ�
	void SetChangedRot(float4 _Rot);

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

	//�ӵ����� �Լ�
	void SpeedLimit();

	physx::PxVec3 GeoMetryScale;
	physx::PxVec3 InitVec3;
	physx::PxTransform RecentTransform;

	// �����÷��̾� �÷���
	bool IsMain = false;
	bool m_bSpeedLimit = false;
};

