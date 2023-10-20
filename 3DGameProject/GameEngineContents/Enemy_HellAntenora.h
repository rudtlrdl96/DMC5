#pragma once

enum FSM_State_HellAntenora
{
	HellAntenora_Idle,                       // em0001_Idle_01_loop, 서있음



	HellAntenora_Counter_Attack,			 // em0001_Attack_counter_attack

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
class Enemy_HellAntenora : public BaseEnemyActor
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
	void DeathCheck() override;

private:
	void PlayerChase(float _DeltaTime);
	void PlayerAttack(float _DeltaTime);
	void RandomAttack();
	void ChangeState(int _StateValue);
	void ChangeState_Client(int _StateValue);
	void AttackCalculation();
	void MonsterAttackCollisionOn();
	void MonsterAttackCollisionOff();

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
};

