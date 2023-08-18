#pragma once
#include "EnemyActor_Normal.h"

// Ό³Έν :
class Enemy_Empusa : public EnemyActor_Normal
{
public:
	// constrcuter destructer
	Enemy_Empusa();
	~Enemy_Empusa();

	// delete Function
	Enemy_Empusa(const Enemy_Empusa& _Other) = delete;
	Enemy_Empusa(Enemy_Empusa&& _Other) noexcept = delete;
	Enemy_Empusa& operator=(const Enemy_Empusa& _Other) = delete;
	Enemy_Empusa& operator=(Enemy_Empusa&& _Other) noexcept = delete;

protected:

private:
	void EnemyMeshLoad() override;
	void EnemyTextureLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;
};

