#pragma once

#include "GameEngineComponent.h"
#include "PhysXDefault.h"

// ���� :
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

	void CreatePhysXActors(const std::string& _MeshName, physx::PxCooking* _cooking, bool _InverseIndex = true, physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRot = { 0.0f, 0.0f }, bool _Gravity = false);

	// ���� �߰�
	void AddForce(float4 _Force);

	void AddTorque(float4 _Torque);

	// ���ӵ��� �߰�
	void AddAngularVelocity(float4 _Velocity);

	// RigidBody->Actor || Actor->RigidBody �÷���
	inline void SetPositionSetFromParentFlag(bool _Flag)
	{
		PositionSetFromParentFlag = _Flag;
	}

	// RigidBody�� ������ ���
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
	// Phys���� ������ �ʿ��� ����
	physx::PxPhysics* m_pPhysics = nullptr;
	physx::PxScene* m_pScene = nullptr;
	physx::PxCooking* m_pCooking = nullptr;

	physx::PxMaterial* m_pMaterial = nullptr;
	physx::PxShape* m_pShape = nullptr;
	physx::PxRigidDynamic* m_pDynamic = nullptr;

	physx::PxConvexMesh* m_pConvexMesh = nullptr;

	std::shared_ptr<class GameEngineFBXMesh> Mesh = nullptr;

	std::vector<physx::PxVec3> VertexVec;
	physx::PxVec3 SeesawPos;
	physx::PxVec3 AddUpdateForce;

	// �� ������Ʈ�� ������ �ִ� Parent�� ���� ����
	std::weak_ptr<class GameEngineActor> ParentActor;

	bool PositionSetFromParentFlag;

};

