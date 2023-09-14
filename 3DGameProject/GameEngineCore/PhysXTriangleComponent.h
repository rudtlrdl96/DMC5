#pragma once

#include "GameEngineComponent.h"
#include "PhysicsActor.h"

// ���� :
class PhysXTriangleComponent : public GameEngineComponent, public PhysicsActor
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
	
	void CreatePhysXActors(const std::string& _MeshName, bool _InverseIndex = true, float _Ratio = 1.f, float4 _GeoMetryRot = { 0.0f, 0.0f });

	inline void SetPositionSetFromParentFlag(bool _Flag)
	{
		PositionSetFromParentFlag = _Flag;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void CustomFBXLoad(const std::string& _MeshName, float _Ratio, bool _InverseIndex);

private:
	// Phys���� ������ �ʿ��� ����
	physx::PxPhysics* m_pPhysics = nullptr;
	physx::PxScene* m_pScene = nullptr;
	physx::PxCooking* m_pCooking = nullptr;
	physx::PxMaterial* m_pMaterial = nullptr;
	physx::PxShape* m_pTriggershape = nullptr;

	physx::PxConvexMesh* m_pConvexMesh = nullptr;

	std::vector<std::vector<physx::PxVec3>> VertexVec;
	std::vector<std::vector<physx::PxU32>> IndexVec;
	std::shared_ptr<class GameEngineFBXMesh> Mesh = nullptr;

	bool PositionSetFromParentFlag;

};

