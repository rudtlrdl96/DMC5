#pragma once

enum FSM_State_Empusa
{
	// ����
	Empusa_Appear_A,      // em0100_enter_ground_A, ���� A
	Empusa_Appear_B,      // em0100_enter_ground_B, ���� B

	// �⺻ (�ڼ��� 3���� // biped == �̾�Ĺ �ڼ� // orb == ���� ��� �ڼ� (�����ڼ�, ���� �Ӹ��ڰ� ����) // quadruped == ���� �ڼ�)
	// �̾�Ĺ �ڼ�
	Empusa_Biped_Idle, // em0100_biped_idle, �̾�Ĺó�� ������
	Empusa_Biped_Detected, // em0100_biped_detected, �̾�Ĺó�� ���ִٰ� ���߰�
	Empusa_Biped_Menace, // em0100_biped_menace, �̾�Ĺó�� ���ִٰ� ����
	Empusa_Biped_Search, // em0100_biped_search, �̾�Ĺó�� �������鼭 �� �����鼭 ��ġ
	Empusa_Biped_To_Quadruped, // em0100_biped_to_quadruped, �̾�Ĺ->�����ڼ� ����
	Empusa_Idle_Undetected, // em0100_Idle_undetected, �̾�Ĺó�� ������
	Empusa_Idle_Variation_A, // em0100_Idle_variation_A, �̾�Ĺ �޽��ڼ� A 
	Empusa_Idle_Variation_B, // em0100_Idle_variation_B, �̾�Ĺ �޽��ڼ� B

	// ���� �ڼ�
	Empusa_Orb_Absorb_Stay, // em0100_orb_absorb_stay, ���� ��� �ڼ�
	Empusa_Orb_Absorb_Walk_Start, // em0100_orb_absorb_walk_start, �����ڼ� ������ ����
	Empusa_Orb_Absorb_Walk_Loop, // em0100_orb_absorb_walk_loop, �����ڼ� ������ ����
	Empusa_Orb_Absorb_Walk_Stop, // em0100_orb_absorb_walk_stop, �����ڼ� ������ ����
	Empusa_Setting_D, // em0100_setting_D, �����ڼ� �޽�
	Empusa_Sonicwave_Start, // em0100_sonicwave_start, �̾�Ĺ�ڼ�->�����ڼ�
	Empusa_Sonicwave_Loop, // em0100_sonicwave_loop, �����ڼ� ����
	Empusa_Sonicwave_End, // em0100_sonicwave_end, �����ڼ�->�̾�Ĺ �ڼ�

	// ���� �ڼ�
	Empusa_Quadruped_Idle, // em0100_quadruped_idle : �����ڼ�
	Empusa_Quadruped_Detected, // em0100_quadruped_detected : �����ڼ� ���߰�
	Empusa_Quadruped_Menace, // em0100_quadruped_menace : �����ڼ� ����
	Empusa_Quadruped_Search, // em0100_quadruped_search : �����ڼ� �� �����鼭 ��ġ
	Empusa_Quadruped_To_Biped, // em0100_quadruped_to_biped : �����ڼ�->�̾�Ĺ�ڼ� ����
	Empusa_Idle_Variation_C, // em0100_Idle_variation_C : �޽��ڼ� C / �����ڼ�
	Empusa_Idle_Variation_D, // em0100_Idle_variation_D : �޽��ڼ� D / �����ڼ�
	Empusa_Setting_C, // em0100_setting_C : �����ڼ� �޽�

	// ������
	Empusa_Biped_Move_Back_Start, // em0100_biped_move_back_start : �̾�Ĺ �ڼ� �ڷ� ������ ����
	Empusa_Biped_Move_Back_Loop, // em0100_biped_move_back_loop : �̾�Ĺ �ڼ� �ڷ� ������ ����
	Empusa_Biped_Move_Back_Stop, // em0100_biped_move_back_stop : �̾�Ĺ �ڼ� �ڷ� ������ ��
	Empusa_Biped_Run_Start, // em0100_biped_run_start : �̾�Ĺ�ڼ�->�����ڼ� ����Ǹ鼭 ������ ����
	Empusa_Biped_Run_Loop, // em0100_biped_run_loop : �̾�Ĺ�ڼ�->�����ڼ� ����� �� ������ ����
	Empusa_Biped_Run_Stop, // em0100_biped_run_stop : �����ڼ�->�̾�Ĺ�ڼ� ����Ǹ鼭 ������ ����
	Empusa_Biped_Walk_Start, // em0100_biped_walk_start : �̾�Ĺ �ڼ� ������ ����
	Empusa_Biped_Walk_Loop, // em0100_biped_walk_loop : �̾�Ĺ �ڼ� ������ ����
	Empusa_Biped_Walk_Stop, // em0100_biped_walk_stop : �̾�Ĺ �ڼ� ������ ��
	Empusa_Step_Front, // em0100_step_front : �����ڼ� ������ �ѹ� ����
	Empusa_Step_Back, // em0100_step_back : �����ڼ� �ڷ� �ѹ� ����
	Empusa_Step_Left, // em0100_step_left : �����ڼ� �������� �ѹ� ����
	Empusa_Step_Right, // em0100_step_right : �����ڼ� ���������� �ѹ� ����

