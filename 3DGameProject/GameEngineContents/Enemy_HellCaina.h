#pragma once
#include "EnemyActor_Normal.h"

enum FSM_State_HellCaina
{
	HellCaina_None,
	HellCaina_Idle,           // em0000_Idle_01, 그냥 서있음

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

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;

private:
	bool CheckBool = false;

	void ChangeState(int _StateValue);	// FSM 변경 함수
	bool FloorCheck(float _Distance); // 바닥체크

	float WaitTime = 0.0f;
	float FallCheckDelayTime = 0.0f;
};

