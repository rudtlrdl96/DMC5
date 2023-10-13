#pragma once

enum FSM_State_CavaliereAngelo
{
	// <basic> ==================================================================================
	CavaliereAngelo_Idle, // em5501_defense-Idle : 방어자세로 서있음

	// <special> ==================================================================================
	CavaliereAngelo_Dengeki_Reload_Start, // em5501_dengeki_reload_start : 번개 충전 시작
	CavaliereAngelo_Dengeki_Reload_Loop,  // em5501_dengeki_reload_loop : 번개 충전 루프
	CavaliereAngelo_Dengeki_Reload_End,   // em5501_dengeki_reload_end : 번개 충전 끝
	CavaliereAngelo_Warp_Start,           // em5501_warp_start : 워프 시작
	CavaliereAngelo_Warp_Offense,         // em5501_warp_offense : 망토 걷고 검 앞으로(먼지 모르겠따)
	CavaliereAngelo_Warp_End,             // em5501_warp_end : 워프 끝->방어자세로 변경

	// <move> ==================================================================================
	CavaliereAngelo_Wark_Attack_To_Guard_Start,  // em5501_A-walk_start_L0 : 서있다가 방어자세 취하면서 걷기 시작
	CavaliereAngelo_Wark_Guard_Start,            // em5501_guard_to_walk : 방어자세에서 걷기 시작
	CavaliereAngelo_Wark_Guard_Loop,             // em5501_A-walk_loop : 걷기 루프
	CavaliereAngelo_Wark_Guard_End,              // em5501_A-walk_End_L0 : 걷기 끝
	CavaliereAngelo_Turn_Attack_To_Guard_Left,   // em5501_Turn_Attack_to_guard_L : 방어자세에서 왼쪽으로 회전
	CavaliereAngelo_Turn_Attack_To_Guard_Right,  // em5501_Turn_Attack_to_guard_R : 방어자세에서 오른쪽으로 회전
	CavaliereAngelo_Guard_Quick_Turn_Left,       // em5501_guard_quick_turn_L : 가드 상태에서 왼쪽으로 빠른 회전
	CavaliereAngelo_Guard_Quick_Turn_Right,      // em5501_guard_quick_turn_R : 가드 상태에서 오른쪽으로 빠른 회전

	// <attack> ==================================================================================
	CavaliereAngelo_Attack_Collider,            // em5501_Attack_collider : 서있다가 칼 위로 들어올리기 시작
	CavaliereAngelo_Attack_Collider_To_Dengeki, // em5501_Attack_collider_to_Dengeki : 칼 위로 들고있다가 내리면서 앞으로 왼손
	CavaliereAngelo_Attack_Collider_To_Rakurai, // em5501_Attack_collider_to_Rakurai : 칼 위로 들고있다가 앞으로 휘두르면서 내리고 방어자세 걷기 시작

	CavaliereAngelo_Attack_Dengeki_Start,	    // em5501_Attack_Dengeki_Start : 앞으로 걷는 방어자세 풀면서 앞으로 왼손
	CavaliereAngelo_Attack_Dengeki_Loop,		// em5501_Attack_Dengeki_Loop : 앞으로 왼손 루프
	CavaliereAngelo_Attack_Dengeki_End,			// em5501_Attack_Dengeki_End : 손 위로 들고있다가 한번 꺾고 내리면서 방어자세 걷기 시작

	CavaliereAngelo_Attack_Rakurai,				// em5501_Attack_Rakurai : 방어자세 걷기 하다가 망토 걷고 왼손 위로 들고있기
	CavaliereAngelo_Attack_Rakurai_to_01,		// em5501_Attack_Rakurai_to_01 : 왼손 위로 들고있다가 앞으로 공격 시작(전)
	CavaliereAngelo_Attack_Rakurai_to_stinger,  // em5501_Attack_Rakurai_to_stinger : 왼손 위로 들고있다가 앞으로 걸어감, 이후 휘두르기 시작 전

