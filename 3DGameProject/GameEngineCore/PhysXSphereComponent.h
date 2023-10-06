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
	}

	void Off() override
	{
		GameEngineObjectBase::Off();
		m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		m_pDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	}

	void PushImpulse(float4 _ImpulsePower);
	void PushImpulseAtLocalPos(float4 _ImpulsePower, float4 _Pos);

	//Reset 함수
	void ResetDynamic();

	// RigidDynamic을 CCT에서 해제하는 함수
	void SetDynamicIdle();

	//회전 움직임 함수
	void SetChangedRot(float4 _Rot);

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

	//속도제한 함수
	void SpeedLimit();

	physx::PxVec3 GeoMetryScale;
	physx::PxVec3 InitVec3;
	physx::PxTransform RecentTransform;

	// 메인플레이어 플래그
	bool IsMain = false;
	bool m_bSpeedLimit = false;
};

