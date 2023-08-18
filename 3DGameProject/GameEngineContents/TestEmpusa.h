#pragma once
#include "EnemyActor_Normal.h"

// Ό³Έν :
class TestEmpusa : public EnemyActor_Normal
{
public:
	// constrcuter destructer
	TestEmpusa();
	~TestEmpusa();

	// delete Function
	TestEmpusa(const TestEmpusa& _Other) = delete;
	TestEmpusa(TestEmpusa&& _Other) noexcept = delete;
	TestEmpusa& operator=(const TestEmpusa& _Other) = delete;
	TestEmpusa& operator=(TestEmpusa&& _Other) noexcept = delete;

protected:

private:
	void EnemyMeshLoad() override;
	void EnemyTextureLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;
	
	std::shared_ptr<class PhysXDynamicActorComponent> Component = nullptr;
};

