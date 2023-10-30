#pragma once
#include "Poolable.h"
#include "BaseEnemyActor.h"

enum FSM_State_Empusa
{
	// 등장
	Empusa_Appear_A,      // em0100_enter_ground_A, 등장 A
	Empusa_Appear_B,      // em0100_enter_ground_B, 등장 B

	// 미어캣 자세
	Empusa_Biped_Idle, // em0100_biped_idle, 미어캣처럼 서있음

	// 돌격 자세
	Empusa_Quadruped_Idle, // em0100_quadruped_idle : 돌격자세
	Empusa_Quadruped_To_Biped, // em0100_quadruped_to_biped : 돌격자세->미어캣자세 변경

	// 움직임
	Empusa_Biped_Run_Start, // em0100_biped_run_start : 미어캣자세->돌격자세 변경되면서 움직임 시작
	Empusa_Biped_Run_Loop, // em0100_biped_run_loop : 미어캣자세->돌격자세 변경된 후 움직임 루프
	Empusa_Biped_Run_Stop, // em0100_biped_run_stop : 돌격자세->미어캣자세 변경되면서 움직임 종료
	Empusa_Biped_Walk_Start, // em0100_biped_walk_start : 미어캣 자세 움직임 시작
	Empusa_Biped_Walk_Loop, // em0100_biped_walk_loop : 미어캣 자세 움직임 루프
	Empusa_Biped_Walk_Stop, // em0100_biped_walk_stop : 미어캣 자세 움직임 끝
	Empusa_Step_Back, // em0100_step_back : 돌격자세 뒤로 한번 전진

	// 회전
	Empusa_Turn_Left_90, // em0100_turn_90_left : 미어캣 자세 왼쪽 90도 턴
	Empusa_Turn_Right_90, // em0100_turn_90_right : 미어캣 자세 오른쪽 90도 턴
	Empusa_Turn_Left_180, // em0100_turn_180_left : 미어캣 자세 왼쪽 180도 턴
	Empusa_Turn_Right_180, // em0100_turn_180_right : 미어캣 자세 오른쪽 180도 턴

	// 공격
	Empusa_Attack_A, // em0100_attack_A : 오른손 위에서 아래로 베기, 136 on, 142 off
	Empusa_Attack_B, // em0100_attack_B : 왼손 위에서 아래로 베기, 68 on, 73 off
	Empusa_Attack_C, // em0100_attack_C : 오른손 횡베기, 60 on, 64 off
	Empusa_Attack_D, // em0100_attack_D : 왼손 횡베기, 60 on, 65 off
	Empusa_Attack_W, // em0100_attack_W : 양손 내려찍기, 151 on, 155 off
	Empusa_Continuation_Attack, // em0100_continuation_attack : 연속공격(여러번 공격하면서 전진), 128/131, 161/163, 195/197, 230/233, 296/301

	// 피격류
	Empusa_Standing_Damage_Weak_Front, // em0100_angledamage_front, 정면 약공격 히트
	Empusa_Standing_Damage_Weak_Back,  // em0100_angledamage_back, 후면 약공격 히트
	Empusa_Standing_Damage_Weak_Left,  // em0100_angledamage_left, 좌측 약공격 히트
	Empusa_Standing_Damage_Weak_Right, // em0100_angledamage_right, 우측 약공격 히트

	Empusa_Air_Damage,				  // em0100_air_damage, 띄우기 피격 start
	Empusa_Air_Damage_Under,		  // em0100_air_damage_under, 띄우기 피격 후 약공 맞을 때 모션

	Empusa_Blown_Back,				  // em0100_blown_back_start, 강공격 피격 시 날아가기 start
	Empusa_Blown_Back_Landing,		  // em0100_blown_back_landing, 강공격 피격 후 날아가다 땅에 부딫힘

	Empusa_Slam_Damage,				  // em0100_slam_damage_start, 슬램 피격 시 start
	Empusa_Slam_Damage_Loop,		  // em0100_slam_damage_fall_loop, 슬램 피격 공중 loop
	Empusa_Slam_Damage_Landing,		  // em0100_slam_damage_landing, 슬램 피격 후 땅에 부딫힘

	Empusa_Snatch,					  // em0100_snatch_loop, 스내치 start
	Empusa_Snatch_End,				  // em0000_snatch_end, 스내치 end

	Empusa_Stun,					  // em0100_stun_loop, 스턴 start
	Empusa_Stun_Revive,				  // em0100_stun_revive, 스턴 상태에서 회복

	Empusa_Death_Front,				  // em0100_death_front, 뒤로 쓰러지면서 죽음
	Empusa_Death_Back,				  // em0100_death_back, 앞으로 쓰러지면서 죽음
	Empusa_Downward_Getup,			  // em0100_downward_down_standup, 앞으로 넘어진 상태에서 일어남
	Empusa_Downward_Death,			  // em0100_downward_die, 앞으로 누워있는 상태에서 Death

	Empusa_Buster_Start,       // em0000_Buster_Start, 버스트 히트 시작
	Empusa_Buster_Loop,        // 버스트 히트 루프
	Empusa_Buster_Finish,      // em0000_Buster_Finish, 버스트 히트 땅에 떨어짐

};

class Enemy_Empusa : public BaseEnemyActor, public Poolable<Enemy_Empusa>
{
public:
	// constrcuter destructer
	Enemy_Empusa();
	~Enemy_Empusa();

	// delete Function
	Enemy_Empusa(const Enemy_Empusa& _Other) = delete;
	Enemy_Empusa(Enemy_Empusa&& _Other) noexcept = delete;
	Enemy_Empusa& operator=(const Enemy_Empusa& _Other) = delete;
	Enemy_Empusa& operator=(Enemy_Empusa&& _Other) noexcept = delete;

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
	bool IsAppearA = false;
};

