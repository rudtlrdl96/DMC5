#pragma once

enum FSM_State_CavaliereAngelo
{
	// <basic> ==================================================================================
	CavaliereAngelo_Idle,						 // em5501_defense-Idle : 방어자세로 서있음

	// <special> ==================================================================================
	CavaliereAngelo_Dengeki_Reload_Start,		 // em5501_dengeki_reload_start : 번개 충전 시작
	CavaliereAngelo_Dengeki_Reload_Loop,		 // em5501_dengeki_reload_loop : 번개 충전 루프
	CavaliereAngelo_Dengeki_Reload_End,			 // em5501_dengeki_reload_end : 번개 충전 끝, 100프레임 on, 105프레임 off
	CavaliereAngelo_Warp_Start,					 // em5501_warp_start : 워프 시작
	CavaliereAngelo_Warp_End,					 // em5501_warp_end : 워프 끝->방어자세로 변경

	// <move> ==================================================================================
	CavaliereAngelo_Wark_Attack_To_Guard_Start,  // em5501_A-walk_start_L0 : 서있다가 방어자세 취하면서 걷기 시작
	CavaliereAngelo_Wark_Guard_Start,            // em5501_guard_to_walk : 방어자세 걷기 시작
	CavaliereAngelo_Wark_Guard_Loop,             // em5501_A-walk_loop : 방어자세 걷기 루프
	CavaliereAngelo_Wark_Guard_End,              // em5501_A-walk_End_L0 : 방어자세 걷기 끝

	CavaliereAngelo_Attack_Dengeki_Start,	     // em5501_Attack_Dengeki_Start : Dengeki 걷기 시작
	CavaliereAngelo_Attack_Dengeki_Loop,		 // em5501_Attack_Dengeki_Loop : Dengeki 걷기 루프
	CavaliereAngelo_Attack_Dengeki_End,			 // em5501_Attack_Dengeki_End : Dengeki 걷기 끝

	CavaliereAngelo_Turn_Attack_To_Guard_Left,   // em5501_Turn_Attack_to_guard_L : 공격 도중 왼쪽으로 180도 회전
	CavaliereAngelo_Turn_Attack_To_Guard_Right,  // em5501_Turn_Attack_to_guard_R : 공격 도중 오른쪽으로 180도 회전
	CavaliereAngelo_Guard_Quick_Turn_Left,       // em5501_guard_quick_turn_L : 방어자세 도중 왼쪽으로 180도 회전
	CavaliereAngelo_Guard_Quick_Turn_Right,      // em5501_guard_quick_turn_R : 방어자세 도중 오른쪽으로 180도 회전

	CavaliereAngelo_Attack_Guard_Turn_Right,     // em5501_Attack_guard_turn_R : 방어자세 도중 오른쪽으로 180도 회전하며 공격

	// <attack> ==================================================================================
	CavaliereAngelo_Attack_Collider,             // em5501_Attack_collider : 공격 도중 칼 위로 들어서 빠른 충전
	CavaliereAngelo_Attack_Collider_To_Dengeki,  // em5501_Attack_collider_to_Dengeki : 충전 완료 후 Dengeki 걷기 시작
	CavaliereAngelo_Attack_Collider_To_Rakurai,  // em5501_Attack_collider_to_Rakurai : 충전 완료 후 Rakurai 실시, 이후 방어자세 걷기 시작
												 
	CavaliereAngelo_Attack_Rakurai,				 // em5501_Attack_Rakurai : 방어자세 도중 Rakurai 실시, 이후 방어자세 걷기 시작
	CavaliereAngelo_Attack_Rakurai_to_01,		 // em5501_Attack_Rakurai_to_01 : Rakurai 후 바로 01(오중단) 공격 시작 전
	CavaliereAngelo_Attack_Rakurai_to_stinger,   // em5501_Attack_Rakurai_to_stinger : Rakurai 후 바로 stinger(왼중단-크게휘두르기) 공격 시작 전
												 
	CavaliereAngelo_Attack_Stinger01,			 // em5501_Attack_stinger01 : 방어자세 걷는 도중 바로 stinger(오중-크게휘두르기) 공격 시작 전
	CavaliereAngelo_Attack_Stinger02,			 // em5501_Attack_stinger02 : stinger 실시, 이후 방어자세 걷기 시작 end 애니메이션으로 변경

	CavaliereAngelo_Attack01,					 // em5501_Attack01_R : 01(오중단) 공격, 끝나면 방어자세, 66프레임 on, 69프레임 off
	CavaliereAngelo_Attack02,					 // em5501_Attack02 : 02(오상단) 공격, 끝나면 방어자세, 44프레임 on, 47프레임 off
	CavaliereAngelo_Attack03,					 // em5501_Attack03 : 03(왼중단) 공격, 끝나면 방어자세, 29프레임 on, 33프레임 off
	CavaliereAngelo_Attack04,					 // em5501_Attack04 : 04(양손으로 상단찍기) 공격, 끝나면 방어자세, 90프레임 on, 94프레임 off
	CavaliereAngelo_Attack04_C1,				 // em5501_Attack04_C1 : C1(위로 올려치기) 공격, 끝나면 방어자세
	CavaliereAngelo_Attack05,					 // em5501_Attack05 : 05(찌르기)끝난 후 방어자세로 전환(역패리 후 찌르기 공격 성공하면 취하는 자세)

