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
	void Move(float _DeltaTime);

	void SetFSMStateValue(int _StateValue) override
	{
		EnemyFSM.ChangeState(_StateValue);
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;
	void DamageCollisionCheck() override;

private:
	bool CheckBool = false;

	float MoveSpeed = 50.0f;

	// ������
	void Idle_Enter();
	void Idle_Update(float _DeltaTime);
	void Idle_Exit();

	// �÷��̾� ����
	void Chase_Enter();
	void Chase_Update(float _DeltaTime);
	void Chase_Exit();

	// ����
	void Attack_Enter();
	void Attack_Update(float _DeltaTime);
	void Attack_Exit();

	// �߶�
	void Fall_Enter();
	void Fall_Update(float _DeltaTime);
	void Fall_Exit();

	// �׷� ����
	void Snatch_Enter();
	void Snatch_Update(float _DeltaTime);
	void Snatch_Exit();

	// ������
	void Buster_Enter();
	void Buster_Update(float _DeltaTime);
	void Buster_Exit();

	// �ǰ�
	void Hit_Enter();
	void Hit_Update(float _DeltaTime);
	void Hit_Exit();

	// ���
	void Death_Enter();
	void Death_Update(float _DeltaTime);
	void Death_Exit();

	std::vector<string> Vec_AttackName;
	int AttackValue = 0;
};