	CavaliereAngelo_Attack_03_to_Rakuraui,      // em5501_Attack_03_to_Rakuraui : 서있다가 방어자세로 전환

	CavaliereAngelo_Attack_guard_turn_R,		// em5501_Attack_guard_turn_R : 가드 자세에서 돌면서 공격 시작
	CavaliereAngelo_Attack_kaiten,				// em5501_Attack_kaiten : 망토 펼치고 앞으로 크게 휘두른 뒤 방어자세 걷기 시작

	CavaliereAngelo_Attack_Stinger_To_01,		// em5501_Attack_stinger_to_01 : 칼 돌려서 올려치기 시작 전
	CavaliereAngelo_Attack_Stinger01,			// em5501_Attack_stinger01 : 방어자세 걷다가 발도자세 취하고 휘두루기 시작 전
	CavaliereAngelo_Attack_Stinger02,			// em5501_Attack_stinger02 : 발도 공격 완료 후 방어자세
	CavaliereAngelo_Attack01,					// em5501_Attack01_R : 방어자세 도중 앞으로 휘두르기
	CavaliereAngelo_Attack01_Turn_Back,			// em5501_Attack01_back_L : 방어자세 도중 왼쪽으로 돌면서 공격 시작 전
	CavaliereAngelo_Attack01_Turn_Left,			// em5501_Attack01_turn_L : 왼쪽으로 돈 후 공격 중
	CavaliereAngelo_Attack01_Turn_Right,		// em5501_Attack01_turn_R : 방어자세 도중 오른쪽으로 돌면서 공격 시작 전
	CavaliereAngelo_Attack02,					// em5501_Attack02 : 위에서 아래로 내려치기 후 방어자세 전환
	CavaliereAngelo_Attack03,					// em5501_Attack03 : 앞으로 크게 휘두르기 후 방어자세 전환
	CavaliereAngelo_Attack04,					// em5501_Attack04 : 앞으로 양손 내려찍기 후 방어자세 전환
	CavaliereAngelo_Attack04_C1,				// em5501_Attack04_C1 : 앞으로 크게 올려치기 후 방어자세 전환
	CavaliereAngelo_Attack04_C2,				// em5501_Attack04_C2 : 앞으로 양손 내려찍기 시작 전
	CavaliereAngelo_Attack05,					// em5501_Attack05 : 앞으로 검 치켜들고 있다가 방어자세 전환
	CavaliereAngelo_Parry_Lose,					// em5501_Parry_lose : 패리 중첩 후 스턴
	CavaliereAngelo_Parry_Even01,				// em5501_Parry_even01 : 1번 공격 패리
	CavaliereAngelo_Parry_Even01_To_02,			// em5501_Parry_even01_to_02 : 1번공격 패리 이후 2번공격 시작 전
	CavaliereAngelo_Parry_Even01_To_04,			// em5501_Parry_even01_to_04 : 1번공격 패리 이후 4번공격 시작 전
	CavaliereAngelo_Parry_normal01,				// em5501_Parry_normal01 : 1번공격 패리 후 스턴(? )
	CavaliereAngelo_Parry_normal01_to_02,		// em5501_Parry_normal01_to_02 : 1번 스턴 후 2번공격 전환 시작 전
	CavaliereAngelo_Parry_normal01_to_03,		// em5501_Parry_normal01_to_03 : 1번 스턴 후 3번공격 전환 시작 전
	CavaliereAngelo_Parry_normal01_to_C1,		// em5501_Parry_normal01_to_C1 : 1번 스턴 후 C1 공격 전환 시작 전
	CavaliereAngelo_Parry_TATE,					// em5501_Parry_TATE : 망토 앞으로 휘두르기(패리 ? )
	CavaliereAngelo_Parry_TATE_To_Rakurai,		// em5501_Parry_TATE_to_Rakurai : Tate 후 왼손 치켜들기 시작 전
	CavaliereAngelo_Parry_TATE_To_05,			// em5501_Parry_TATE_to_05 : Tate 후 5번 공격 시작 전
	CavaliereAngelo_Parry_TATE_To_C1,			// em5501_Parry_TATE_to_C1 : Tate 후 C1 공격 시작 전
	CavaliereAngelo_Parry_Win01,		// em5501_Parry_win01 : 1번 패리 윈(? )
	CavaliereAngelo_Parry_Win01_to_05,  // em5501_Parry_win01_to_05 : 1번 윈 후 5번공격 전환 시작 전
	CavaliereAngelo_Parry_Win01_to_C1,  // em5501_Parry_win01_to_C1 : 1번 윈 후 C1공격 전환 시작 전
	CavaliereAngelo_Stun_To_02,		    // em5501_Stun_to_02 : 스턴 후 2번 공격 시작 전
	CavaliereAngelo_Warp_End_Stinger,   // em5501_warp_end_stinger : 워프 후 스트링거 시작 전

