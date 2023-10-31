#pragma once

#include "Poolable.h"

enum FSM_State_HellAntenora
{
	HellAntenora_Idle,                       // em0001_Idle_01_loop, 서있음

	HellAntenora_Menace_01_Loop,			 // em0001_menace_01_loop : 두리번거림
	HellAntenora_Menace_02,					 // em0001_menace_02 : 칼 땅에 꽂았다가 뽑음

	HellAntenora_Appear_01,					 // em0001_appear_01 : 바닥에서 등장 1
	HellAntenora_Appear_02,					 // em0001_appear_02 : 바닥에서 등장 2
	HellAntenora_Appear_05,					 // em0001_appear_05 : 위에서 등장
	HellAntenora_Appear_30,					 // em0001_appear_30 : 벽에서  등장

	HellAntenora_fall_loop,					 // em0001_fall_loop : 위에서 등장 후 떨어질때
	HellAntenora_fall_landing,				 // em0001_fall_landing : 위에서 등장 후 랜딩

	HellAntenora_Navi_Turn_Left,			 // em0001_turn_left_90_noTransRot : 슬러프턴
	HellAntenora_Turn_Left_180,				 // em0001_turn_left_180 : 25프레임 애니메이션 턴
	HellAntenora_Navi_Turn_Right,			 // em0001_turn_right_90_noTransRot : 슬러프턴
	HellAntenora_Turn_Right_180,			 // em0001_turn_right_180 : 25프레임 애니메이션 턴

	HellAntenora_Walk_Start,				 // em0001_walk_start : 걷기 시작
	HellAntenora_Walk_Loop,					 // em0001_walk_loop : 걷기 루프
	HellAntenora_Walk_End,					 // em0001_walk_stop : 걷기 끝

	HellAntenora_Attack_Counter_JumpAttack_Start, // em0001_Attack_Counter_JumpAttack_Start : 점프어택 스타트, 66 on, 71 off // 안쓸듯
	HellAntenora_Attack_Counter_JumpAttack_End,   // em0001_Attack_Counter_JumpAttack_End : 점프어택 앤드                    // 안쓸듯

	HellAntenora_Attack_Counter_Start,			  // em0001_Attack_counter_Start : 돌진 전 뒤구르기
	HellAntenora_Attack_Counter_Landing,		  // em0001_Attack_counter_landing : 엎드렸다가 돌진
	HellAntenora_Attack_Counter_Loop,			  // em0001_Attack_counter_loop : 돌진 루프
	HellAntenora_Attack_Miss_Loop,                // em0001_Attack_counter_loop : y축 벗어나면 앞으로 돌진
	HellAntenora_Attack_Counter_Attack,			  // em0001_Attack_counter_attack : 앞으로 휘두르고 뒤로 돔, 38 on, 43 off / 55 프레임 애니메이션 턴
	HellAntenora_Attack_Counter_Stop,			  // em0001_Attack_counter_stop : 돌진 종료

	HellAntenora_Counter_Damage,				  // em0001_counter_s_damage : 약공 맞았을때 반격 곻격          // 안쓸듯
	HellAntenora_Counter_Short_Attack,			  // em0001_counter_short_attack : 비틀거리다가 일어나면서 공격

	HellAntenora_Multiattack_Start,					  // em0001_multiattack_Start : 오른쪽에서 왼쪽으로 휘두르기 공격 시작
	HellAntenora_Multiattack_SideAttack_to_Right,     // em0001_multiattack_SideAttack_to_R : 오른쪽에서 왼쪽으로 크게 휘두름, 22 R on, 27 R Off / 37 L on, 42 L Off
	HellAntenora_Multiattack_From_Right_End,		  // em0001_multiattack_from_R_End : 오른쪽에서 왼쪽으로 크게 휘두름 끝
	HellAntenora_Multiattack_To_Attack_End_Right,	  // em0001_multiattack_to_attack_End_R : 오른쪽에서 왼쪽으로 휘두른 뒤 위에서 찍는 연계공격, 73 on, 78 off
	HellAntenora_Multiattack_UpAttack_To_Right,		  // em0001_multiattack_UpAttack_to_R : 오른쪽에서 왼쪽으로 휘두른 뒤 다시 왼쪽 위로 휘두르는 연계공격, 60 R on, 65 R Off, 76 L On, 81 L Off

	HellAntenora_Multiattack_SideAttack_To_Left,	  // em0001_multiattack_SideAttack_to_L : 왼쪽에서 오른쪽으로 크게 휘두름, 22 L on, 27 L off / 37 R on, 42 off
	HellAntenora_Multiattack_From_Left_End,			  // em0001_multiattack_from_L_End : 왼쪽에서 오른쪽으로 크게 휘두름 끝
	HellAntenora_Multiattack_To_Attack_End_Left,	  // em0001_multiattack_to_attack_End_L : 왼쪽에서 오른쪽으로 휘두른 뒤 위에서 찍는 연계공격, 73 on, 78 off
	HellAntenora_Multiattack_UpAttack_To_Left,		  // em0001_multiattack_UpAttack_to_L : 왼쪽에서 오른쪽으로 휘두른 뒤 다시 오른쪽 위로 휘두르는 연계공격, 61 L on, 66 L Off, 76 R On, 81 R Off

