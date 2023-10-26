#pragma once

enum FSM_State_ScudoAngelo
{
	// ����
	ScudoAngelo_Appear_01,							 // em0600_Appear_01, ������
	ScudoAngelo_Response_Stay,						 // em0600_Response_Stay : ������ �� ������ �ִ� �ڼ�, �� �� ������ �ߴٰ� ���ִ� �ڼ��� ��ȯ
	ScudoAngelo_Idle,								 // em0001_Idle_01_loop, ������
	ScudoAngelo_Attack_Idle_Loop,					 // em0600_Attack_Idle_Loop : ���� �ڼ��� ������

	// <move> ======================================================================================== =
	ScudoAngelo_Step_Back_M,						 // em0600_Step_Back_M : �ڷ� ������ M
	ScudoAngelo_Step_Back_S,						 // em0600_Step_Back_S : �ڷ� ������ S

	ScudoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_L, // em0600_Turn_Attack_To_Guard_L_Dummy_Turn_L : ������ ����(�̸� �߸� ����)
	ScudoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_R, // em0600_Turn_Attack_To_Guard_L_Dummy_Turn_R : ���� ����(�̸� �߸� ����)

	ScudoAngelo_Walk_Front_Start,					 // em0600_Walk_Front_Start : ������ �ȱ� ����, 20������ �ȱ� ����
	ScudoAngelo_Walk_Front_Loop,					 // em0600_Walk_Front_Loop : ������ �ȱ� ����
	ScudoAngelo_Walk_Front_Stop,					 // em0600_Walk_Front_Stop : ������ �ȱ� ��, 30������ �ȱ� ��

	// <attck> ======================================================================================== =
	ScudoAngelo_Group_Command_Attack,				 // em0600_Group_Command_Attack : ���� ���� ���
	ScudoAngelo_Group_Command_Attack_End,			 // em0600_Group_Command_Attack_End : ���� ���� ������ ���(�ȶٰ� �ٷ� ���) // 10������ ������ �̵� ����, 55������ ������ �̵� �� // 50������ on, 55������ off // 120 ������ ���� ����, 220 ������ ���� ��
	ScudoAngelo_Attack_T_Hikkake,					 // em0600_Attack_T_Hikkake : �� ���� �� �ڷ�, 13������ ����, 45������ ���� �� // 52������ on, 57������ off // 80 ������ ����, 130 ������ ���� ��
	ScudoAngelo_Attack_T_Kesa,						 // em0600_Attack_T_Kesa : ������ ����, 43������ on, 48������ off
													 
	ScudoAngelo_Attack_T_Run_Start,					 // em0600_Attack_T_Run_Start : �޸��� ���� ����
	ScudoAngelo_Attack_T_Run_Loop,					 // em0600_Attack_T_Run_Loop : �޸��� ���� ����
	ScudoAngelo_Attack_T_Run_Attack,				 // em0600_Attack_T_Run_Attack : ���� ����, 42������ on, 47������ off // 100������ ���� �� // 110������ ���� ����, 180 ������ ���� ��
	ScudoAngelo_Attack_T_Run_Stop_A,				 // em0600_Attack_T_Run_Stop_A : �޸��� ���� ���� // 50������ �ٱ� ��, �ȱ� ����, 100������ �ȱ� �� // 120������ �ȱ� ����, 195������ �ȱ� ��

	// <damage> ======================================================================================== =	
	ScudoAngelo_Standing_Damage_Weak_Front,			 // em0600_Standing_Damage_Weak_Front : �տ��� ��� ����
	ScudoAngelo_Standing_Damage_Weak_Back,			 // em0600_Standing_Damage_Weak_Back : �ڿ��� ��� �°� �ڵ�, 65������ �ִϸ��̼� ��
													 
	ScudoAngelo_Blown_Back,							 // em0600_Blown_Back : ��� ���� ����
	ScudoAngelo_Blown_Back_Loop,					 // em0600_Blown_Back_Loop : ��� ���� ����
	ScudoAngelo_Blown_Back_Landing,					 // em0600_Blown_Back_Landing : ������ ������ ����
													 
	ScudoAngelo_Blown_Up,							 // em0600_Blown_Up : ���� ���� ����
	ScudoAngelo_Blown_Up_Loop,						 // em0600_Blown_Up_Loop : ���� ���� ����
	ScudoAngelo_Blown_Up_Landing,					 // em0600_Blown_Up_Landing : ���� ���� ������ ����
	ScudoAngelo_Air_Damage,							 // em0600_Air_Damage : ���߿��� �¾�����
													 
	ScudoAngelo_Death_Back,							 // em0600_Death_Back : �տ��� �°� ����, 70 ������ �ȱ� ��
	ScudoAngelo_Death_Front,						 // em0600_Death_Front : �ڿ��� �°� ����, 70 ������ �ȱ� ��
													 
	ScudoAngelo_Lie_Death,							 // em0600_Lie_Death : �ڿ� ������ ���·� ����
	ScudoAngelo_Prone_Death,						 // em0600_Prone_Death : ������ ������ ���·� ����
	ScudoAngelo_Prone_Getup,						 // em0600_Prone_Getup : ������ �������ִٰ� �Ͼ, 120������ �ް��� ����, 170������ ����
													 
	ScudoAngelo_Parry_Lose_Modori,					 // em0600_Parry_Lose_Modori : �и� ���� ���
													 
	ScudoAngelo_Slam_Damage,						 // em0600_Slam_Damage : ���� ����
	ScudoAngelo_Slam_Damage_Loop,					 // em0600_Slam_Damage_Loop : ���� ����
	ScudoAngelo_Slam_Damage_Landing,				 // em0600_Slam_Damage_Landing : ���� ������ ����
													 
	ScudoAngelo_Snatch,								 // em0600_Snatch : ����ġ ����
	ScudoAngelo_Snatch_End,							 // em0600_Snatch_End : ����ġ ��
													 
	ScudoAngelo_Buster_Start,						 // em0600_Air_Buster : ������ ����
	ScudoAngelo_Buster_Loop,						 // ������ ����
	ScudoAngelo_Buster_Finish,
													 
	ScudoAngelo_Stun,								 // em0600_Stun : ���� ����
	ScudoAngelo_Stun_Revive,						 // em0600_Stun_Revive : ���� ȸ��
};

// ���� :
class Enemy_ScudoAngelo : public BaseEnemyActor
{
public:
	// constrcuter destructer
	Enemy_ScudoAngelo();
	~Enemy_ScudoAngelo();

	// delete Function
	Enemy_ScudoAngelo(const Enemy_ScudoAngelo& _Other) = delete;
	Enemy_ScudoAngelo(Enemy_ScudoAngelo&& _Other) noexcept = delete;
	Enemy_ScudoAngelo& operator=(const Enemy_ScudoAngelo& _Other) = delete;
	Enemy_ScudoAngelo& operator=(Enemy_ScudoAngelo&& _Other) noexcept = delete;

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
	void MonsterAttackCollisionOn();
	void MonsterAttackCollisionOff();
	void MoveLoop();
	void ParryCheck();
	void ParryTime();

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
	bool ParryEvent = true;
	bool IsSuperArmor = true;
};