	CavaliereAngelo_Parry_Even01,				 // em5501_Parry_even01 : 플레이어 패리 성공 후 바로 공격
	CavaliereAngelo_Parry_Even01_To_02,			 // em5501_Parry_even01_to_02 : 01 패리 후 바로 02 시작 전, 37프레임 스타트 (38 프레임으로 변경)
	CavaliereAngelo_Parry_Even01_To_03,			 // em5501_Parry_even01_to_04 : 01 패리 후 바로 03 시작 전, 24 프레임 스타트 (23 프레임으로 변경)
	CavaliereAngelo_Parry_normal01,				 // em5501_Parry_normal01 : 플레이어 패리 후 성공 후 약경직 
	CavaliereAngelo_Parry_normal01_to_02,		 // em5501_Parry_normal01_to_02 : 01 패리 후 경직, 이후 02 시작 전, 37프레임 스타트 (38 프레임으로 변경)
	CavaliereAngelo_Parry_normal01_to_03,		 // em5501_Parry_normal01_to_03 : 01 패리 후 경직, 이후 03 시작 전, 24 프레임 스타트 (23 프레임으로 변경)
	CavaliereAngelo_Parry_normal01_to_C1,		 // em5501_Parry_normal01_to_C1 : 01 패리 후 경직, 이후 C1 시작 전, 62 프레임 스타트

	// <damage> ==================================================================================
	CavaliereAngelo_Dengeki_Reload_Damage,       // em5501_dengeki_reload_damage : 번개 충전 중 공격 맞을 때 모션
	CavaliereAngelo_Dengeki_Reload_Damage_Left,  // em5501_dengeki_reload_damage_L : 번개 충전 중 왼쪽 공격 맞을 때 모션
	CavaliereAngelo_Dengeki_Reload_Damage_Right, // em5501_dengeki_reload_damage_R : 번개 충전 중 오른쪽 공격 맞을 때 모션

	CavaliereAngelo_Damage_Front,				 // em5501_damage_F : 정면공격 맞고 스턴, 이후 자세 회복
	CavaliereAngelo_Damage_Back,				 // em5501_damage_B : 후면공격 맞고 스턴, 이후 뒤로 돌아 자세 회복
	CavaliereAngelo_Damage_Drill,				 // em5501_Damage_Drill : 정면공격(드릴) 맞고 스턴 (스턴이 조금 더 김), 이후 자세 회복

	CavaliereAngelo_Guard_Front,			     // em5501_guard_F : 방어자세 서있는 상태에서 정면 공격 피격
	CavaliereAngelo_Guard_Left,				     // em5501_guard_L : 방어자세 서있는 상태에서 좌측 공격 피격
	CavaliereAngelo_Guard_Right,			     // em5501_guard_R : 방어자세 서있는 상태에서 우측 공격 피격
	CavaliereAngelo_Daeth_Front,				 // em5501_daeth_front : 정면 공격 맞고 사망

	CavaliereAngelo_Stun_Start,				     // em5501_stun_start : 데미지 중첩 스턴 시작
	CavaliereAngelo_Stun_Hukki,				     // em5501_stun_hukki : 데미지 중첩 스턴 끝

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

	void HPSeverStackPlus(int _Value)
	{
		HPServerStack += _Value;
	}

	void HPClientStackPlus(int _Value)
	{
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
	void DeathCheck() override;

private:
	std::shared_ptr<class GameEngineCollision> ParryCollision = nullptr;
	std::shared_ptr<class FXSystem> EffectRenderer_0 = nullptr;
	std::shared_ptr<class FXSystem> EffectRenderer_1 = nullptr;
	std::shared_ptr<class Cavaliere_Electric> Electric = nullptr;

	void PlayerChase();
	void ChangeState(int _StateValue);
	void ChangeState_Client(int _StateValue);
	void AttackCalculation();
	void ParryCheck();
	void ParryTime();
	void BossTurn();
	void PlayDamageSound(DamageSoundType _Type);

	float WaitTime = 0.0f;
	float RotationDelayTime = 0.0f;
	float FallCheckDelayTime = 0.0f;
	float AttackDelayCheck = 0.0f;
	float AttackDelayTime = 0.0f;
	float ElectroTime = 0.0f;

	short ParryStack = 0;
	short ColliderStack = 0;

	int ChargeDamageStack = 0;
	int HPServerStack = 0;
	int HPClientStack = 0;

	bool IsVergilLight = false;
	bool IsSturn = false;
	bool IsRecognize = false;
	bool IsParryCheck = false;    // 패리 체크 on off
	bool ParryOkay = false;
	bool IsStun = false; // 기절상태 (버스터 가능)
	bool Event01 = false;
	bool Normal01 = false;
	bool IsCharge = false;
	bool IsPowerUp = false;
	bool IsFastCharge = false;

};