#pragma once

enum FSM_State_Empusa
{
	// 등장
	Empusa_Appear_02,      // em0000_appear_02, 등장
	Empusa_Appear_10,      // em0000_appear_10, 등장

	// 기본
	Empusa_Idle,           // em0000_Idle_01, 그냥 서있음
	Empusa_Menace_Front,   // em0000_menace_01, 앞쪽으로 위협
	Empusa_Menace_Up,      // em0000_menace_02, 위로 함성 위협

	// 움직임
	Empusa_Walk_Start,       // em0000_walk_start
	Empusa_Walk_Loop,        // em0000_walk_loop
	Empusa_Walk_Stop,        // em0000_walk_stop
	Empusa_Walk_Right_Start, // em0000_walk_right_start
	Empusa_Walk_Right_Loop,  // em0000_walk_right_loop
	Empusa_Walk_Right_Stop,  // em0000_walk_right_stop
	Empusa_Walk_Left_Start,  // em0000_walk_left_start
	Empusa_Walk_Left_Loop,   // em0000_walk_left_loop
	Empusa_Walk_Left_Stop,   // em0000_walk_left_stop

	// 회전
	Empusa_Navi_Turn_Left,  // em0000_navi_turn_left_90, 왼쪽 슬러프 회전
	Empusa_Navi_Turn_Right, // em0000_navi_turn_right_90, 오른쪽 슬러프 회전
	Empusa_Turn_Left_180,   // em0000_turn_left_180, 왼쪽 회전
	Empusa_Turn_Right_180,  // em0000_turn_right_180, 오른쪽 회전

	// 공격
	Empusa_Attack_DownUp,  // em0000_attack_01, 아래에서 위로 베기
	Empusa_Attack_UpDown,  // em0000_attack_02, 위에서 아래로 베기
	Empusa_Attack_Turn,    // em0000_attack_01_turn, 뒤로 돌아서 공격
	Empusa_Attack_Dash,    // em0000_attack_atackhard, 돌진베기

	// 피격류
	Empusa_Standing_Damage_Weak_Front, // em0000_standing_damage_weak_front_01, 정면 약공격 히트
	Empusa_Standing_Damage_Weak_Back,  // em0000_standing_damage_weak_back_01, 후면 약공격 히트
	Empusa_Standing_Damage_Weak_Left,  // em0000_standing_damage_weak_left_01, 좌측 약공격 히트
	Empusa_Standing_Damage_Weak_Right, // em0000_standing_damage_weak_right_01, 우측 약공격 히트

	Empusa_Transdamage_Front,		  // em0000_transdamage_front, 중공격 히트 
	Empusa_Transdamage_Back,			  // em0000_transdamage_back, 뒤에서 중공격 맞고 돌아봄

	Empusa_Blown_Up,					  // em0000_blown_up, 띄우기 피격 start
	Empusa_Air_Damage,				  // em0000_air_damage, 띄우기 피격 후 최고점에서 꺾이면서 내려올 때 애니메이션
	Empusa_Air_Damage_Under,			  // em0000_air_damage_under, 띄우기 피격 후 약공 맞을 때 모션

	Empusa_Blown_Back,				  // em0000_blown_back, 강공격 피격 시 날아가기 start
	Empusa_Blown_Back_Loop,			  // em0000_blown_back_loop, 강공격 피격 후 땅에 닿기 전 Loop
	Empusa_Blown_Back_Landing,		  // em0000_blown_back_landing, 강공격 피격 후 날아가다 땅에 부딫힘

	Empusa_Blown_Wall_Back,			  // em0000_blown_wall_back, 벽에 박음 start
	Empusa_Blown_Wall_Back_Landing,    // em0000_blown_wall_back_landing, 벽에 박고 땅에 부딫힐 때

	Empusa_Slam_Damage,				  // em0000_slam_damage, 슬램 피격 시 start
	Empusa_Slam_Damage_Loop,			  // em0000_slam_damage_loop, 슬램 피격 공중 loop
	Empusa_Slam_Damage_Landing,		  // em0000_slam_damage_landing, 슬램 피격 후 땅에 부딫힘

	Empusa_Snatch,					  // em0000_snatch, 스내치 start
	Empusa_Snatch_End,				  // em0000_snatch_end, 스내치 end

	Empusa_Stun,						  // em0000_stun, 스턴 start
	Empusa_Stun_Revive,				  // em0000_stun_revive, 스턴 상태에서 회복

	Empusa_Death_Front,				  // em0000_death_front, 뒤로 쓰러지면서 죽음
	Empusa_Lie_Damage_Gun,			  // em0000_lie_damage_gun, 뒤로 누워있는 상태에서 약공 맞을때
	Empusa_Lie_Death,				  // em0000_lie_death, 뒤로 넘어진 상태에서 Death
	Empusa_Lie_Down,					  // em0000_lie_down, 뒤로 넘어짐
	Empusa_Lie_Getup,				  // em0000_lie_getup, 뒤로 넘어진 상태에서 일어남
	Empusa_Death_Back,				  // em0000_death_back, 앞으로 쓰러지면서 죽음
	Empusa_Prone_Damage_Gun,			  // em0000_prone_damage_gun, 앞으로 누워있는 상태에서 약공 맞을때
	Empusa_Prone_Death,				  // em0000_prone_death, 앞으로 누워있는 상태에서 Death
	Empusa_Prone_Down,				  // em0000_prone_down, 앞으로 넘어짐
	Empusa_Prone_Getup,				  // em0000_prone_getup, 앞으로 넘어진 상태에서 일어남

	Empusa_Air_Buster,         // em0000_Air-Buster, 공중에서 버스트 히트
	Empusa_Buster,             // em0000_Buster, 버스트 연속동작 (히트부터 다운까지)
	Empusa_Buster_Start,       // em0000_Buster_Start, 버스트 히트 시작
	Empusa_Buster_Loop,        // 버스트 히트 루프
	Empusa_Buster_Finish,      // em0000_Buster_Finish, 버스트 히트 땅에 떨어짐
	Empusa_Gun_Buster,         // em0000_Gun_Buster, 총 버스트 히트, 뒤로 돌아서 무릎꿇음
	Empusa_Gun_Buster_Start,   // em0000_Gun_Buster_shoot, 총 버스트 히트 스타트(?)
	Empusa_Gun_Buster_Finish,  // em0000_Gun_Buster_end, 총 버스트 엔드
	Empusa_M_Buster,           // em0000_M-Buster, ?

};
class Enemy_Empusa : public BaseEnemyActor
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
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;
	void EnemyCreateFSM_Client() override;
	void DamageCollisionCheck(float _DeltaTime) override;
	void RecognizeCollisionCheck(float _DeltaTime) override;

private:
	void PlayerChase(float _DeltaTime);
	void PlayerAttack(float _DeltaTime);
	void RandomAttack();
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
};

