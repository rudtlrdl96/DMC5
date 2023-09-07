#pragma once
#include "BaseEnemyActor.h"
enum EnemyState
{
	M_Idle,
	M_Chase,
	M_Attack,
	M_Fall,
	M_Snatch,
	M_Buster,
	M_Hit,
	M_Death,
};

class EnemyActor_Normal : public BaseEnemyActor
{
public:
	EnemyActor_Normal();
	~EnemyActor_Normal();

	EnemyActor_Normal(const EnemyActor_Normal& _Other) = delete;
	EnemyActor_Normal(EnemyActor_Normal&& _Other) noexcept = delete;
	EnemyActor_Normal& operator=(const EnemyActor_Normal& _Other) = delete;
	EnemyActor_Normal& operator=(EnemyActor_Normal&& _Other) noexcept = delete;

	inline bool IsSuperArmor() const
	{
		return IsSuperArmorValue;
	}

	void RNPlayer();
protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	// ������
	virtual void Idle_Enter();
	virtual void Idle_Update(float _DeltaTime);
	virtual void Idle_Exit();

	// �÷��̾� ����
	virtual void Chase_Enter();
	virtual void Chase_Update(float _DeltaTime);
	virtual void Chase_Exit();

	// ����
	virtual void Attack_Enter();
	virtual void Attack_Update(float _DeltaTime);
	virtual void Attack_Exit();

	// �߶�
	virtual void Fall_Enter();
	virtual void Fall_Update(float _DeltaTime);
	virtual void Fall_Exit();

	// �׷� ����
	virtual void Snatch_Enter();
	virtual void Snatch_Update(float _DeltaTime);
	virtual void Snatch_Exit();

	// ������
	virtual void Buster_Enter();
	virtual void Buster_Update(float _DeltaTime);
	virtual void Buster_Exit();

	// �ǰ�
	virtual void Hit_Enter();
	virtual void Hit_Update(float _DeltaTime);
	virtual void Hit_Exit();

	// ���
	virtual void Death_Enter();
	virtual void Death_Update(float _DeltaTime);
	virtual void Death_Exit();
};