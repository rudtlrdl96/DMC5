#pragma once

#include "Poolable.h"

enum FSM_State_ProtoAngelo
{
	ProtoAngelo_Appear_01,							 // em0600_Appear_01, 등장
	ProtoAngelo_Idle,								 // em0601_Idle_Loop, 서있음

	// <move> ======================================================================================== =
	ProtoAngelo_Step_Back,							 // em0601_Step_Back : 뒤로 빽스텝

	ProtoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_L, // em0601_Turn_Attack_To_Quard_L : 오른쪽 돌기(이름 잘못 저장), 23프레임 애니메이션 턴
	ProtoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_R, // em0601_Turn_Attack_To_Quard_R : 왼쪽 돌기(이름 잘못 저장), 20프레임 애니메이션 턴 // 10프레임 전진 시작,  64프레임 전진 종료

	ProtoAngelo_Walk_Front_Start,					 // em0601_A_Walk_Start_L0 : 앞으로 걷기 시작, 18프레임 걷기 시작
	ProtoAngelo_Walk_Front_Loop,					 // em0601_A_Walk_Loop : 앞으로 걷기 루프
	ProtoAngelo_Walk_Front_Stop,					 // em0601_A_Walk_End_L0 : 앞으로 걷기 끝, 20프레임 걷기 끝

	// <attck> ======================================================================================== =
	ProtoAngelo_Attack_01_Turn_L,					 // em0601_Attack_01_Turn_L : 왼쪽으로 180도 돌면서 내려찍기(04) 공격 시작 전, 10프레임 애니메이션 턴
	ProtoAngelo_Attack_01_Turn_R,					 // em0601_Attack_01_Turn_R : 오른쪽으로 180도 돌면서 발도(03) 공격 시작 전, 40프레임 애니메이션 턴
	ProtoAngelo_Attack_02,							 // em0601_Attack_02 : 오른쪽 위에서 대각선 아래로 공격(02) 후 걷기 시작, 35프레임 세팅 시 오른쪽 대각선 아래 공격(02)으로 전환 가능, 90프레임 세팅 시 발도(03)로 전환 // 43프레임 on 48프레임 off // 141프레임 걷기 시작
	ProtoAngelo_Attack_03,							 // em0601_Attack_03 : 발도, 38프레임 세팅 시 내려찍기(04)로 전환 가능 // 28프레임 on 33프레임 off // 115프레임 전진, 165프레임(End) 끝 // 
	ProtoAngelo_Attack_04,							 // em0601_Attack_04 : 발도 후 내려찍기, 53프레임 세팅 시 내려찍기 공격(04)으로 전환 가능 // 88프레임 on 93프레임 off // 165프레임 전진, 227프레임(End) 끝
	ProtoAngelo_Attack_Dash_Tatakituke_Start,		 // em0601_Attack_Dash_Tatakituke_Start : 자세 잡고 돌진 시작, Tatakituke는 오른쪽으로 검을 들고있음 // 100프레임 돌진 시작
	ProtoAngelo_Attack_Dash_Tatakituke_Loop,		 // em0601_Attack_Dash_Tatakituke_Loop : 타타키 돌진 루프
	ProtoAngelo_Attack_Dash_Tatakituke,				 // em0601_Attack_Dash_Tatakituke : 돌진 후 발견 시 공격 // 10프레임 공격 모션 시작 // 40프레임 멈춤 // 51프레임 on 56프레임 off // 56프레임 전진 시작 81프레임 전진 끝 // 134프레임 후퇴 시작 158프레임 후퇴 끝
	ProtoAngelo_Attack_Kesa,						 // em0601_Attack_Kesa : 앞으로 짧게 베기 // 30프레임 전진, 54프레임 전진 끝 // 60프레임 on, 65프레임 off // 169 프레임 후퇴 시작, 203프레임 후퇴 끝
	
	ProtoAngelo_Attack_Tossin_Parry_Start,			 // em0601_Attack_Tossin_Parry_Start : 자세잡고 돌진 시작, Tossin은 왼쪽으로 검을 들고있음 // 60프레임 돌진 시작
	ProtoAngelo_Attack_03_To_Tossin,				 // em0601_Attack_03_To_Tossin : 발도 공격 후 돌진 시작 전, Tossin은 왼쪽으로 검을 들고있음 // 65프레임 돌진 시작
	ProtoAngelo_Attack_Tossin_Parry_Yokonagi,		 // em0601_Attack_Tossin_Parry_Yokonagi : 돌진 중 발견 시 회전베기(발도, 03) // 45프레임 돌진 종료 // 64프레임 On 69프레임 Off
	ProtoAngelo_Attack_Tossin_Parry_Loop,			 // em0601_Attack_Tossin_Parry_Loop : 토신 돌진 루프
	ProtoAngelo_Attack_Tossin_Parry_End,			 // em0601_Attack_Tossin_Parry_End : 돌진 끝 // 45프레임 돌진 종료 // 85프레임 전진 시작, 151프레임 전진 종료
	ProtoAngelo_Attack_Tossin_Parry_End_Turn,		 // em0601_Attack_Tossin_Parry_End_Turn : 돌진 끝난 뒤 뒤로 돔 // 140프레임 돌진 종료 // 75프레임 애니메이션 턴 

