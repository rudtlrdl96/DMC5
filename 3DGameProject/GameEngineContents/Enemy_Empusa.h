#pragma once
#include "EnemyActor_Normal.h"

using namespace std;

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
	void RN_Idle();
	void Move();
protected:
	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;
private:
	bool CheckBool = false;

	float MoveSpeed = 50.0f;

	// 대기상태
	void Idle_Enter();
	void Idle_Update(float _DeltaTime);
	void Idle_Exit();

	// 플레이어 추적
	void Chase_Enter();
	void Chase_Update(float _DeltaTime);
	void Chase_Exit();

	// 공격
	void Attack_Enter();
	void Attack_Update(float _DeltaTime);
	void Attack_Exit();

	// 추락
	void Fall_Enter();
	void Fall_Update(float _DeltaTime);
	void Fall_Exit();

	// 그랩 상태
	void Snatch_Enter();
	void Snatch_Update(float _DeltaTime);
	void Snatch_Exit();

	// 버스터
	void Buster_Enter();
	void Buster_Update(float _DeltaTime);
	void Buster_Exit();

	// 피격
	void Hit_Enter();
	void Hit_Update(float _DeltaTime);
	void Hit_Exit();

	// 사망
	void Death_Enter();
	void Death_Update(float _DeltaTime);
	void Death_Exit();
};

