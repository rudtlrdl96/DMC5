#pragma once

#include "Poolable.h"

enum FSM_State_ProtoAngelo
{
	ProtoAngelo_Appear_01,							 // em0600_Appear_01, ����
	ProtoAngelo_Idle,								 // em0601_Idle_Loop, ������

	// <move> ======================================================================================== =
	ProtoAngelo_Step_Back,							 // em0601_Step_Back : �ڷ� ������

	ProtoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_L, // em0601_Turn_Attack_To_Quard_L : ������ ����(�̸� �߸� ����), 23������ �ִϸ��̼� ��
	ProtoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_R, // em0601_Turn_Attack_To_Quard_R : ���� ����(�̸� �߸� ����), 20������ �ִϸ��̼� �� // 10������ ���� ����,  64������ ���� ����

	ProtoAngelo_Walk_Front_Start,					 // em0601_A_Walk_Start_L0 : ������ �ȱ� ����, 18������ �ȱ� ����
	ProtoAngelo_Walk_Front_Loop,					 // em0601_A_Walk_Loop : ������ �ȱ� ����
	ProtoAngelo_Walk_Front_Stop,					 // em0601_A_Walk_End_L0 : ������ �ȱ� ��, 20������ �ȱ� ��

	// <attck> ======================================================================================== =
	ProtoAngelo_Attack_01_Turn_L,					 // em0601_Attack_01_Turn_L : �������� 180�� ���鼭 �������(04) ���� ���� ��, 10������ �ִϸ��̼� ��
	ProtoAngelo_Attack_01_Turn_R,					 // em0601_Attack_01_Turn_R : ���������� 180�� ���鼭 �ߵ�(03) ���� ���� ��, 40������ �ִϸ��̼� ��
	ProtoAngelo_Attack_03,							 // em0601_Attack_03 : �ߵ�, 38������ ���� �� �������(04)�� ��ȯ ���� // 28������ on 33������ off // 115������ ����, 165������(End) �� // 
	ProtoAngelo_Attack_04,							 // em0601_Attack_04 : �ߵ� �� �������, 53������ ���� �� ������� ����(04)���� ��ȯ ���� // 88������ on 93������ off // 165������ ����, 227������(End) ��
	ProtoAngelo_Attack_Kesa,						 // em0601_Attack_Kesa : ������ ª�� ���� // 30������ ����, 54������ ���� �� // 60������ on, 65������ off // 169 ������ ���� ����, 203������ ���� ��

	ProtoAngelo_Attack_Dash_Tatakituke_Start,		 // em0601_Attack_Dash_Tatakituke_Start : �ڼ� ��� ���� ����, Tatakituke�� ���������� ���� ������� // 100������ ���� ����
	ProtoAngelo_Attack_Dash_Tatakituke_Loop,		 // em0601_Attack_Dash_Tatakituke_Loop : ŸŸŰ ���� ����
	ProtoAngelo_Attack_Dash_Tatakituke,				 // em0601_Attack_Dash_Tatakituke : ���� �� �߰� �� ���� // 10������ ���� ��� ���� // 40������ ���� // 51������ on 56������ off // 56������ ���� ���� 81������ ���� �� // 134������ ���� ���� 158������ ���� ��

	ProtoAngelo_Attack_Tossin_Parry_Start,			 // em0601_Attack_Tossin_Parry_Start : �ڼ���� ���� ����, Tossin�� �������� ���� ������� // 60������ ���� ����
	ProtoAngelo_Attack_Tossin_Parry_Loop,			 // em0601_Attack_Tossin_Parry_Loop : ��� ���� ����
	ProtoAngelo_Attack_Tossin_Parry_End,			 // em0601_Attack_Tossin_Parry_End : ���� �� // 45������ ���� ���� // 85������ ���� ����, 151������ ���� ����
	ProtoAngelo_Attack_Tossin_Parry_End_Turn,		 // em0601_Attack_Tossin_Parry_End_Turn : ���� ���� �� �ڷ� �� // 140������ ���� ���� // 75������ �ִϸ��̼� �� 
	ProtoAngelo_Attack_Tossin_Parry_Attack,          // em0601_Attack_Tossin_Parry_Yokonagi : �߽߰� ����

	ProtoAngelo_Parry_Attack_Start,					 // em0601_Parry_Attack_Start : ���ִٰ� ��� �ڼ��� ���� // 15������ ���� ����, 45������ ���� ����
	ProtoAngelo_Parry_Attack_Loop,					 // em0601_Parry_Attack_Loop : ��� �޸��� ���� �ڼ� ����
	ProtoAngelo_Parry_Attack_End,					 // em0601_Parry_Attack_End : ��� �и��ڼ����� ���ִ� �ڼ��� ����
	ProtoAngelo_Attack_Tossin_Parry_Yokonagi,		 // em0601_Attack_Tossin_Parry_Yokonagi : ���� �� �߰� �� ȸ������(�ߵ�, 03) // 45������ ���� ���� // 64������ On 69������ Off

