#pragma once

#include "GameEngineComponent.h"
#include "PhysXDefault.h"

// 설명 :
class PhysXConvexComponent : public GameEngineComponent, public PhysXDefault
{
public:
	// constrcuter destructer
	PhysXConvexComponent();
	~PhysXConvexComponent();

	// delete Function
	PhysXConvexComponent(const PhysXConvexComponent& _Other) = delete;
	PhysXConvexComponent(PhysXConvexComponent&& _Other) noexcept = delete;
	PhysXConvexComponent& operator=(const PhysXConvexComponent& _Other) = delete;
	PhysXConvexComponent& operator=(PhysXConvexComponent&& _Other) noexcept = delete;

	void CreatePhysXActors(const std::string& _MeshName, physx::PxScene* _Scene, physx::PxPhysics* _physics,
		physx::PxCooking* _cooking, bool _InverseIndex = true, physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRot = { 0.0f, 0.0f }, bool _Gravity = false);

	// 힘을 추가
	void AddForce(float4 _Force);

	void AddTorque(float4 _Torque);

	// 가속도를 추가
	void AddAngularVelocity(float4 _Velocity);

	// RigidBody->Actor || Actor->RigidBody 플래그
	inline void SetPositionSetFromParentFlag(bool _Flag)
	{
		PositionSetFromParentFlag = _Flag;
	}

	// RigidBody의 질량을 취득
	inline physx::PxReal GetMass()
	{
		return m_pDynamic->getMass();
	}

	inline void ReleaseRigidBody()
	{
		m_pDynamic->release();
		this->Death();
	}

	inline void SetUpdateForce(physx::PxVec3 _UpdateForce)
	{
		AddUpdateForce = _UpdateForce;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void CustomFBXLoad(const std::string& _MeshName);

private:
	// Phys액터 생성에 필요한 정보
	physx::PxPhysics* m_pPhysics;
	physx::PxScene* m_pScene;

	physx::PxMaterial* m_pMaterial;
	physx::PxShape* m_pShape;
	physx::PxRigidDynamic* m_pDynamic;

	physx::PxConvexMesh* m_pConvexMesh;
	std::vector<physx::PxVec3> VertexVec;
	std::shared_ptr<class GameEngineFBXMesh> Mesh;

	physx::PxVec3 SeesawPos;

	// 이 컴포넌트를 가지고 있는 Parent에 대한 정보
	std::weak_ptr<class GameEngineActor> ParentActor;

	physx::PxVec3 AddUpdateForce;

	bool PositionSetFromParentFlag;

};

