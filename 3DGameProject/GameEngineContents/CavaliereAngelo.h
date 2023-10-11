#pragma once

enum FSM_State_CavaliereAngelo
{
	// <basic> ==================================================================================
	CavaliereAngelo_Idle, // em5501_defense-Idle : 방어자세로 서있음

	// <move> ==================================================================================
	CavaliereAngelo_Wark_Attack_To_Guard_Start,  // em5501_A-walk_start_L0 : 서있다가 방어자세 취하면서 걷기 시작
	CavaliereAngelo_Wark_Guard_Start,            // em5501_guard_to_walk : 방어자세에서 걷기 시작
	CavaliereAngelo_Wark_Guard_Loop,             // em5501_A-walk_loop : 걷기 루프
	CavaliereAngelo_Wark_Guard_End,              // em5501_A-walk_End_L0 : 걷기 끝
	CavaliereAngelo_Turn_Attack_To_Guard_Left,   // em5501_Turn_Attack_to_guard_L : 방어자세에서 왼쪽으로 회전
	CavaliereAngelo_Turn_Attack_To_Guard_Right,  // em5501_Turn_Attack_to_guard_R : 방어자세에서 오른쪽으로 회전

	// <special> ==================================================================================
	CavaliereAngelo_Dengeki_Reload_Start, // em5501_dengeki_reload_start : 번개 충전 시작
	CavaliereAngelo_Dengeki_Reload_Loop,  // em5501_dengeki_reload_loop : 번개 충전 루프
	CavaliereAngelo_Dengeki_Reload_End,   // em5501_dengeki_reload_end : 번개 충전 끝
	CavaliereAngelo_Warp_Start,           // em5501_warp_start : 워프 시작
	CavaliereAngelo_Warp_Offense,         // em5501_warp_offense : 망토 걷고 검 앞으로(먼지 모르겠따)
	CavaliereAngelo_Warp_End,             // em5501_warp_end : 워프 끝->방어자세로 변경

	// <damage> ==================================================================================

	// em5501_blown_up_landing_death : 랜딩 후 데스(가만히 있음)
	// em5501_daeth_front : 정면 공격 맞고 사망
	// em5501_damage_B : 뒤에서 맞고 뒤로 돔
	// em5501_Damage_Drill : 드릴공격 맞고 스턴, 이후 자세 회복
	// em5501_damage_F : 정면공격 맞고 스턴
	// em5501_dengeki_reload_damage : 번개 충전 중 공격 맞을 때 모션
	// em5501_dengeki_reload_damage_L : 번개 충전 중 왼쪽 공격 맞을 때 모션
	// em5501_dengeki_reload_damage_R : 번개 충전 중 오른쪽 공격 맞을 때 모션
	// em5501_guard_F : 가드 상태에서 정면 공격 피격
	// em5501_guard_L : 가드 상태에서 좌측 공격 피격
	// em5501_guard_R : 가드 상태에서 우측 공격 피격
	// em5501_guard_waek : 가드 상태에서 살짝 스턴
	// em5501_guard_quick_turn_L : 가드 상태에서 왼쪽으로 빠른 회전
	// em5501_guard_quick_turn_R : 가드 상태에서 오른쪽으로 빠른 회전
	// em5501_Parry_lose_to_damage : 패리 중 공격 피격
	// em5501_prone_death : 앞에서 쓰러진 상태로 사망
	// em5501_stun_start : 스턴맞고 비틀거림
	// em5501_stun_loop : 비틀거림 유지
	// em5501_stun_hukki : 스턴 종료
	// em5501_walk_guard_F : 방어자세 걷는 도중 정면 피격
	// em5501_walk_guard_L : 방어자세 걷는 도중 좌측 피격
	// em5501_walk_guard_R : 방어자세 걷는 도중 우측 피격
};

// 설명 :
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

	bool IsHeavyAttack = false;   // 강공격 히트
	bool IsAirAttack = false;     // 에어공격 히트
	bool IsSlamAttack = false;    // 슬램공격 히트
	bool IsBusterAttack = false;  // 버스터 히트
	bool IsVergilLight = false;
	bool IsCollapse = false;      // 쓰러져있는 상태
	bool IsRecognize = false;

	bool IsPowerUpValue = false;

};