	ProtoAngelo_Parry_Attack_Start,					 // em0601_Parry_Attack_Start : 서있다가 토신 자세로 변경 // 15프레임 후퇴 시작, 45프레임 후퇴 종료
	ProtoAngelo_Parry_Attack_End,					 // em0601_Parry_Attack_End : 토신 패리자세에서 서있는 자세로 변경 //  
	ProtoAngelo_Parry_Attack_Loop,					 // em0601_Parry_Attack_Loop : 토신 달리기 직전 자세 루프
	ProtoAngelo_Parry_Attack_To_Kesagiri,			 // em0601_Parry_Attack_To_Kesagiri : 토신 준비 자세에서 바로 04 공격, 04를 87프레임으로 세팅해야함
	ProtoAngelo_Parry_Hurimuki_Attack_L,			 // em0601_Parry_Hurimuki_Attack_L : 토신 준비 자세에서 뒤로 180도 돌면서 발도 시작, 03을 20프레임으로 바꿔야함

	// <damage> ======================================================================================== =	
	ProtoAngelo_Standing_Damage_Weak_Front,			 // em0601_Standing_Damage_Weak_Front : 앞에서 약공 맞음
	ProtoAngelo_Standing_Damage_Weak_Back,			 // em0601_Standing_Damage_Weak_Back : 뒤에서 약공 맞고 뒤돔, 65프레임 애니메이션 턴

	ProtoAngelo_Blown_Back,							 // em0601_Blown_Back : 헤비 어택 시작
	ProtoAngelo_Blown_Back_Loop,					 // em0601_Blown_Back_Loop : 헤비 어택 루프
	ProtoAngelo_Blown_Back_Landing,					 // em0601_Blown_Back_Landing : 헤비어택 끝나고 랜딩

	ProtoAngelo_Blown_Up,							 // em0601_Blown_Up : 에어 어택 시작
	ProtoAngelo_Blown_Up_Loop,						 // em0601_Blown_Up_Loop : 에어 어택 루프
	ProtoAngelo_Blown_Up_Landing,					 // em0601_Blown_Up_Landing : 에어 어택 끝나고 랜딩
	ProtoAngelo_Air_Damage,							 // em0601_Air_Damage : 공중에서 맞았을때

	ProtoAngelo_Death_Back,							 // em0601_Death_Back : 앞에서 맞고 죽음, 70 프레임 걷기 끝
	ProtoAngelo_Death_Front,						 // em0601_Death_Front : 뒤에서 맞고 죽음, 70 프레임 걷기 끝

	ProtoAngelo_Lie_Death,							 // em0601_Lie_Death : 뒤에 쓰러진 상태로 죽음
	ProtoAngelo_Prone_Death,						 // em0601_Prone_Death : 앞으로 쓰러진 상태로 죽음
	ProtoAngelo_Prone_Getup,						 // em0601_Prone_Getup : 앞으로 쓰러져있다가 일어남, 120프레임 뒷걸음 시작, 170프레임 종료

	ProtoAngelo_Parry_Lose_Modori,					 // em0601_Parry_Lose_Modori : 패리 성공 모션

	ProtoAngelo_Slam_Damage,						 // em0601_Slam_Damage : 슬램 시작
	ProtoAngelo_Slam_Damage_Loop,					 // em0601_Slam_Damage_Loop : 슬램 루프
	ProtoAngelo_Slam_Damage_Landing,				 // em0601_Slam_Damage_Landing : 슬램 끝나고 랜딩

	ProtoAngelo_Snatch,								 // em0601_Snatch : 스내치 시작
	ProtoAngelo_Snatch_End,							 // em0601_Snatch_End : 스내치 끝

	ProtoAngelo_Buster_Start,						 // em0601_Air_Buster : 버스터 시작
	ProtoAngelo_Buster_Loop,						 // 버스터 루프
	ProtoAngelo_Buster_Finish,

	ProtoAngelo_Stun,								 // em0601_Stun : 스턴 시작
	ProtoAngelo_Stun_Revive,						 // em0601_Stun_Revive : 스턴 회복

};

// 설명 :
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
	void RandomAttack();
	void ChangeState(int _StateValue);
	void ChangeState_Client(int _StateValue);
	void AttackCalculation();
	void MonsterAttackCollisionOn();
	void MonsterAttackCollisionOff();
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

	bool IsHeavyAttack = false;   // 강공격 히트
	bool IsAirAttack = false;     // 에어공격 히트
	bool IsSlamAttack = false;    // 슬램공격 히트
	bool IsBusterAttack = false;  // 버스터 히트
	bool IsVergilLight = false;
	bool IsCollapse = false;      // 쓰러져있는 상태
	bool IsRecognize = false;
	bool AttackTypeChange = false;
	bool IsParryCheck = false;    // 패리 체크 on off
	bool ParryOkay = false;
	bool ParryEvent = false;
	bool IsSuperArmor = false;

};

