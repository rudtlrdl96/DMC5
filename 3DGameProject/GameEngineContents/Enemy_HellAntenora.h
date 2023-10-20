#pragma once

enum FSM_State_HellAntenora
{
	HellAntenora_Idle,                       // em0001_Idle_01_loop, ������



	HellAntenora_Counter_Attack,			 // em0001_Attack_counter_attack

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
	void MonsterAttackCollisionOn();
	void MonsterAttackCollisionOff();

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

