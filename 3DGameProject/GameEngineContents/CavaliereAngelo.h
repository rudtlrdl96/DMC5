#pragma once

enum FSM_State_CavaliereAngelo
{
	// <basic> ==================================================================================
	CavaliereAngelo_Idle,						 // em5501_defense-Idle : ����ڼ��� ������

	// <special> ==================================================================================
	CavaliereAngelo_Dengeki_Reload_Start,		 // em5501_dengeki_reload_start : ���� ���� ����
	CavaliereAngelo_Dengeki_Reload_Loop,		 // em5501_dengeki_reload_loop : ���� ���� ����
	CavaliereAngelo_Dengeki_Reload_End,			 // em5501_dengeki_reload_end : ���� ���� ��, 100������ on, 105������ off
	CavaliereAngelo_Warp_Start,					 // em5501_warp_start : ���� ����
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

	CavaliereAngelo_Attack_Guard_Turn_Right,     // em5501_Attack_guard_turn_R : ����ڼ� ���� ���������� 180�� ȸ���ϸ� ����

	// <attack> ==================================================================================
	CavaliereAngelo_Attack_Collider,             // em5501_Attack_collider : ���� ���� Į ���� �� ���� ����
	CavaliereAngelo_Attack_Collider_To_Dengeki,  // em5501_Attack_collider_to_Dengeki : ���� �Ϸ� �� Dengeki �ȱ� ����
	CavaliereAngelo_Attack_Collider_To_Rakurai,  // em5501_Attack_collider_to_Rakurai : ���� �Ϸ� �� Rakurai �ǽ�, ���� ����ڼ� �ȱ� ����
												 
	CavaliereAngelo_Attack_Rakurai,				 // em5501_Attack_Rakurai : ����ڼ� ���� Rakurai �ǽ�, ���� ����ڼ� �ȱ� ����
	CavaliereAngelo_Attack_Rakurai_to_01,		 // em5501_Attack_Rakurai_to_01 : Rakurai �� �ٷ� 01(���ߴ�) ���� ���� ��
	CavaliereAngelo_Attack_Rakurai_to_stinger,   // em5501_Attack_Rakurai_to_stinger : Rakurai �� �ٷ� stinger(���ߴ�-ũ���ֵθ���) ���� ���� ��
												 
	CavaliereAngelo_Attack_Stinger01,			 // em5501_Attack_stinger01 : ����ڼ� �ȴ� ���� �ٷ� stinger(����-ũ���ֵθ���) ���� ���� ��
	CavaliereAngelo_Attack_Stinger02,			 // em5501_Attack_stinger02 : stinger �ǽ�, ���� ����ڼ� �ȱ� ���� end �ִϸ��̼����� ����

	CavaliereAngelo_Attack01,					 // em5501_Attack01_R : 01(���ߴ�) ����, ������ ����ڼ�, 66������ on, 69������ off
	CavaliereAngelo_Attack02,					 // em5501_Attack02 : 02(�����) ����, ������ ����ڼ�, 44������ on, 47������ off
	CavaliereAngelo_Attack03,					 // em5501_Attack03 : 03(���ߴ�) ����, ������ ����ڼ�, 29������ on, 33������ off
	CavaliereAngelo_Attack04,					 // em5501_Attack04 : 04(������� ������) ����, ������ ����ڼ�, 90������ on, 94������ off
	CavaliereAngelo_Attack04_C1,				 // em5501_Attack04_C1 : C1(���� �÷�ġ��) ����, ������ ����ڼ�
	CavaliereAngelo_Attack05,					 // em5501_Attack05 : 05(���)���� �� ����ڼ��� ��ȯ(���и� �� ��� ���� �����ϸ� ���ϴ� �ڼ�)

	CavaliereAngelo_Parry_Even01,				 // em5501_Parry_even01 : �÷��̾� �и� ���� �� �ٷ� ����
	CavaliereAngelo_Parry_Even01_To_02,			 // em5501_Parry_even01_to_02 : 01 �и� �� �ٷ� 02 ���� ��, 37������ ��ŸƮ (38 ���������� ����)
	CavaliereAngelo_Parry_Even01_To_03,			 // em5501_Parry_even01_to_04 : 01 �и� �� �ٷ� 03 ���� ��, 24 ������ ��ŸƮ (23 ���������� ����)
	CavaliereAngelo_Parry_normal01,				 // em5501_Parry_normal01 : �÷��̾� �и� �� ���� �� ����� 
	CavaliereAngelo_Parry_normal01_to_02,		 // em5501_Parry_normal01_to_02 : 01 �и� �� ����, ���� 02 ���� ��, 37������ ��ŸƮ (38 ���������� ����)
	CavaliereAngelo_Parry_normal01_to_03,		 // em5501_Parry_normal01_to_03 : 01 �и� �� ����, ���� 03 ���� ��, 24 ������ ��ŸƮ (23 ���������� ����)
	CavaliereAngelo_Parry_normal01_to_C1,		 // em5501_Parry_normal01_to_C1 : 01 �и� �� ����, ���� C1 ���� ��, 62 ������ ��ŸƮ

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

	CavaliereAngelo_Stun_Start,				     // em5501_stun_start : ������ ��ø ���� ����
	CavaliereAngelo_Stun_Hukki,				     // em5501_stun_hukki : ������ ��ø ���� ��

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

	void MinusEnemyHP(int _Value)
	{
		EnemyHP -= _Value;
	}

	void HPStackPlus(int _Value)
	{
		HPOverallStack += _Value;
	}

	void HPSeverStackPlus(int _Value)
	{
		HPOverallStack += _Value;
		HPServerStack += _Value;
	}

	void HPClientStackPlus(int _Value)
	{
		HPOverallStack += _Value;
		HPClientStack += _Value;
	}

	bool GetIsStun()
	{
		return IsStun;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
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
	std::shared_ptr<class FXSystem> EffectRenderer_0 = nullptr;
	std::shared_ptr<class FXSystem> EffectRenderer_1 = nullptr;
	std::shared_ptr<class Cavaliere_Electric> Electric = nullptr;

	void PlayerChase(float _DeltaTime);
	void PlayerAttack(float _DeltaTime);
	void ChangeState(int _StateValue);
	void ChangeState_Client(int _StateValue);
	void AttackCalculation();
	void ParryCheck();
	void ParryTime();
	void BossTurn();

	float WaitTime = 0.0f;
	float RotationDelayTime = 0.0f;
	float FallCheckDelayTime = 0.0f;
	float AttackDelayCheck = 0.0f;
	float AttackDelayTime = 0.0f;
	float ElectroTime = 0.0f;

	short ParryStack = 0;
	short ColliderStack = 0;

	int ChargeDamageStack = 0;

	bool IsVergilLight = false;
	bool IsSturn = false;
	bool IsRecognize = false;
	bool IsParryCheck = false;    // �и� üũ on off
	bool IsStun = false; // �������� (������ ����)
	bool ParryOkay = false;
	bool Event01 = false;
	bool Normal01 = false;
	bool IsCharge = false;
	bool IsPowerUp = false;
	bool IsFastCharge = false;

};