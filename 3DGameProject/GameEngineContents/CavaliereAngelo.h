#pragma once

enum FSM_State_CavaliereAngelo
{
	// <basic> ==================================================================================
	CavaliereAngelo_Idle,						 // em5501_defense-Idle : ����ڼ��� ������

	// <special> ==================================================================================
	CavaliereAngelo_Dengeki_Reload_Start,		 // em5501_dengeki_reload_start : ���� ���� ����
	CavaliereAngelo_Dengeki_Reload_Loop,		 // em5501_dengeki_reload_loop : ���� ���� ����
	CavaliereAngelo_Dengeki_Reload_End,			 // em5501_dengeki_reload_end : ���� ���� ��
	CavaliereAngelo_Warp_Start,					 // em5501_warp_start : ���� ����
	CavaliereAngelo_Warp_Offense,				 // em5501_warp_offense : ���� �Ȱ� �� ������(���� �𸣰ڵ�)
	CavaliereAngelo_Warp_End,					 // em5501_warp_end : ���� ��->����ڼ��� ����

	// <move> ==================================================================================
	CavaliereAngelo_Wark_Attack_To_Guard_Start,  // em5501_A-walk_start_L0 : ���ִٰ� ����ڼ� ���ϸ鼭 �ȱ� ����
	CavaliereAngelo_Wark_Guard_Start,            // em5501_guard_to_walk : ����ڼ� �ȱ� ����
	CavaliereAngelo_Wark_Guard_Loop,             // em5501_A-walk_loop : ����ڼ� �ȱ� ����
	CavaliereAngelo_Wark_Guard_End,              // em5501_A-walk_End_L0 : ����ڼ� �ȱ� ��

	CavaliereAngelo_Attack_Dengeki_Start,	     // em5501_Attack_Dengeki_Start : Dengeki �ȱ� ����
	CavaliereAngelo_Attack_Dengeki_Loop,		 // em5501_Attack_Dengeki_Loop : Dengeki �ȱ� ����
	CavaliereAngelo_Attack_Dengeki_End,			 // em5501_Attack_Dengeki_End : Dengeki �ȱ� ��

	CavaliereAngelo_Turn_Attack_To_Guard_Left,   // em5501_Turn_Attack_to_guard_L : ���� ���� �������� 180�� ȸ��
	CavaliereAngelo_Turn_Attack_To_Guard_Right,  // em5501_Turn_Attack_to_guard_R : ���� ���� ���������� 180�� ȸ��
	CavaliereAngelo_Guard_Quick_Turn_Left,       // em5501_guard_quick_turn_L : ����ڼ� ���� �������� 180�� ȸ��
	CavaliereAngelo_Guard_Quick_Turn_Right,      // em5501_guard_quick_turn_R : ����ڼ� ���� ���������� 180�� ȸ��

	// <attack> ==================================================================================
	CavaliereAngelo_Attack_Collider,             // em5501_Attack_collider : ���ִٰ� Į ���� ���ø��� ����
	CavaliereAngelo_Attack_Collider_To_Dengeki,  // em5501_Attack_collider_to_Dengeki : Į ���� ����ִٰ� �����鼭 ������ �޼�
	CavaliereAngelo_Attack_Collider_To_Rakurai,  // em5501_Attack_collider_to_Rakurai : Į ���� ����ִٰ� ������ �ֵθ��鼭 ������ ����ڼ� �ȱ� ����
												 
	CavaliereAngelo_Attack_Rakurai,				 // em5501_Attack_Rakurai : ����ڼ� �ȱ� �ϴٰ� ���� �Ȱ� �޼� ���� ����ֱ�
	CavaliereAngelo_Attack_Rakurai_to_01,		 // em5501_Attack_Rakurai_to_01 : �޼� ���� ����ִٰ� ������ ���� ����(��)
	CavaliereAngelo_Attack_Rakurai_to_stinger,   // em5501_Attack_Rakurai_to_stinger : �޼� ���� ����ִٰ� ������ �ɾ, ���� �ֵθ��� ���� ��
												 
	CavaliereAngelo_Attack_03_to_Rakuraui,       // em5501_Attack_03_to_Rakuraui : ���ִٰ� ����ڼ��� ��ȯ
												 
	CavaliereAngelo_Attack_guard_turn_R,		 // em5501_Attack_guard_turn_R : ���� �ڼ����� ���鼭 ���� ����
	CavaliereAngelo_Attack_kaiten,				 // em5501_Attack_kaiten : ���� ��ġ�� ������ ũ�� �ֵθ� �� ����ڼ� �ȱ� ����
												 
	CavaliereAngelo_Attack_Stinger_To_01,		 // em5501_Attack_stinger_to_01 : Į ������ �÷�ġ�� ���� ��
	CavaliereAngelo_Attack_Stinger01,			 // em5501_Attack_stinger01 : ����ڼ� �ȴٰ� �ߵ��ڼ� ���ϰ� �ֵη�� ���� ��
	CavaliereAngelo_Attack_Stinger02,			 // em5501_Attack_stinger02 : �ߵ� ���� �Ϸ� �� ����ڼ�

	CavaliereAngelo_Attack01,					 // em5501_Attack01_R : ����ڼ� ���� ������ �ֵθ���
	CavaliereAngelo_Attack01_Turn_Back,			 // em5501_Attack01_back_L : ����ڼ� ���� �������� ���鼭 ���� ���� ��
	CavaliereAngelo_Attack01_Turn_Left,			 // em5501_Attack01_turn_L : �������� �� �� ���� ��
	CavaliereAngelo_Attack01_Turn_Right,		 // em5501_Attack01_turn_R : ����ڼ� ���� ���������� ���鼭 ���� ���� ��

