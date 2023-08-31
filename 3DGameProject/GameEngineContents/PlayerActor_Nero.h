#pragma once
#include "BasePlayerActor.h"
#include <GameEngineCore/GameEngineFSM.h>

enum FSM_State_Nero
{
	Nero_None,
	Nero_Idle,
	Nero_RunStart,
	Nero_Run,
	Nero_RunStop,
	Nero_Dash,
	Nero_DashStop,
	Nero_Jump_Vertical,
	Nero_Jump_Fly,
	Nero_Landing,
	Nero_Evade_Left,
	Nero_Evade_Right,

	Nero_RQ_ComboA_1,
	Nero_RQ_ComboA_2,
	Nero_RQ_ComboA_3,
	Nero_RQ_ComboA_4,




	Nero_BR_Switch_Idle_to_Lockon,	// 기본자세에서 락온으로
	Nero_BR_Switch_Lockon_to_Idle,	// 락온에서 기본자세로
	Nero_BR_Lockon_Front,			// 정면 락온
	Nero_BR_Strafe,		// 락온하며 움직임
};

class PlayerActor_Nero : public BasePlayerActor
{
public:
	// constrcuter destructer
	PlayerActor_Nero();
	~PlayerActor_Nero();

	// delete Function
	PlayerActor_Nero(const PlayerActor_Nero& _Other) = delete;
	PlayerActor_Nero(PlayerActor_Nero&& _Other) noexcept = delete;
	PlayerActor_Nero& operator=(const PlayerActor_Nero& _Other) = delete;
	PlayerActor_Nero& operator=(PlayerActor_Nero&& _Other) noexcept = delete;

protected:
	void Start() override;
	void TestLoad();
	void NeroLoad();
	void Update_Character(float _DeltaTime) override;

private:
	GameEngineFSM FSM;
	float WalkSpeed = 300;
	float RunSpeed = 450;
	float DashSpeed = 800;
	float DashTimer = 0;
	float JumpForce = 7000.0f;
	bool InputCheck = false;
	bool MoveCheck = false;
	char CurDir = 'n';

	void RedQueenOn();
	void RedQueenOff();
	void BlueRoseOn();
	void BlueRoseOff();
	void WeaponIdle();
};