	HellAntenora_Turn_Attack_Left,					  // em0001_turn_attack_L : 왼쪽 뒤로 돌면서 공격, 78 프레임 애니메이션 턴, 84 L on, 89 L Off
	HellAntenora_Turn_Attack_Right,					  // em0001_turn_attack_R : 오른쪽 뒤로 돌면서 공격, 78 프레임 애니메이션 턴, 85 L on, 90 L Off

	HellAntenora_Standing_Damage_Weak_Front, // em0001_standing_damage_weak_front_01 : 정면 약공 히트
	HellAntenora_Standing_Damage_Weak_Back,  // em0001_standing_damage_weak_back_01 : 뒤에서 약공 히트시, 105 프레임 애니메이션 턴
	HellAntenora_Standing_Damage_Weak_Left,  // em0001_standing_damage_weak_left_01 : 왼쪽 약공 히트, 115 프레임 애니메이션 턴
	HellAntenora_Standing_Damage_Weak_Right, // em0001_standing_damage_weak_right : 오른쪽 약공 히트, 100 프레임 애니메이션 턴

	HellAntenora_Blown_Back,				 // em0001_blown_back : 헤비어택 히트시
	HellAntenora_Blown_Back_Loop,			 // em0001_blown_back_loop : 헤비어택 루프    // 아직 안씀
	HellAntenora_Blown_Back_Landing,		 // em0001_blown_back_landing : 헤비어택 랜딩

	HellAntenora_Air_Damage_Under,			 // em0001_air_damage_gun : 에어어택 상태에서 히트시
	HellAntenora_Blown_Up,					 // em0001_blown_up : 에어어택 히트시
	HellAntenora_Blown_Up_Loop,				 // em0001_blown_up_loop : 에어어택 루프
	HellAntenora_Blown_Up_Landing,			 // em0001_blown_up_landing : 에어어택 착지 시 앉은 상태이지만 비틀거리다가 쓰러지는 랜딩

	HellAntenora_Slam_Damage,				 // em0001_slam_damage : 슬램 공격 히트시
	HellAntenora_Slam_Damage_Loop,			 // em0001_slam_damage_loop : 슬램 상태 루프
	HellAntenora_Slam_Damage_Landing,		 // em0001_slam_damage_landing : 슬램 공격 랜딩시

	HellAntenora_Snatch,					 // em0001_snatch : 스내치 히트시

	HellAntenora_Stun,						 // em0001_stun : 스턴 히트시
	HellAntenora_Stun_Revive,				 // em0001_stun_revive : 스턴 회복

	HellAntenora_Prone_Getup,				 //em0001_prone_getup : 앞으로 쓰러진 상태에서 일어남
	HellAntenora_Prone_Loop,				 //em0001_prone_loop : 앞으로 쓰러진 상태 루프

	HellAntenora_Death_Back,				 // em0001_death_back : 앞으로 쓰러지면서 죽음
	HellAntenora_Death_Front,				 // em0001_death_front : 뒤로 쓰러지면서 죽음
	HellAntenora_Prone_Death,				 // em0001_prone_death : 앞으로 쓰러진 상태에서 죽음
	HellAntenora_Lie_Death,					 // em0001_lie_death : 뒤로 쓰러진 상태에서 죽음

	HellAntenora_Buster_Start,				 // em0001_buster_loop : 버스터 히트
	HellAntenora_Buster_Loop,				 // 버스터 맞았을때 루프
	HellAntenora_Buster_Finish,              // 버스터 땅에 닿음

};

// 설명 :
class Enemy_HellAntenora : public BaseEnemyActor, public Poolable<Enemy_HellAntenora>
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

private:
	void PlayerChase();
	void ChangeState(int _StateValue);
	void ChangeState_Client(int _StateValue, NetworkObjectBase* _Obj);
	void AttackCalculation();
	void MonsterAttackCollisionOn();
	void MonsterAttackCollisionOff();
	void MoveLoop();

	float WaitTime = 0.0f;
	float RunTime = 0.0f;
	float RotationDelayTime = 0.0f;
	float FallCheckDelayTime = 0.0f;
	float AttackDelayCheck = 0.0f;
	float AttackDelayTime = 0.0f;

	short WalkCount = 0;
	short MultiAttackStack = 0;

	bool IsHeavyAttack = false;   // 강공격 히트
	bool IsAirAttack = false;     // 에어공격 히트
	bool IsSlamAttack = false;    // 슬램공격 히트
	bool IsBusterAttack = false;  // 버스터 히트
	bool IsVergilLight = false;
	bool IsCollapse = false;      // 쓰러져있는 상태
	bool IsRecognize = false;
	bool AttackTypeChange = false;
	bool DashAttackSetting = false;
	bool IsAirAttackSetting = false;
};

