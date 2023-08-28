#pragma once
#include "EnemyActor_Normal.h"

// Ό³Έν :
class Enemy_RedEmpusa : public EnemyActor_Normal
{
public:
	// constrcuter destructer
	Enemy_RedEmpusa();
	~Enemy_RedEmpusa();

	// delete Function
	Enemy_RedEmpusa(const Enemy_RedEmpusa& _Other) = delete;
	Enemy_RedEmpusa(Enemy_RedEmpusa&& _Other) noexcept = delete;
	Enemy_RedEmpusa& operator=(const Enemy_RedEmpusa& _Other) = delete;
	Enemy_RedEmpusa& operator=(Enemy_RedEmpusa&& _Other) noexcept = delete;

protected:

private:
	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;
};

