#pragma once
enum FSM_State_Empusa
{
	//Idle
	Empusa_Idle,

	// 움직임
	Empusa_Walk_Start,       // em0100_walk_start
	Empusa_Walk_Loop,        // em0100_walk_loop
	Empusa_Walk_Stop,        // em0100_walk_stop
	Empusa_Walk_Right_Start, // em0100_walk_right_start
	Empusa_Walk_Right_Loop,  // em0100_walk_right_loop
	Empusa_Walk_Right_Stop,  // em0100_walk_right_stop
	Empusa_Walk_Left_Start,  // em0100_walk_left_start
	Empusa_Walk_Left_Loop,   // em0100_walk_left_loop
	Empusa_Walk_Left_Stop,   // em0100_walk_left_stop

	// 회전
	Empusa_Turn_Left,  // em0100
	Empusa_Turn_Right, // em0100
	Empusa_Turn_Left_90,    // em0100_turn_left_90, 왼쪽 회전
	Empusa_Turn_Right_90,   // em0100_turn_right_90, 오른쪽 회전
	Empusa_Turn_Left_180,   // em0100_turn_left_180, 왼쪽 회전
	Empusa_Turn_Right_180,  // em0100_turn_right_180, 오른쪽 회전

	//피격
	Empusa_Standing_Damage_Weak_Front, // em0100_standing_damage_weak_front_01, 정면 약공격 히트
	Empusa_Standing_Damage_Weak_Back,  // em0100_standing_damage_weak_back_01, 후면 약공격 히트
	Empusa_Standing_Damage_Weak_Left,  // em0100_standing_damage_weak_left_01, 좌측 약공격 히트
	Empusa_Standing_Damage_Weak_Right, // em0100_standing_damage_weak_right_01, 우측 약공격 히트

	//////////////공격
	Empusa_attack_A,
	Empusa_attack_B,
	Empusa_attack_C,
	Empusa_attack_D,
	Empusa_attack_W,

};
class Enemy_Empusa : public BaseEnemyActor
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
	
	void PlayerChase(float _DeltaTime);
	void ChangeState(int _StateValue);
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;
	void EnemyCreateFSM_Client() override;
	void DamageCollisionCheck(float _DeltaTime) override;
	void RecognizeCollisionCheck(float _DeltaTime) override;

private:
	bool CheckBool = false;
	float AttackDelayCheck = 0.0f;

	short WalkCount = 0;

	bool IsHeavyAttack = false;
	bool IsAirAttack = false;
	bool IsAirLanding = false;
	bool IsSlamAttack = false;
	bool IsRecognize = false;
};

