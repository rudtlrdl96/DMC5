#pragma once

#include "Poolable.h"

enum FSM_State_HellAntenora
{
	HellAntenora_Idle,                       // em0001_Idle_01_loop, ������

	HellAntenora_Menace_01_Loop,			 // em0001_menace_01_loop : �θ����Ÿ�
	HellAntenora_Menace_02,					 // em0001_menace_02 : Į ���� �ȾҴٰ� ����

	HellAntenora_Appear_01,					 // em0001_appear_01 : �ٴڿ��� ���� 1
	HellAntenora_Appear_02,					 // em0001_appear_02 : �ٴڿ��� ���� 2
	HellAntenora_Appear_05,					 // em0001_appear_05 : ������ ����
	HellAntenora_Appear_30,					 // em0001_appear_30 : ������  ����

	HellAntenora_fall_loop,					 // em0001_fall_loop : ������ ���� �� ��������
	HellAntenora_fall_landing,				 // em0001_fall_landing : ������ ���� �� ����

	HellAntenora_Navi_Turn_Left,			 // em0001_turn_left_90_noTransRot : ��������
	HellAntenora_Turn_Left_180,				 // em0001_turn_left_180 : 25������ �ִϸ��̼� ��
	HellAntenora_Navi_Turn_Right,			 // em0001_turn_right_90_noTransRot : ��������
	HellAntenora_Turn_Right_180,			 // em0001_turn_right_180 : 25������ �ִϸ��̼� ��

	HellAntenora_Walk_Start,				 // em0001_walk_start : �ȱ� ����
	HellAntenora_Walk_Loop,					 // em0001_walk_loop : �ȱ� ����
	HellAntenora_Walk_End,					 // em0001_walk_stop : �ȱ� ��

	HellAntenora_Attack_Counter_JumpAttack_Start, // em0001_Attack_Counter_JumpAttack_Start : �������� ��ŸƮ, 66 on, 71 off // �Ⱦ���
	HellAntenora_Attack_Counter_JumpAttack_End,   // em0001_Attack_Counter_JumpAttack_End : �������� �ص�                    // �Ⱦ���

	HellAntenora_Attack_Counter_Start,			  // em0001_Attack_counter_Start : ���� �� �ڱ�����
	HellAntenora_Attack_Counter_Landing,		  // em0001_Attack_counter_landing : ����ȴٰ� ����
	HellAntenora_Attack_Counter_Loop,			  // em0001_Attack_counter_loop : ���� ����
	HellAntenora_Attack_Counter_Attack,			  // em0001_Attack_counter_attack : ������ �ֵθ��� �ڷ� ��, 38 on, 43 off / 55 ������ �ִϸ��̼� ��
	HellAntenora_Attack_Counter_Stop,			  // em0001_Attack_counter_stop : ���� ����

	HellAntenora_Counter_Damage,				  // em0001_counter_s_damage : ��� �¾����� �ݰ� ���          // �Ⱦ���
	HellAntenora_Counter_Short_Attack,			  // em0001_counter_short_attack : ��Ʋ�Ÿ��ٰ� �Ͼ�鼭 ����

	HellAntenora_Multiattack_Start,					  // em0001_multiattack_Start : �����ʿ��� �������� �ֵθ��� ���� ����
	HellAntenora_Multiattack_SideAttack_to_Right,     // em0001_multiattack_SideAttack_to_R : �����ʿ��� �������� ũ�� �ֵθ�, 22 R on, 27 R Off / 37 L on, 42 L Off
	HellAntenora_Multiattack_From_Right_End,		  // em0001_multiattack_from_R_End : �����ʿ��� �������� ũ�� �ֵθ� ��
	HellAntenora_Multiattack_To_Attack_End_Right,	  // em0001_multiattack_to_attack_End_R : �����ʿ��� �������� �ֵθ� �� ������ ��� �������, 73 on, 78 off
	HellAntenora_Multiattack_UpAttack_To_Right,		  // em0001_multiattack_UpAttack_to_R : �����ʿ��� �������� �ֵθ� �� �ٽ� ���� ���� �ֵθ��� �������, 60 R on, 65 R Off, 76 L On, 81 L Off

	HellAntenora_Multiattack_SideAttack_To_Left,	  // em0001_multiattack_SideAttack_to_L : ���ʿ��� ���������� ũ�� �ֵθ�, 22 L on, 27 L off / 37 R on, 42 off
	HellAntenora_Multiattack_From_Left_End,			  // em0001_multiattack_from_L_End : ���ʿ��� ���������� ũ�� �ֵθ� ��
	HellAntenora_Multiattack_To_Attack_End_Left,	  // em0001_multiattack_to_attack_End_L : ���ʿ��� ���������� �ֵθ� �� ������ ��� �������, 73 on, 78 off
	HellAntenora_Multiattack_UpAttack_To_Left,		  // em0001_multiattack_UpAttack_to_L : ���ʿ��� ���������� �ֵθ� �� �ٽ� ������ ���� �ֵθ��� �������, 61 L on, 66 L Off, 76 R On, 81 R Off

