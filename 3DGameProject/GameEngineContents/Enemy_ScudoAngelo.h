#pragma once

enum FSM_State_ScudoAngelo
{
	// 등장
	ScudoAngelo_Appear_01,							 // em0600_Appear_01, 서있음
	ScudoAngelo_Response_Stay,						 // em0600_Response_Stay : 리스폰 후 가만히 있는 자세, 검 방 앞으로 했다가 서있는 자세로 전환
													 
	// 서있음										   
	ScudoAngelo_Idle,								 // em0001_Idle_01_loop, 서있음
	ScudoAngelo_Attack_Idle_Loop,					 // em0600_Attack_Idle_Loop : 공격 자세로 서있음

	// <move> ======================================================================================== =
	ScudoAngelo_Run_Front_Start,					 // em0600_Run_Front_Start : 앞으로 달리기 시작, 20프레임 걷기 시작
	ScudoAngelo_Run_Front_Loop,						 // em0600_Run_Front_Loop : 앞으로 달리기 루프
	ScudoAngelo_Run_Front_Stop,						 // em0600_Run_Front_Stop : 앞으로 달리기 끝, 걷기로 루프로 변경, 50프레임 걷기 시작
	ScudoAngelo_Run_Front_Stop_Turn_L,				 // em0600_Run_Front_Stop_Turn_L : 달리다가 왼쪽으로 돌아서 오른쪽 걷기 루프로 변경, 70프레임 걷기 시작
	ScudoAngelo_Run_Front_Stop_Turn_R,				 // em0600_Run_Front_Stop_Turn_R : 달리다가 오른쪽으로 돌아서 왼쪽 걷기 루프로 변경, 65프레임 걷기 시작
	ScudoAngelo_Shiedl_QuickWalk_Start,				 // em0600_Shiedl_QuickWalk_Start : 방패 전방 걷기 시작, 50프레임 걷기 시작
	ScudoAngelo_Shiedl_QuickWalk_Loop,				 // em0600_Shiedl_QuickWalk_Loop : 방패 전방 걷기 루프
	ScudoAngelo_Shiedl_QuickWalk_Stop_A,			 // em0600_Shiedl_QuickWalk_Stop_A : 방패 전방 걷기 끝, 40 프레임 뛰기 끝, 120프레임 걷기 끝
	ScudoAngelo_Shiedl_QuickWalk_Stop_B,			 // em0600_Shiedl_QuickWalk_Stop_B : 방패 전방 걷기 끝, 40 프레임 애니메이션 턴, 180도 회전, 60프레임 걷기 끝
	ScudoAngelo_Step_Back_M,						 // em0600_Step_Back_M : 뒤로 빽스텝 M
	ScudoAngelo_Step_Back_S,						 // em0600_Step_Back_S : 뒤로 빽스텝 S
	ScudoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_L, // em0600_Turn_Attack_To_Guard_L_Dummy_Turn_L : 오른쪽 돌기(이름 잘못 저장)
	ScudoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_R, // em0600_Turn_Attack_To_Guard_L_Dummy_Turn_R : 왼쪽 돌기(이름 잘못 저장)
	ScudoAngelo_Walk_Back_Start,					 // em0600_Walk_Back_Start : 뒤로 걷기 시작
	ScudoAngelo_Walk_Back_Loop,						 // em0600_Walk_Back_Loop : 뒤로 걷기 루프
	ScudoAngelo_Walk_Back_Stop,						 // em0600_Walk_Back_Stop : 뒤로 걷기 끝, 47프레임 걷기 끝
	ScudoAngelo_Walk_Front_Start,					 // em0600_Walk_Front_Start : 앞으로 걷기 시작, 20프레임 걷기 시작
	ScudoAngelo_Walk_Front_Loop,					 // em0600_Walk_Front_Loop : 앞으로 걷기 루프
	ScudoAngelo_Walk_Front_Stop,					 // em0600_Walk_Front_Stop : 앞으로 걷기 끝, 30프레임 걷기 끝
	ScudoAngelo_Walk_Left_Start,					 // em0600_Walk_Left_Start : 왼쪽 걷기 시작, 25프레임 걷기 시작
	ScudoAngelo_Walk_Left_Loop,						 // em0600_Walk_Left_Loop : 왼쪽 걷기 루프
	ScudoAngelo_Walk_Left_Stop,						 // em0600_Walk_Left_Stop : 왼쪽 걷기 끝, 100 프레임 걷기 끝
	ScudoAngelo_Walk_Right_Start,					 // em0600_Walk_Right_Start : 오른쪽 걷기 시작, 20프레임 걷기 시작
	ScudoAngelo_Walk_Right_Loop,					 // em0600_Walk_Right_Loop : 오른쪽 걷기 루프
	ScudoAngelo_Walk_Right_Stop,					 // em0600_Walk_Right_Stop : 오른쪽 걷기 끝, 80프레임 걷기 끝

	// <attck> ======================================================================================== =
	ScudoAngelo_Group_Command_Attack,				 // em0600_Group_Command_Attack : 공격 시작 모션
	ScudoAngelo_Group_Command_Attack_Loop,			 // em0600_Group_Command_Attack_Loop : 공격 시작 루프
	ScudoAngelo_Group_Command_Attack_To_Etc,		 // em0600_Group_Command_Attack_To_Etc : 공격 시작 돌진, 34프레임 뛰기 시작
	ScudoAngelo_Group_Command_Attack_End,			 // em0600_Group_Command_Attack_End : 공격 시작 앞으로 찌르기(안뛰고 바로 찌르기) // 10프레임 앞으로 이동 시작, 55프레임 앞으로 이동 끝 // 50프레임 on, 55프레임 off // 120 프레임 후퇴 시작, 220 프레임 후퇴 끝
													 
