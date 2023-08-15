#pragma once

#include "GameEngineLevel.h"

// 설명 :
class PhysicsLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	PhysicsLevel();
	~PhysicsLevel();

	// delete Function
	PhysicsLevel(const PhysicsLevel& _Other) = delete;
	PhysicsLevel(PhysicsLevel&& _Other) noexcept = delete;
	PhysicsLevel& operator=(const PhysicsLevel& _Other) = delete;
	PhysicsLevel& operator=(PhysicsLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	physx::PxDefaultAllocator		m_Allocator;
	physx::PxDefaultErrorCallback	m_ErrorCallback;

	physx::PxFoundation* m_pFoundation = nullptr;

	physx::PxPhysics* m_pPhysics = nullptr;

	physx::PxDefaultCpuDispatcher* m_pDispatcher = nullptr;
	physx::PxScene* m_pScene = nullptr;
	physx::PxMaterial* m_pMaterial = nullptr;

	physx::PxPvd* m_pPvd = nullptr;

	// 클래스 초기화
	void Initialize();
	void Simulate(bool _Value = true);
	void Release();

};