	HellAntenora_Turn_Attack_Left,					  // em0001_turn_attack_L : ���� �ڷ� ���鼭 ����, 78 ������ �ִϸ��̼� ��, 84 L on, 89 L Off
	HellAntenora_Turn_Attack_Right,					  // em0001_turn_attack_R : ������ �ڷ� ���鼭 ����, 78 ������ �ִϸ��̼� ��, 85 L on, 90 L Off

	HellAntenora_Standing_Damage_Weak_Front, // em0001_standing_damage_weak_front_01 : ���� ��� ��Ʈ
	HellAntenora_Standing_Damage_Weak_Back,  // em0001_standing_damage_weak_back_01 : �ڿ��� ��� ��Ʈ��, 105 ������ �ִϸ��̼� ��
	HellAntenora_Standing_Damage_Weak_Left,  // em0001_standing_damage_weak_left_01 : ���� ��� ��Ʈ, 115 ������ �ִϸ��̼� ��
	HellAntenora_Standing_Damage_Weak_Right, // em0001_standing_damage_weak_right : ������ ��� ��Ʈ, 100 ������ �ִϸ��̼� ��

	HellAntenora_Blown_Back,				 // em0001_blown_back : ������ ��Ʈ��
	HellAntenora_Blown_Back_Loop,			 // em0001_blown_back_loop : ������ ����    // ���� �Ⱦ�
	HellAntenora_Blown_Back_Landing,		 // em0001_blown_back_landing : ������ ����

	HellAntenora_Air_Damage_Under,			 // em0001_air_damage_gun : ������� ���¿��� ��Ʈ��
	HellAntenora_Blown_Up,					 // em0001_blown_up : ������� ��Ʈ��
	HellAntenora_Blown_Up_Loop,				 // em0001_blown_up_loop : ������� ����
	HellAntenora_Blown_Up_Landing,			 // em0001_blown_up_landing : ������� ���� �� ���� ���������� ��Ʋ�Ÿ��ٰ� �������� ����

	HellAntenora_Slam_Damage,				 // em0001_slam_damage : ���� ���� ��Ʈ��
	HellAntenora_Slam_Damage_Loop,			 // em0001_slam_damage_loop : ���� ���� ����
	HellAntenora_Slam_Damage_Landing,		 // em0001_slam_damage_landing : ���� ���� ������

	HellAntenora_Snatch,					 // em0001_snatch : ����ġ ��Ʈ��

	HellAntenora_Stun,						 // em0001_stun : ���� ��Ʈ��
	HellAntenora_Stun_Revive,				 // em0001_stun_revive : ���� ȸ��

	HellAntenora_Prone_Getup,				 //em0001_prone_getup : ������ ������ ���¿��� �Ͼ
	HellAntenora_Prone_Loop,				 //em0001_prone_loop : ������ ������ ���� ����

	HellAntenora_Death_Back,				 // em0001_death_back : ������ �������鼭 ����
	HellAntenora_Death_Front,				 // em0001_death_front : �ڷ� �������鼭 ����
	HellAntenora_Prone_Death,				 // em0001_prone_death : ������ ������ ���¿��� ����
	HellAntenora_Lie_Death,					 // em0001_lie_death : �ڷ� ������ ���¿��� ����

	HellAntenora_Buster_Start,				 // em0001_buster_loop : ������ ��Ʈ
	HellAntenora_Buster_Loop,				 // ������ �¾����� ����
	HellAntenora_Buster_Finish,              // ������ ���� ����

};

// ���� :
class Enemy_HellAntenora : public BaseEnemyActor, public Poolable<Enemy_HellAntenora>
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

private:
	void PlayerChase();
	void ChangeState(int _StateValue);
	void ChangeState_Client(int _StateValue);
	void AttackCalculation();
	void MonsterAttackCollisionOn();
	void MonsterAttackCollisionOff();
	void MoveLoop();

	float WaitTime = 0.0f;
	float RunTime = 0.0f;
	float RotationDelayTime = 0.0f;
	float FallCheckDelayTime = 0.0f;
	float AttackDelayCheck = 0.0f;
	float AttackDelayTime = 0.0f;

	short WalkCount = 0;
	short MultiAttackStack = 0;

	bool IsHeavyAttack = false;   // ������ ��Ʈ
	bool IsAirAttack = false;     // ������� ��Ʈ
	bool IsSlamAttack = false;    // �������� ��Ʈ
	bool IsBusterAttack = false;  // ������ ��Ʈ
	bool IsVergilLight = false;
	bool IsCollapse = false;      // �������ִ� ����
	bool IsRecognize = false;
	bool AttackTypeChange = false;
	bool DashAttackSetting = false;
	bool IsAirAttackSetting = false;
};