	ScudoAngelo_Attack_T_Hikkake,					 // em0600_Attack_T_Hikkake : 앞 공격 후 뒤로, 13프레임 전진, 45프레임 전진 끝 // 52프레임 on, 57프레임 off // 80 프레임 후퇴, 130 프레임 후퇴 끝
	ScudoAngelo_Attack_T_Kesa,						 // em0600_Attack_T_Kesa : 앞으로 공격, 43프레임 on, 48프레임 off
	ScudoAngelo_Attack_T_Kiriage,					 // em0600_Attack_T_Kiriage : 돌진 방패 올려치기, 53프레임 on, 58프레임 off // 103 프레임 전진 끝
													 
	ScudoAngelo_Attack_T_Run_Start,					 // em0600_Attack_T_Run_Start : 달리기 어택 시작
	ScudoAngelo_Attack_T_Run_Loop,					 // em0600_Attack_T_Run_Loop : 달리기 어택 루프
	ScudoAngelo_Attack_T_Run_Attack,				 // em0600_Attack_T_Run_Attack : 돌진 공격, 42프레임 on, 47프레임 off // 100프레임 전진 끝 // 110프레임 후퇴 시작, 180 프레임 후퇴 끝
	ScudoAngelo_Attack_T_Run_Stop_A,				 // em0600_Attack_T_Run_Stop_A : 달리기 어택 종료 // 50프레임 뛰기 끝, 걷기 시작, 100프레임 걷기 끝 // 120프레임 걷기 시작, 195프레임 걷기 끝
	ScudoAngelo_Attack_T_Run_Stop_B,				 // em0600_Attack_T_Run_Stop_B : 달리기 어택 종료하면서 뒤로 돔, 35프레임 애니메이션 턴 // 60프레임 뛰기 끝, 걷기 시작, 110 프레임 걷기 끝

	// <damage> ======================================================================================== =	
	ScudoAngelo_Standing_Damage_Weak_Front,			 // em0600_Standing_Damage_Weak_Front : 앞에서 약공 맞음
	ScudoAngelo_Standing_Damage_Weak_Back,			 // em0600_Standing_Damage_Weak_Back : 뒤에서 약공 맞고 뒤돔, 65프레임 애니메이션 턴
													 
	ScudoAngelo_Blown_Back,							 // em0600_Blown_Back : 헤비 어택 시작
	ScudoAngelo_Blown_Back_Loop,					 // em0600_Blown_Back_Loop : 헤비 어택 루프
	ScudoAngelo_Blown_Back_Landing,					 // em0600_Blown_Back_Landing : 헤비어택 끝나고 랜딩
													 
	ScudoAngelo_Blown_Up,							 // em0600_Blown_Up : 에어 어택 시작
	ScudoAngelo_Blown_Up_Loop,						 // em0600_Blown_Up_Loop : 에어 어택 루프
	ScudoAngelo_Blown_Up_Landing,					 // em0600_Blown_Up_Landing : 에어 어택 끝나고 랜딩
	ScudoAngelo_Air_Damage,							 // em0600_Air_Damage : 공중에서 맞았을때
													 
	ScudoAngelo_Death_Back,							 // em0600_Death_Back : 앞에서 맞고 죽음, 70 프레임 걷기 끝
	ScudoAngelo_Death_Front,						 // em0600_Death_Front : 뒤에서 맞고 죽음, 70 프레임 걷기 끝
													 
	ScudoAngelo_Lie_Death,							 // em0600_Lie_Death : 뒤에 쓰러진 상태로 죽음
	ScudoAngelo_Prone_Death,						 // em0600_Prone_Death : 앞으로 쓰러진 상태로 죽음
	ScudoAngelo_Prone_Getup,						 // em0600_Prone_Getup : 앞으로 쓰러져있다가 일어남
													 
	ScudoAngelo_Parry_Lose_Modori,					 // em0600_Parry_Lose_Modori : 패리 성공 모션
	ScudoAngelo_Attack_T_Tate_BouncedBack,			 // em0600_Attack_T_Tate_BouncedBack : 뒤로 밀려남, 95 프레임 끝
													 
	ScudoAngelo_Slam_Damage,						 // em0600_Slam_Damage : 슬램 시작
	ScudoAngelo_Slam_Damage_Loop,					 // em0600_Slam_Damage_Loop : 슬램 루프
	ScudoAngelo_Slam_Damage_Landing,				 // em0600_Slam_Damage_Landing : 슬램 끝나고 랜딩
													 
	ScudoAngelo_Snatch,								 // em0600_Snatch : 스내치 시작
	ScudoAngelo_Snatch_End,							 // em0600_Snatch_End : 스내치 끝
													 
	ScudoAngelo_Buster_Start,						 // em0600_Air_Buster : 버스터 시작
	ScudoAngelo_Buster_Loop,						 // 버스터 루프
	ScudoAngelo_Buster_Finish,
													 
	ScudoAngelo_Stun,								 // em0600_Stun : 스턴 시작
	ScudoAngelo_Stun_Revive,						 // em0600_Stun_Revive : 스턴 회복
};

// 설명 :
class Enemy_ScudoAngelo : public BaseEnemyActor
{
public:
	// constrcuter destructer
	Enemy_ScudoAngelo();
	~Enemy_ScudoAngelo();

	// delete Function
	Enemy_ScudoAngelo(const Enemy_ScudoAngelo& _Other) = delete;
	Enemy_ScudoAngelo(Enemy_ScudoAngelo&& _Other) noexcept = delete;
	Enemy_ScudoAngelo& operator=(const Enemy_ScudoAngelo& _Other) = delete;
	Enemy_ScudoAngelo& operator=(Enemy_ScudoAngelo&& _Other) noexcept = delete;

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

