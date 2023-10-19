#pragma once

enum FSM_State_HellAntenora
{
	HellAntenora_Idle,

	HellAntenora_Counter_Attack, //em0001_Attack_counter_attack
};

// 설명 :
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

	bool IsHeavyAttack = false;   // 강공격 히트
	bool IsAirAttack = false;     // 에어공격 히트
	bool IsSlamAttack = false;    // 슬램공격 히트
	bool IsBusterAttack = false;  // 버스터 히트
	bool IsVergilLight = false;
	bool IsCollapse = false;      // 쓰러져있는 상태
	bool IsRecognize = false;
	bool AttackTypeChange = false;
};