	// <damage> ======================================================================================== =	
	ProtoAngelo_Standing_Damage_Weak_Front,			 // em0601_Standing_Damage_Weak_Front : �տ��� ��� ����
	ProtoAngelo_Standing_Damage_Weak_Back,			 // em0601_Standing_Damage_Weak_Back : �ڿ��� ��� �°� �ڵ�, 65������ �ִϸ��̼� ��

	ProtoAngelo_Blown_Back,							 // em0601_Blown_Back : ��� ���� ����
	ProtoAngelo_Blown_Back_Loop,					 // em0601_Blown_Back_Loop : ��� ���� ����
	ProtoAngelo_Blown_Back_Landing,					 // em0601_Blown_Back_Landing : ������ ������ ����

	ProtoAngelo_Blown_Up,							 // em0601_Blown_Up : ���� ���� ����
	ProtoAngelo_Blown_Up_Loop,						 // em0601_Blown_Up_Loop : ���� ���� ����
	ProtoAngelo_Blown_Up_Landing,					 // em0601_Blown_Up_Landing : ���� ���� ������ ����
	ProtoAngelo_Air_Damage,							 // em0601_Air_Damage : ���߿��� �¾�����

	ProtoAngelo_Death_Back,							 // em0601_Death_Back : �տ��� �°� ����, 70 ������ �ȱ� ��
	ProtoAngelo_Death_Front,						 // em0601_Death_Front : �ڿ��� �°� ����, 70 ������ �ȱ� ��

	ProtoAngelo_Lie_Death,							 // em0601_Lie_Death : �ڿ� ������ ���·� ����
	ProtoAngelo_Prone_Death,						 // em0601_Prone_Death : ������ ������ ���·� ����
	ProtoAngelo_Prone_Getup,						 // em0601_Prone_Getup : ������ �������ִٰ� �Ͼ, 120������ �ް��� ����, 170������ ����

	ProtoAngelo_Parry_Lose_Modori,					 // em0601_Parry_Lose_Modori : �и� ���� ���

	ProtoAngelo_Slam_Damage,						 // em0601_Slam_Damage : ���� ����
	ProtoAngelo_Slam_Damage_Loop,					 // em0601_Slam_Damage_Loop : ���� ����
	ProtoAngelo_Slam_Damage_Landing,				 // em0601_Slam_Damage_Landing : ���� ������ ����

	ProtoAngelo_Snatch,								 // em0601_Snatch : ����ġ ����
	ProtoAngelo_Snatch_End,							 // em0601_Snatch_End : ����ġ ��

	ProtoAngelo_Buster_Start,						 // em0601_Air_Buster : ������ ����
	ProtoAngelo_Buster_Loop,						 // ������ ����
	ProtoAngelo_Buster_Finish,

	ProtoAngelo_Stun,								 // em0601_Stun : ���� ����
	ProtoAngelo_Stun_Revive,						 // em0601_Stun_Revive : ���� ȸ��

};

// ���� :
class Enemy_ProtoAngelo : public BaseEnemyActor, public Poolable<Enemy_ProtoAngelo>
{
public:
	// constrcuter destructer
	Enemy_ProtoAngelo();
	~Enemy_ProtoAngelo();

	// delete Function
	Enemy_ProtoAngelo(const Enemy_ProtoAngelo& _Other) = delete;
	Enemy_ProtoAngelo(Enemy_ProtoAngelo&& _Other) noexcept = delete;
	Enemy_ProtoAngelo& operator=(const Enemy_ProtoAngelo& _Other) = delete;
	Enemy_ProtoAngelo& operator=(Enemy_ProtoAngelo&& _Other) noexcept = delete;

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
	void PlayerAttack();
	void ChangeState(int _StateValue);
	void ChangeState_Client(int _StateValue);
	void AttackCalculation();
	void MoveLoop();
	void ParryCheck();
	void ParryCheck_Client();
	void ParryTime();
	void PlayDamageEvent(DamageSoundType _Type, bool _PlayVoice);

	std::shared_ptr<class GameEngineCollision> ParryCollision = nullptr;

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
	bool IsParryCheck = false;    // �и� üũ on off
	bool ParryOkay = false;
	bool ParryEvent = false;
	bool IsSuperArmor = false;

	bool RightAttackEvent = false;
};

