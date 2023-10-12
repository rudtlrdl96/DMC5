#pragma once
#include "Poolable.h"

enum FSM_State_HellCaina
{
	// ����
	HellCaina_Appear_02,      // em0000_appear_02, ����

	// �⺻
	HellCaina_Idle,           // em0000_Idle_01, �׳� ������
	HellCaina_Menace_Front,   // em0000_menace_01, �������� ����
	HellCaina_Menace_Up,      // em0000_menace_02, ���� �Լ� ����

	// ������
	HellCaina_Walk_Start,       // em0000_walk_start
	HellCaina_Walk_Loop,        // em0000_walk_loop
	HellCaina_Walk_Stop,        // em0000_walk_stop
	HellCaina_Walk_Right_Start, // em0000_walk_right_start
	HellCaina_Walk_Right_Loop,  // em0000_walk_right_loop
	HellCaina_Walk_Right_Stop,  // em0000_walk_right_stop
	HellCaina_Walk_Left_Start,  // em0000_walk_left_start
	HellCaina_Walk_Left_Loop,   // em0000_walk_left_loop
	HellCaina_Walk_Left_Stop,   // em0000_walk_left_stop

	// ȸ��
	HellCaina_Navi_Turn_Left,  // em0000_navi_turn_left_90, ���� ������ ȸ��
	HellCaina_Navi_Turn_Right, // em0000_navi_turn_right_90, ������ ������ ȸ��
	HellCaina_Turn_Left_180,   // em0000_turn_left_180, ���� ȸ��
	HellCaina_Turn_Right_180,  // em0000_turn_right_180, ������ ȸ��

	// ����
	HellCaina_Attack_DownUp,  // em0000_attack_01, �Ʒ����� ���� ����
	HellCaina_Attack_UpDown,  // em0000_attack_02, ������ �Ʒ��� ����
	HellCaina_Attack_Turn,    // em0000_attack_01_turn, �ڷ� ���Ƽ� ����
	HellCaina_Attack_Dash,    // em0000_attack_atackhard, ��������

	// �ǰݷ�
	HellCaina_Standing_Damage_Weak_Front, // em0000_standing_damage_weak_front_01, ���� ����� ��Ʈ
	HellCaina_Standing_Damage_Weak_Back,  // em0000_standing_damage_weak_back_01, �ĸ� ����� ��Ʈ
	HellCaina_Standing_Damage_Weak_Left,  // em0000_standing_damage_weak_left_01, ���� ����� ��Ʈ
	HellCaina_Standing_Damage_Weak_Right, // em0000_standing_damage_weak_right_01, ���� ����� ��Ʈ

	HellCaina_Blown_Up,					  // em0000_blown_up, ���� �ǰ� start
	HellCaina_Air_Damage_Under,			  // em0000_air_damage_under, ���� �ǰ� �� ��� ���� �� ���

	HellCaina_Blown_Back,				  // em0000_blown_back, ������ �ǰ� �� ���ư��� start
	HellCaina_Blown_Back_Loop,			  // em0000_blown_back_loop, ������ �ǰ� �� ���� ��� �� Loop
	HellCaina_Blown_Back_Landing,		  // em0000_blown_back_landing, ������ �ǰ� �� ���ư��� ���� �΋H��

	HellCaina_Blown_Wall_Back,			  // em0000_blown_wall_back, ���� ���� start
	HellCaina_Blown_Wall_Back_Landing,    // em0000_blown_wall_back_landing, ���� �ڰ� ���� �΋H�� ��

	HellCaina_Slam_Damage,				  // em0000_slam_damage, ���� �ǰ� �� start
	HellCaina_Slam_Damage_Loop,			  // em0000_slam_damage_loop, ���� �ǰ� ���� loop
	HellCaina_Slam_Damage_Landing,		  // em0000_slam_damage_landing, ���� �ǰ� �� ���� �΋H��

	HellCaina_Snatch,					  // em0000_snatch, ����ġ start

	HellCaina_Stun,						  // em0000_stun, ���� start
	HellCaina_Stun_Revive,				  // em0000_stun_revive, ���� ���¿��� ȸ��

	HellCaina_Death_Front,				  // em0000_death_front, �ڷ� �������鼭 ����
	HellCaina_Death_Back,				  // em0000_death_back, ������ �������鼭 ����

	HellCaina_Prone_Damage_Gun,			  // em0000_prone_damage_gun, ������ �����ִ� ���¿��� ��� ������
	HellCaina_Prone_Death,				  // em0000_prone_death, ������ �����ִ� ���¿��� Death
	HellCaina_Prone_Down,				  // em0000_prone_down, ������ �Ѿ���
	HellCaina_Prone_Getup,				  // em0000_prone_getup, ������ �Ѿ��� ���¿��� �Ͼ

	HellCaina_Buster_Start,       // em0000_Buster_Start, ����Ʈ ��Ʈ ����
	HellCaina_Buster_Loop,        // ����Ʈ ��Ʈ ����
	HellCaina_Buster_Finish,      // em0000_Buster_Finish, ����Ʈ ��Ʈ ���� ������
};

// ���� :
class Enemy_HellCaina : public BaseEnemyActor, public Poolable<Enemy_HellCaina>
{
public:
	// constrcuter destructer
	Enemy_HellCaina();
	~Enemy_HellCaina();

	// delete Function
	Enemy_HellCaina(const Enemy_HellCaina& _Other) = delete;
	Enemy_HellCaina(Enemy_HellCaina&& _Other) noexcept = delete;
	Enemy_HellCaina& operator=(const Enemy_HellCaina& _Other) = delete;
	Enemy_HellCaina& operator=(Enemy_HellCaina&& _Other) noexcept = delete;

protected:
	void Start() override;
	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;
	void EnemyCreateFSM_Client() override;
	void DamageCollisionCheck(float _DeltaTime) override;
	void DamageCollisionCheck_Client(float _DeltaTime) override;
	void RecognizeCollisionCheck(float _DeltaTime) override;

private:
	void PlayerChase(float _DeltaTime);
	void PlayerAttack(float _DeltaTime);
	void RandomAttack();
	void ChangeState(int _StateValue);
	void ChangeState_Client(int _StateValue);
	void AttackCalculation();

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

