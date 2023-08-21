#pragma once

#include "GameEngineComponent.h"
#include "PhysXDefault.h"

// 설명 :
class PhysXTriangleComponent : public GameEngineComponent, public PhysXDefault
{
public:
	// constrcuter destructer
	PhysXTriangleComponent();
	~PhysXTriangleComponent();

	// delete Function
	PhysXTriangleComponent(const PhysXTriangleComponent& _Other) = delete;
	PhysXTriangleComponent(PhysXTriangleComponent&& _Other) noexcept = delete;
	PhysXTriangleComponent& operator=(const PhysXTriangleComponent& _Other) = delete;
	PhysXTriangleComponent& operator=(PhysXTriangleComponent&& _Other) noexcept = delete;
	
	void CreatePhysXActors(const std::string& _MeshName, physx::PxScene* _Scene, physx::PxPhysics* _physics,
		physx::PxCooking* _cooking, bool _InverseIndex = true, physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRot = { 0.0f, 0.0f });

	inline void ReleasePhysX()
	{
		if (true == m_pRigidStatic->isReleasable())
		{
			m_pRigidStatic->release();
			ParentActor.lock()->Death();
		}
	}

	inline void SetPositionSetFromParentFlag(bool _Flag)
	{
		PositionSetFromParentFlag = _Flag;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void CustomFBXLoad(const std::string& _MeshName, bool _InverseIndex);

private:
	// Phys액터 생성에 필요한 정보
	physx::PxPhysics* m_pPhysics;
	physx::PxScene* m_pScene;

	physx::PxMaterial* m_pMaterial;
	physx::PxShape* m_pShape;
	physx::PxShape* m_pTriggershape;
	physx::PxRigidStatic* m_pRigidStatic;

	physx::PxConvexMesh* m_pConvexMesh;

	std::vector<std::vector<physx::PxVec3>> VertexVec;
	std::vector<std::vector<physx::PxU32>> IndexVec;
	std::shared_ptr<class GameEngineFBXMesh> Mesh;

	// 이 컴포넌트를 가지고 있는 Parent에 대한 정보
	std::weak_ptr<GameEngineActor> ParentActor;

	bool PositionSetFromParentFlag;

};