	// <damage> ==================================================================================
	CavaliereAngelo_Damage_Front,  // em5501_damage_F : 정면공격 맞고 스턴
	CavaliereAngelo_Damage_Back,   // em5501_damage_B : 뒤에서 맞고 뒤로 돔
	CavaliereAngelo_Damage_Drill,  // em5501_Damage_Drill : 드릴공격 맞고 스턴, 이후 자세 회복
	CavaliereAngelo_Dengeki_Reload_Damage,       // em5501_dengeki_reload_damage : 번개 충전 중 공격 맞을 때 모션
	CavaliereAngelo_Dengeki_Reload_Damage_Left,  // em5501_dengeki_reload_damage_L : 번개 충전 중 왼쪽 공격 맞을 때 모션
	CavaliereAngelo_Dengeki_Reload_Damage_Right, // em5501_dengeki_reload_damage_R : 번개 충전 중 오른쪽 공격 맞을 때 모션
	CavaliereAngelo_Guard_Front,			     // em5501_guard_F : 가드 상태에서 정면 공격 피격
	CavaliereAngelo_Guard_Left,				     // em5501_guard_L : 가드 상태에서 좌측 공격 피격
	CavaliereAngelo_Guard_Right,			     // em5501_guard_R : 가드 상태에서 우측 공격 피격
	CavaliereAngelo_Guard_Waek,				     // em5501_guard_waek : 가드 상태에서 살짝 스턴
	CavaliereAngelo_Parry_Lose_Damage,           // em5501_Parry_lose_to_damage : 패리 중 공격 피격
	CavaliereAngelo_Daeth_Front,		    // em5501_daeth_front : 정면 공격 맞고 사망
	CavaliereAngelo_Death_Prone,		    // em5501_prone_death : 앞에서 쓰러진 상태로 사망
	CavaliereAngelo_Blown_Up_Landing_Death, // em5501_blown_up_landing_death : 앞으로 엎어진 후 가만히 있음

	CavaliereAngelo_Stun_Start,               // em5501_stun_start : 스턴맞고 비틀거림
	CavaliereAngelo_Stun_Loop,                // em5501_stun_loop : 비틀거림 유지
	CavaliereAngelo_Stun_Hukki,               // em5501_stun_hukki : 스턴 종료
	CavaliereAngelo_Walk_Guard_Damage_Front,  // em5501_walk_guard_F : 방어자세 걷는 도중 정면 피격
	CavaliereAngelo_Walk_Guard_Damage_Left,   // em5501_walk_guard_L : 방어자세 걷는 도중 좌측 피격
	CavaliereAngelo_Walk_Guard_Damage_Right,  // em5501_walk_guard_R : 방어자세 걷는 도중 우측 피격
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

	bool IsHeavyAttack = false;   // 강공격 히트
	bool IsAirAttack = false;     // 에어공격 히트
	bool IsSlamAttack = false;    // 슬램공격 히트
	bool IsBusterAttack = false;  // 버스터 히트
	bool IsVergilLight = false;
	bool IsCollapse = false;      // 쓰러져있는 상태
	bool IsRecognize = false;

	bool IsPowerUpValue = false;

};