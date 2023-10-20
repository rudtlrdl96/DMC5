#pragma once
#include "Poolable.h"
#include "BaseEnemyActor.h"

enum FSM_State_Empusa
{
	// ����
	Empusa_Appear_A,      // em0100_enter_ground_A, ���� A
	Empusa_Appear_B,      // em0100_enter_ground_B, ���� B

	// �̾�Ĺ �ڼ�
	Empusa_Biped_Idle, // em0100_biped_idle, �̾�Ĺó�� ������

	// ���� �ڼ�
	Empusa_Quadruped_Idle, // em0100_quadruped_idle : �����ڼ�
	Empusa_Quadruped_To_Biped, // em0100_quadruped_to_biped : �����ڼ�->�̾�Ĺ�ڼ� ����

	// ������
	Empusa_Biped_Run_Start, // em0100_biped_run_start : �̾�Ĺ�ڼ�->�����ڼ� ����Ǹ鼭 ������ ����
	Empusa_Biped_Run_Loop, // em0100_biped_run_loop : �̾�Ĺ�ڼ�->�����ڼ� ����� �� ������ ����
	Empusa_Biped_Run_Stop, // em0100_biped_run_stop : �����ڼ�->�̾�Ĺ�ڼ� ����Ǹ鼭 ������ ����
	Empusa_Biped_Walk_Start, // em0100_biped_walk_start : �̾�Ĺ �ڼ� ������ ����
	Empusa_Biped_Walk_Loop, // em0100_biped_walk_loop : �̾�Ĺ �ڼ� ������ ����
	Empusa_Biped_Walk_Stop, // em0100_biped_walk_stop : �̾�Ĺ �ڼ� ������ ��
	Empusa_Step_Back, // em0100_step_back : �����ڼ� �ڷ� �ѹ� ����

	// ȸ��
	Empusa_Turn_Left_90, // em0100_turn_90_left : �̾�Ĺ �ڼ� ���� 90�� ��
	Empusa_Turn_Right_90, // em0100_turn_90_right : �̾�Ĺ �ڼ� ������ 90�� ��
	Empusa_Turn_Left_180, // em0100_turn_180_left : �̾�Ĺ �ڼ� ���� 180�� ��
	Empusa_Turn_Right_180, // em0100_turn_180_right : �̾�Ĺ �ڼ� ������ 180�� ��

	// ����
	Empusa_Attack_A, // em0100_attack_A : ������ ������ �Ʒ��� ����, 136 on, 142 off
	Empusa_Attack_B, // em0100_attack_B : �޼� ������ �Ʒ��� ����, 68 on, 73 off
	Empusa_Attack_C, // em0100_attack_C : ������ Ⱦ����, 60 on, 64 off
	Empusa_Attack_D, // em0100_attack_D : �޼� Ⱦ����, 60 on, 65 off
	Empusa_Attack_W, // em0100_attack_W : ��� �������, 151 on, 155 off
	Empusa_Continuation_Attack, // em0100_continuation_attack : ���Ӱ���(������ �����ϸ鼭 ����), 128/131, 161/163, 195/197, 230/233, 296/301

	// �ǰݷ�
	Empusa_Standing_Damage_Weak_Front, // em0100_angledamage_front, ���� ����� ��Ʈ
	Empusa_Standing_Damage_Weak_Back,  // em0100_angledamage_back, �ĸ� ����� ��Ʈ
	Empusa_Standing_Damage_Weak_Left,  // em0100_angledamage_left, ���� ����� ��Ʈ
	Empusa_Standing_Damage_Weak_Right, // em0100_angledamage_right, ���� ����� ��Ʈ

	Empusa_Air_Damage,				  // em0100_air_damage, ���� �ǰ� start
	Empusa_Air_Damage_Under,		  // em0100_air_damage_under, ���� �ǰ� �� ��� ���� �� ���

	Empusa_Blown_Back,				  // em0100_blown_back_start, ������ �ǰ� �� ���ư��� start
	Empusa_Blown_Back_Landing,		  // em0100_blown_back_landing, ������ �ǰ� �� ���ư��� ���� �΋H��

	Empusa_Slam_Damage,				  // em0100_slam_damage_start, ���� �ǰ� �� start
	Empusa_Slam_Damage_Loop,		  // em0100_slam_damage_fall_loop, ���� �ǰ� ���� loop
	Empusa_Slam_Damage_Landing,		  // em0100_slam_damage_landing, ���� �ǰ� �� ���� �΋H��

	Empusa_Snatch,					  // em0100_snatch_loop, ����ġ start
	Empusa_Snatch_End,				  // em0000_snatch_end, ����ġ end

	Empusa_Stun,					  // em0100_stun_loop, ���� start
	Empusa_Stun_Revive,				  // em0100_stun_revive, ���� ���¿��� ȸ��

	Empusa_Death_Front,				  // em0100_death_front, �ڷ� �������鼭 ����
	Empusa_Death_Back,				  // em0100_death_back, ������ �������鼭 ����
	Empusa_Downward_Getup,			  // em0100_downward_down_standup, ������ �Ѿ��� ���¿��� �Ͼ
	Empusa_Downward_Death,			  // em0100_downward_die, ������ �����ִ� ���¿��� Death

	Empusa_Buster_Start,       // em0000_Buster_Start, ����Ʈ ��Ʈ ����
	Empusa_Buster_Loop,        // ����Ʈ ��Ʈ ����
	Empusa_Buster_Finish,      // em0000_Buster_Finish, ����Ʈ ��Ʈ ���� ������

};

class Enemy_Empusa : public BaseEnemyActor, public Poolable<Enemy_Empusa>
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
	void MoveLoop();

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
	bool IsAppearA = false;
};

