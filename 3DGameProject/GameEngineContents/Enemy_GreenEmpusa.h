#pragma once

enum FSM_State_GreenEmpusa
{
	GreenEmpusa_Idle,
};

// ���� :
class Enemy_GreenEmpusa : public BaseEnemyActor
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
	void Start() override;
	void EnemyMeshLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyTypeLoad() override;
	void EnemyCreateFSM() override;
	void EnemyCreateFSM_Client() override;
	void DamageCollisionCheck(float _DeltaTime) override;
	void DamageCollisionCheck_Client(float _DeltaTime) override;
	void RecognizeCollisionCheck(float _DeltaTime) override;
	void DeathCheck() override;

private:
	void PlayerChase(float _DeltaTime);
	void PlayerAttack(float _DeltaTime);
	void RandomAttack();
	void ChangeState(int _StateValue);
	void ChangeState_Client(int _StateValue);
	void AttackCalculation();

	float WaitTime = 0.0f;
	float RunTime = 0.0f;
	float RotationDelayTime = 0.0f;
	float FallCheckDelayTime = 0.0f;
	float AttackDelayCheck = 0.0f;
	float AttackDelayTime = 0.0f;

	short WalkCount = 0;

	bool IsHeavyAttack = false;   // ������ ��Ʈ
	bool IsAirAttack = false;     // ������� ��Ʈ
	bool IsSlamAttack = false;    // �������� ��Ʈ
	bool IsBusterAttack = false;  // ������ ��Ʈ
	bool IsVergilLight = false;
	bool IsCollapse = false;      // �������ִ� ����
	bool IsRecognize = false;
	bool AttackTypeChange = false;

};

