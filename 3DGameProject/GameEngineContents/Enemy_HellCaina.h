#pragma once
#include "EnemyActor_Normal.h"

enum FSM_State_HellCaina
{
	HellCaina_None,
	HellCaina_Idle,           // em0000_Idle_01, 그냥 서있음

	HellCaina_Turn_Left_90,   // 왼쪽 회전
	HellCaina_Turn_Left_180,
	HellCaina_Turn_Right_90,  // 오른쪽 회전
	HellCaina_Turn_Right_180,

	HellCaina_Walk_Start,
	HellCaina_Walk_Loop,
	HellCaina_Walk_Stop,

	HellCaina_Standing_Damage_Weak_Front, // = 정면 약공격 히트
	HellCaina_Standing_Damage_Weak_Back, // = 후면 약공격 히트
	HellCaina_Standing_Damage_Weak_Left, // = 좌측 약공격 히트
	HellCaina_Standing_Damage_Weak_Right, // = 우측 약공격 히트

	//HellCaina_Blown_Front = 날아가면서 앞으로 고꾸라짐
	//HellCaina_Blown_Front_loop = 날아가면서 앞으로 고꾸라짐 루프
	HellCaina_Blown_Back, // = 날아가면서 뒤로 고꾸라짐
	HellCaina_Blown_Back_Under, // = 날아가면서 뒤로 고꾸라짐 최고점 꺾을때 애니메이션
	HellCaina_Blown_Back_Loop, //  = 날아가면서 뒤로 고꾸라짐 루프
	HellCaina_Blown_Back_Landing, // = 날아가면서 뒤로 고꾸라짐 랜딩

	HellCaina_Lie_Getup,  // 뒤로 엎어졌을 때 일어나는 모션
	HellCaina_Prone_Getup, // 앞으로 엎어졌을 때 일어나는 모션

	HellCaina_Menace_Front,   // em0000_menace_01, 앞쪽으로 위협
	HellCaina_Menace_Up,      // em0000_menace_02, 위로 함성 위협

	HellCaina_Attack_DownUp,      // 아래에서 위로 베기
	HellCaina_Attack_DownUp_step, // 아래에서 위로 베기 전 스텝
	HellCaina_Attack_UpDown,      // 위에서 아래로 베기
	HellCaina_Attack_Dash,        // 돌진베기

	HellCaina_Air_Buster,         // 공중에서 연속공격 맞을 때
	HellCaina_Buster_Start,       // 에어어택 스타트
	HellCaina_Buster_Finish,      // 에어에서 땅에 떨어졌을 때
	HellCaina_Gun_Buster,
	HellCaina_Gun_Buster_Start,
	HellCaina_Gun_Buster_Finish,
	HellCaina_M_Buster,
};

// 설명 :
class Enemy_HellCaina : public EnemyActor_Normal
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

	void SetFSMStateValue(int _StateValue) override
	{
		EnemyFSM.ChangeState(_StateValue);
	}

	void LightDamage();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;

private:
	bool CheckBool = false;
	bool AttackCheck = false;

	void ChangeState(int _StateValue);	// FSM 변경 함수
	bool FloorCheck(float _Distance); // 바닥체크
	void TurnToPlayer(float _DeltaTime);
	void DamageColCheck();

	float WaitTime = 0.0f;
	float FallCheckDelayTime = 0.0f;
};

