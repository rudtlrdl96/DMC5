#pragma once
#include "Poolable.h"

enum FSM_State_HellCaina
{
	// 등장
	HellCaina_Appear_02,      // em0000_appear_02, 등장

	// 기본
	HellCaina_Idle,           // em0000_Idle_01, 그냥 서있음
	HellCaina_Menace_Front,   // em0000_menace_01, 앞쪽으로 위협
	HellCaina_Menace_Up,      // em0000_menace_02, 위로 함성 위협

	// 움직임
	HellCaina_Walk_Start,       // em0000_walk_start
	HellCaina_Walk_Loop,        // em0000_walk_loop
	HellCaina_Walk_Stop,        // em0000_walk_stop
	HellCaina_Walk_Right_Start, // em0000_walk_right_start
	HellCaina_Walk_Right_Loop,  // em0000_walk_right_loop
	HellCaina_Walk_Right_Stop,  // em0000_walk_right_stop
	HellCaina_Walk_Left_Start,  // em0000_walk_left_start
	HellCaina_Walk_Left_Loop,   // em0000_walk_left_loop
	HellCaina_Walk_Left_Stop,   // em0000_walk_left_stop

	// 회전
	HellCaina_Navi_Turn_Left,  // em0000_navi_turn_left_90, 왼쪽 슬러프 회전
	HellCaina_Navi_Turn_Right, // em0000_navi_turn_right_90, 오른쪽 슬러프 회전
	HellCaina_Turn_Left_180,   // em0000_turn_left_180, 왼쪽 회전
	HellCaina_Turn_Right_180,  // em0000_turn_right_180, 오른쪽 회전

	// 공격
	HellCaina_Attack_DownUp,  // em0000_attack_01, 아래에서 위로 베기, 70 on, 74 off
	HellCaina_Attack_UpDown,  // em0000_attack_02, 위에서 아래로 베기, 79 on, 83 off
	HellCaina_Attack_Turn,    // em0000_attack_01_turn, 뒤로 돌아서 공격, 68 on, 72 off
	HellCaina_Attack_Dash,    // em0000_attack_atackhard, 돌진베기, 110 start, 133 on, 137 off

	// 피격류
	HellCaina_Standing_Damage_Weak_Front, // em0000_standing_damage_weak_front_01, 정면 약공격 히트
	HellCaina_Standing_Damage_Weak_Back,  // em0000_standing_damage_weak_back_01, 후면 약공격 히트
	HellCaina_Standing_Damage_Weak_Left,  // em0000_standing_damage_weak_left_01, 좌측 약공격 히트
	HellCaina_Standing_Damage_Weak_Right, // em0000_standing_damage_weak_right_01, 우측 약공격 히트

	HellCaina_Blown_Up,					  // em0000_blown_up, 띄우기 피격 start
	HellCaina_Air_Damage_Under,			  // em0000_air_damage_under, 띄우기 피격 후 약공 맞을 때 모션

	HellCaina_Blown_Back,				  // em0000_blown_back, 강공격 피격 시 날아가기 start
	HellCaina_Blown_Back_Loop,			  // em0000_blown_back_loop, 강공격 피격 후 땅에 닿기 전 Loop
	HellCaina_Blown_Back_Landing,		  // em0000_blown_back_landing, 강공격 피격 후 날아가다 땅에 부딫힘

	HellCaina_Slam_Damage,				  // em0000_slam_damage, 슬램 피격 시 start
	HellCaina_Slam_Damage_Loop,			  // em0000_slam_damage_loop, 슬램 피격 공중 loop
	HellCaina_Slam_Damage_Landing,		  // em0000_slam_damage_landing, 슬램 피격 후 땅에 부딫힘

	HellCaina_Snatch,					  // em0000_snatch, 스내치 start

	HellCaina_Stun,						  // em0000_stun, 스턴 start
	HellCaina_Stun_Revive,				  // em0000_stun_revive, 스턴 상태에서 회복

	HellCaina_Death_Front,				  // em0000_death_front, 뒤로 쓰러지면서 죽음
	HellCaina_Death_Back,				  // em0000_death_back, 앞으로 쓰러지면서 죽음

	HellCaina_Prone_Death,				  // em0000_prone_death, 앞으로 누워있는 상태에서 Death
	HellCaina_Prone_Getup,				  // em0000_prone_getup, 앞으로 넘어진 상태에서 일어남

	HellCaina_Buster_Start,       // em0000_Buster_Start, 버스트 히트 시작
	HellCaina_Buster_Loop,        // 버스트 히트 루프
	HellCaina_Buster_Finish,      // em0000_Buster_Finish, 버스트 히트 땅에 떨어짐
};

// 설명 :
class Enemy_HellCaina : public BaseEnemyActor, public Poolable<Enemy_HellCaina>
{
public:
	// constrcuter destructer
	Enemy_HellCaina();
	~Enemy_HellCaina();

	// delete Function
	Enemy_HellCaina(const Enemy_HellCaina& _Other) = delete;
	Enemy_HellCaina(Enemy_HellCaina&& _Other) noexcept = delete;
	Enemy_HellCaina& operator=(const Enemy_HellCaina& _Other) = delete;
	Enemy_HellCaina& operator=(Enemy_HellCaina&& _Other) noexcept = delete;

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
	void ChangeState_Client(int _StateValue, NetworkObjectBase* _Obj);
	void AttackCalculation();
	void MoveLoop();

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
	bool DashAttackSetting = false;

};

