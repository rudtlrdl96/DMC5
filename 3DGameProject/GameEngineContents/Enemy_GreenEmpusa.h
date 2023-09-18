#pragma once
#include "EnemyActor_Fly.h"

// Ό³Έν :
class Enemy_GreenEmpusa : public EnemyActor_Fly
{
public:
	// constrcuter destructer
	Enemy_GreenEmpusa();
	~Enemy_GreenEmpusa();

	// delete Function
	Enemy_GreenEmpusa(const Enemy_GreenEmpusa& _Other) = delete;
	Enemy_GreenEmpusa(Enemy_GreenEmpusa&& _Other) noexcept = delete;
	Enemy_GreenEmpusa& operator=(const Enemy_GreenEmpusa& _Other) = delete;
	Enemy_GreenEmpusa& operator=(Enemy_GreenEmpusa&& _Other) noexcept = delete;



protected:

private:
	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;
	void EnemyCreateFSM_Client() override;
};