	CavaliereAngelo_Attack02,					 // em5501_Attack02 : ������ �Ʒ��� ����ġ�� �� ����ڼ� ��ȯ
	CavaliereAngelo_Attack03,					 // em5501_Attack03 : ������ ũ�� �ֵθ��� �� ����ڼ� ��ȯ
	CavaliereAngelo_Attack04,					 // em5501_Attack04 : ������ ��� ������� �� ����ڼ� ��ȯ
	CavaliereAngelo_Attack04_C1,				 // em5501_Attack04_C1 : ������ ũ�� �÷�ġ�� �� ����ڼ� ��ȯ
	CavaliereAngelo_Attack04_C2,				 // em5501_Attack04_C2 : ������ ��� ������� ���� ��
	CavaliereAngelo_Attack05,					 // em5501_Attack05 : ������ �� ġ�ѵ�� �ִٰ� ����ڼ� ��ȯ

	CavaliereAngelo_Parry_Lose,					 // em5501_Parry_lose : �и� ��ø �� ����
	CavaliereAngelo_Parry_Even01,				 // em5501_Parry_even01 : 1�� ���� �и�
	CavaliereAngelo_Parry_Even01_To_02,			 // em5501_Parry_even01_to_02 : 1������ �и� ���� 2������ ���� ��
	CavaliereAngelo_Parry_Even01_To_04,			 // em5501_Parry_even01_to_04 : 1������ �и� ���� 4������ ���� ��
	CavaliereAngelo_Parry_normal01,				 // em5501_Parry_normal01 : 1������ �и� �� ����(? )
	CavaliereAngelo_Parry_normal01_to_02,		 // em5501_Parry_normal01_to_02 : 1�� ���� �� 2������ ��ȯ ���� ��
	CavaliereAngelo_Parry_normal01_to_03,		 // em5501_Parry_normal01_to_03 : 1�� ���� �� 3������ ��ȯ ���� ��
	CavaliereAngelo_Parry_normal01_to_C1,		 // em5501_Parry_normal01_to_C1 : 1�� ���� �� C1 ���� ��ȯ ���� ��

	CavaliereAngelo_Parry_TATE,					 // em5501_Parry_TATE : ���� ������ �ֵθ���(�и� ? )
	CavaliereAngelo_Parry_TATE_To_Rakurai,		 // em5501_Parry_TATE_to_Rakurai : Tate �� �޼� ġ�ѵ�� ���� ��
	CavaliereAngelo_Parry_TATE_To_05,			 // em5501_Parry_TATE_to_05 : Tate �� 5�� ���� ���� ��
	CavaliereAngelo_Parry_TATE_To_C1,			 // em5501_Parry_TATE_to_C1 : Tate �� C1 ���� ���� ��
	CavaliereAngelo_Parry_Win01,				 // em5501_Parry_win01 : 1�� �и� ��(? )
	CavaliereAngelo_Parry_Win01_to_05,			 // em5501_Parry_win01_to_05 : 1�� �� �� 5������ ��ȯ ���� ��
	CavaliereAngelo_Parry_Win01_to_C1,			 // em5501_Parry_win01_to_C1 : 1�� �� �� C1���� ��ȯ ���� ��
	CavaliereAngelo_Stun_To_02,					 // em5501_Stun_to_02 : ���� �� 2�� ���� ���� ��
	CavaliereAngelo_Warp_End_Stinger,		     // em5501_warp_end_stinger : ���� �� ��Ʈ���� ���� ��

	// <damage> ==================================================================================
	CavaliereAngelo_Dengeki_Reload_Damage,       // em5501_dengeki_reload_damage : ���� ���� �� ���� ���� �� ���
	CavaliereAngelo_Dengeki_Reload_Damage_Left,  // em5501_dengeki_reload_damage_L : ���� ���� �� ���� ���� ���� �� ���
	CavaliereAngelo_Dengeki_Reload_Damage_Right, // em5501_dengeki_reload_damage_R : ���� ���� �� ������ ���� ���� �� ���

	CavaliereAngelo_Damage_Front,				 // em5501_damage_F : ������� �°� ����, ���� �ڼ� ȸ��
	CavaliereAngelo_Damage_Back,				 // em5501_damage_B : �ĸ���� �°� ����, ���� �ڷ� ���� �ڼ� ȸ��
	CavaliereAngelo_Damage_Drill,				 // em5501_Damage_Drill : �������(�帱) �°� ���� (������ ���� �� ��), ���� �ڼ� ȸ��

	CavaliereAngelo_Guard_Front,			     // em5501_guard_F : ����ڼ� ���ִ� ���¿��� ���� ���� �ǰ�
	CavaliereAngelo_Guard_Left,				     // em5501_guard_L : ����ڼ� ���ִ� ���¿��� ���� ���� �ǰ�
	CavaliereAngelo_Guard_Right,			     // em5501_guard_R : ����ڼ� ���ִ� ���¿��� ���� ���� �ǰ�
	CavaliereAngelo_Daeth_Front,				 // em5501_daeth_front : ���� ���� �°� ���

};

// ���� :
class CavaliereAngelo : public BaseEnemyActor
{
public:
	// constrcuter destructer
	CavaliereAngelo();
	~CavaliereAngelo();

	// delete Function
	CavaliereAngelo(const CavaliereAngelo& _Other) = delete;
	CavaliereAngelo(CavaliereAngelo&& _Other) noexcept = delete;
	CavaliereAngelo& operator=(const CavaliereAngelo& _Other) = delete;
	CavaliereAngelo& operator=(CavaliereAngelo&& _Other) noexcept = delete;

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
	std::shared_ptr<class GameEngineCollision> ParryCollision = nullptr;

	void PlayerChase(float _DeltaTime);
	void PlayerAttack(float _DeltaTime);
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

	bool IsPowerUpValue = false;

};