	Empusa_Walk_Left_Start, // em0100_walk_left_start : �̾�Ĺ �ڼ� ���� ������ ����
	Empusa_Walk_Left_Loop, // em0100_walk_left_loop : �̾�Ĺ �ڼ� ���� ������ ����
	Empusa_Walk_Left_Stop, // em0100_walk_left_stop : �̾�Ĺ �ڼ� ���� ������ ��
	Empusa_Walk_Right_Start, // em0100_walk_right_start : �̾�Ĺ �ڼ� ������ ������ ����
	Empusa_Walk_Right_Loop, // em0100_walk_right_loop : �̾�Ĺ �ڼ� ������ ������ ����
	Empusa_Walk_Right_Stop, // em0100_walk_right_stop : �̾�Ĺ �ڼ� ������ ������ ��

	// ȸ��
	Empusa_Turn_Left_90, // em0100_turn_90_left : �̾�Ĺ �ڼ� ���� 90�� ��
	Empusa_Turn_Right_90, // em0100_turn_90_right : �̾�Ĺ �ڼ� ������ 90�� ��
	Empusa_Turn_Left_180, // em0100_turn_180_left : �̾�Ĺ �ڼ� ���� 180�� ��
	Empusa_Turn_Right_180, // em0100_turn_180_right : �̾�Ĺ �ڼ� ������ 180�� ��

	// ����
	Empusa_Attack_A, // em0100_attack_A : ������ ������ �Ʒ��� ����
	Empusa_Attack_B, // em0100_attack_B : �޼� ������ �Ʒ��� ����
	Empusa_Attack_C, // em0100_attack_C : ������ Ⱦ����
	Empusa_Attack_D, // em0100_attack_D : �޼� Ⱦ����
	Empusa_Attack_W, // em0100_attack_W : ��� �������
	Empusa_Continuation_Attack, // em0100_continuation_attack : ���Ӱ���(������ �����ϸ鼭 ����)

	// �ǰݷ�
	Empusa_Standing_Damage_Weak_Front, // em0100_angledamage_front, ���� ����� ��Ʈ
	Empusa_Standing_Damage_Weak_Back,  // em0100_angledamage_back, �ĸ� ����� ��Ʈ
	Empusa_Standing_Damage_Weak_Left,  // em0100_angledamage_left, ���� ����� ��Ʈ
	Empusa_Standing_Damage_Weak_Right, // em0100_angledamage_right, ���� ����� ��Ʈ

	Empusa_Air_Damage,				  // em0100_air_damage, ���� �ǰ� start
	Empusa_Air_Damage_Under,		  // em0100_air_damage_under, ���� �ǰ� �� ��� ���� �� ���

	Empusa_Blown_Back,				  // em0100_blown_back_start, ������ �ǰ� �� ���ư��� start
	Empusa_Blown_Back_Landing,		  // em0100_blown_back_landing, ������ �ǰ� �� ���ư��� ���� �΋H��

	Empusa_Blown_Wall_Back,			  // em0100_blown_wall_back, ���� ���� start

	Empusa_Slam_Damage,				  // em0100_slam_damage_start, ���� �ǰ� �� start
	Empusa_Slam_Damage_Loop,		  // em0100_slam_damage_fall_loop, ���� �ǰ� ���� loop
	Empusa_Slam_Damage_Landing,		  // em0100_slam_damage_landing, ���� �ǰ� �� ���� �΋H��

	Empusa_Snatch,					  // em0100_snatch_loop, ����ġ start
	Empusa_Snatch_End,				  // em0000_snatch_end, ����ġ end

	Empusa_Stun,					  // em0100_stun_loop, ���� start
	Empusa_Stun_Revive,				  // em0100_stun_revive, ���� ���¿��� ȸ��

	Empusa_Death_Front,				  // em0100_death_front, �ڷ� �������鼭 ����
	Empusa_Death_Back,				  // em0100_death_back, ������ �������鼭 ����
	Empusa_Downward_Down,			  // em0100_downward_down_start, ������ �Ѿ���
	Empusa_Downward_Down_Loop, 	      // em0100_downward_down_loop, ������ �Ѿ��� ���� loop
	Empusa_Downward_Getup,			  // em0100_downward_down_standup, ������ �Ѿ��� ���¿��� �Ͼ
	Empusa_Downward_Death,			  // em0100_downward_die, ������ �����ִ� ���¿��� Death
	Empusa_Downward_Damage,		      // em0100_downward_damage, ������ �����ִ� ���¿��� ��� ������

	Empusa_Buster_Start,       // em0000_Buster_Start, ����Ʈ ��Ʈ ����
	Empusa_Buster_Loop,        // ����Ʈ ��Ʈ ����
	Empusa_Buster_Finish,      // em0000_Buster_Finish, ����Ʈ ��Ʈ ���� ������

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
	
protected:
	void Start() override;

	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;
	void EnemyCreateFSM_Client() override;
	void DamageCollisionCheck(float _DeltaTime) override;
	void RecognizeCollisionCheck(float _DeltaTime) override;

private:
	void PlayerChase(float _DeltaTime);
	void PlayerAttack(float _DeltaTime);
	void RandomAttack();
	void ChangeState(int _StateValue);

	float WaitTime = 0.0f;
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
};

