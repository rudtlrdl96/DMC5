#pragma once

enum FSM_State_CavaliereAngelo
{
	// <basic> ==================================================================================
	CavaliereAngelo_Idle, // em5501_defense-Idle : ����ڼ��� ������

	// <move> ==================================================================================
	CavaliereAngelo_Wark_Attack_To_Guard_Start,  // em5501_A-walk_start_L0 : ���ִٰ� ����ڼ� ���ϸ鼭 �ȱ� ����
	CavaliereAngelo_Wark_Guard_Start,            // em5501_guard_to_walk : ����ڼ����� �ȱ� ����
	CavaliereAngelo_Wark_Guard_Loop,             // em5501_A-walk_loop : �ȱ� ����
	CavaliereAngelo_Wark_Guard_End,              // em5501_A-walk_End_L0 : �ȱ� ��
	CavaliereAngelo_Turn_Attack_To_Guard_Left,   // em5501_Turn_Attack_to_guard_L : ����ڼ����� �������� ȸ��
	CavaliereAngelo_Turn_Attack_To_Guard_Right,  // em5501_Turn_Attack_to_guard_R : ����ڼ����� ���������� ȸ��

	// <special> ==================================================================================
	CavaliereAngelo_Dengeki_Reload_Start, // em5501_dengeki_reload_start : ���� ���� ����
	CavaliereAngelo_Dengeki_Reload_Loop,  // em5501_dengeki_reload_loop : ���� ���� ����
	CavaliereAngelo_Dengeki_Reload_End,   // em5501_dengeki_reload_end : ���� ���� ��
	CavaliereAngelo_Warp_Start,           // em5501_warp_start : ���� ����
	CavaliereAngelo_Warp_Offense,         // em5501_warp_offense : ���� �Ȱ� �� ������(���� �𸣰ڵ�)
	CavaliereAngelo_Warp_End,             // em5501_warp_end : ���� ��->����ڼ��� ����

	// <damage> ==================================================================================

	// em5501_blown_up_landing_death : ���� �� ����(������ ����)
	// em5501_daeth_front : ���� ���� �°� ���
	// em5501_damage_B : �ڿ��� �°� �ڷ� ��
	// em5501_Damage_Drill : �帱���� �°� ����, ���� �ڼ� ȸ��
	// em5501_damage_F : ������� �°� ����
	// em5501_dengeki_reload_damage : ���� ���� �� ���� ���� �� ���
	// em5501_dengeki_reload_damage_L : ���� ���� �� ���� ���� ���� �� ���
	// em5501_dengeki_reload_damage_R : ���� ���� �� ������ ���� ���� �� ���
	// em5501_guard_F : ���� ���¿��� ���� ���� �ǰ�
	// em5501_guard_L : ���� ���¿��� ���� ���� �ǰ�
	// em5501_guard_R : ���� ���¿��� ���� ���� �ǰ�
	// em5501_guard_waek : ���� ���¿��� ��¦ ����
	// em5501_guard_quick_turn_L : ���� ���¿��� �������� ���� ȸ��
	// em5501_guard_quick_turn_R : ���� ���¿��� ���������� ���� ȸ��
	// em5501_Parry_lose_to_damage : �и� �� ���� �ǰ�
	// em5501_prone_death : �տ��� ������ ���·� ���
	// em5501_stun_start : ���ϸ°� ��Ʋ�Ÿ�
	// em5501_stun_loop : ��Ʋ�Ÿ� ����
	// em5501_stun_hukki : ���� ����
	// em5501_walk_guard_F : ����ڼ� �ȴ� ���� ���� �ǰ�
	// em5501_walk_guard_L : ����ڼ� �ȴ� ���� ���� �ǰ�
	// em5501_walk_guard_R : ����ڼ� �ȴ� ���� ���� �ǰ�
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
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;
	void EnemyCreateFSM_Client() override;
	void DamageCollisionCheck(float _DeltaTime) override;
	void RecognizeCollisionCheck(float _DeltaTime) override;

private:
	void PlayerChase(float _DeltaTime);
	void PlayerAttack(float _DeltaTime);
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

	bool IsPowerUpValue = false;

};