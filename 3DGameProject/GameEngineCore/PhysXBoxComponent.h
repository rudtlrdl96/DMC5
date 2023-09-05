#pragma once

#include "GameEngineComponent.h"
#include "PhysXDefault.h"

// 설명 :
class PhysXBoxComponent : public GameEngineComponent, public PhysXDefault
{
public:
	// constrcuter destructer
	PhysXBoxComponent();
	~PhysXBoxComponent();

	// delete Function
	PhysXBoxComponent(const PhysXBoxComponent& _Other) = delete;
	PhysXBoxComponent(PhysXBoxComponent&& _Other) noexcept = delete;
	PhysXBoxComponent& operator=(const PhysXBoxComponent& _Other) = delete;
	PhysXBoxComponent& operator=(PhysXBoxComponent&& _Other) noexcept = delete;

	void CreatePhysXActors(physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRot = float4::ZERO);

	inline void ReleasePhysX()
	{
		if (true == m_pDynamic->isReleasable())
		{
			m_pDynamic->release();
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

private:
	// Phys액터 생성에 필요한 정보
	physx::PxPhysics* m_pPhysics = nullptr;
	physx::PxScene* m_pScene = nullptr;

	physx::PxMaterial* m_pMaterial = nullptr;
	physx::PxShape* m_pShape = nullptr;

	std::weak_ptr<GameEngineActor> ParentActor;

	bool PositionSetFromParentFlag = true;

};

