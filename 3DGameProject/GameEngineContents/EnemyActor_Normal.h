#pragma once
#include "BaseEnemyActor.h"

class EnemyActor_Normal : public BaseEnemyActor
{
public:
	EnemyActor_Normal();
	~EnemyActor_Normal();

	EnemyActor_Normal(const EnemyActor_Normal& _Other) = delete;
	EnemyActor_Normal(EnemyActor_Normal&& _Other) noexcept = delete;
	EnemyActor_Normal& operator=(const EnemyActor_Normal& _Other) = delete;
	EnemyActor_Normal& operator=(EnemyActor_Normal&& _Other) noexcept = delete;

protected:
	void Start() override;
	
private:
	void Idle_Enter();
	void Idle_Update(float _DeltaTime);
	void Idle_Exit();

	void Chase_Enter();
	void Chase_Update(float _DeltaTime);
	void Chase_Exit();

	void Attack_Enter();
	void Attack_Update(float _DeltaTime);
	void Attack_Exit();

	void Hit_Enter();
	void Hit_Update(float _DeltaTime);
	void Hit_Exit();

	void Fall_Enter();
	void Fall_Update(float _DeltaTime);
	void Fall_Exit();

	void Snatch_Enter();
	void Snatch_Update(float _DeltaTime);
	void Snatch_Exit();

	void Buster_Enter();
	void Buster_Update(float _DeltaTime);
	void Buster_Exit();
};

