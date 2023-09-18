#pragma once

// 설명 :
class Enemy_RedEmpusa : public BaseEnemyActor
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
	void Start() override;
	void Update(float _DeltaTime) override;

	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;

private:
	bool CheckBool = false;

	// 대기상태
	virtual void Idle_Enter();
	virtual void Idle_Update(float _DeltaTime);
	virtual void Idle_Exit();

	// 플레이어 추적
	virtual void Chase_Enter();
	virtual void Chase_Update(float _DeltaTime);
	virtual void Chase_Exit();

	// 공격
	virtual void Attack_Enter();
	virtual void Attack_Update(float _DeltaTime);
	virtual void Attack_Exit();

	// 추락
	virtual void Fall_Enter();
	virtual void Fall_Update(float _DeltaTime);
	virtual void Fall_Exit();

	// 그랩 상태
	virtual void Snatch_Enter();
	virtual void Snatch_Update(float _DeltaTime);
	virtual void Snatch_Exit();

	// 버스터
	virtual void Buster_Enter();
	virtual void Buster_Update(float _DeltaTime);
	virtual void Buster_Exit();

	// 피격
	virtual void Hit_Enter();
	virtual void Hit_Update(float _DeltaTime);
	virtual void Hit_Exit();

	// 사망
	virtual void Death_Enter();
	virtual void Death_Update(float _DeltaTime);
	virtual void Death_Exit();
};

