#pragma once

// ���� :
class Enemy_HellAntenora : public BaseEnemyActor
{
public:
	// constrcuter destructer
	Enemy_HellAntenora();
	~Enemy_HellAntenora();

	// delete Function
	Enemy_HellAntenora(const Enemy_HellAntenora& _Other) = delete;
	Enemy_HellAntenora(Enemy_HellAntenora&& _Other) noexcept = delete;
	Enemy_HellAntenora& operator=(const Enemy_HellAntenora& _Other) = delete;
	Enemy_HellAntenora& operator=(Enemy_HellAntenora&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;
	void EnemyCreateFSM_Client() override;

private:
	bool CheckBool = false;

	//// ������
	//virtual void Idle_Enter();
	//virtual void Idle_Update(float _DeltaTime);
	//virtual void Idle_Exit();

	//// �÷��̾� ����
	//virtual void Chase_Enter();
	//virtual void Chase_Update(float _DeltaTime);
	//virtual void Chase_Exit();

	//// ����
	//virtual void Attack_Enter();
	//virtual void Attack_Update(float _DeltaTime);
	//virtual void Attack_Exit();

	//// �߶�
	//virtual void Fall_Enter();
	//virtual void Fall_Update(float _DeltaTime);
	//virtual void Fall_Exit();

	//// �׷� ����
	//virtual void Snatch_Enter();
	//virtual void Snatch_Update(float _DeltaTime);
	//virtual void Snatch_Exit();

	//// ������
	//virtual void Buster_Enter();
	//virtual void Buster_Update(float _DeltaTime);
	//virtual void Buster_Exit();

	//// �ǰ�
	//virtual void Hit_Enter();
	//virtual void Hit_Update(float _DeltaTime);
	//virtual void Hit_Exit();

	//// ���
	//virtual void Death_Enter();
	//virtual void Death_Update(float _DeltaTime);
	//virtual void Death_Exit();
};

