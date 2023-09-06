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
	Nero_RQ_ComboD_1,
	Nero_RQ_ComboD_2,
	Nero_RQ_ComboD_3,
	Nero_RQ_ComboD_4,

	Nero_RQ_AirComboA_1,
	Nero_RQ_AirComboA_2,
	Nero_RQ_AirComboA_3,
	Nero_RQ_AirComboB,

	// 하이룰러 (뛰우는 공격)
	Nero_RQ_Skill_HR,


	// 스피릿 (낙공)
	Nero_RQ_Skill_Split_1,
	Nero_RQ_Skill_Split_2,
	Nero_RQ_Skill_Split_3,

	Nero_RQ_Skill_Caliber_1,
	Nero_RQ_Skill_Caliber_2,



	Nero_BR_Switch_Idle_to_Lockon,	// 기본자세에서 락온으로
	Nero_BR_Switch_Lockon_to_Idle,	// 락온에서 기본자세로
	Nero_BR_Lockon_Front,			// 정면 락온
	Nero_BR_Strafe,		// 락온하며 움직임
	Nero_BR_Strafe_F,		// 락온하며 움직임
	Nero_BR_Strafe_FL,		// 락온하며 움직임
	Nero_BR_Strafe_L,		// 락온하며 움직임
	Nero_BR_Strafe_BL,		// 락온하며 움직임
	Nero_BR_Strafe_B,		// 락온하며 움직임
	Nero_BR_Strafe_BR,		// 락온하며 움직임
	Nero_BR_Strafe_R,		// 락온하며 움직임
	Nero_BR_Shoot,		// 사격
	Nero_BR_AirShoot,		// 공중사격
};

class PlayerActor_Nero : public BasePlayerActor
{
public:
	// constrcuter destructer
	PlayerActor_Nero()
	{
		SetControll(NetControllType::UserControll);
	}
	PlayerActor_Nero(NetControllType _ControllType)
	{
		SetControll(_ControllType);
	}
	~PlayerActor_Nero();

	// delete Function
	PlayerActor_Nero(const PlayerActor_Nero& _Other) = delete;
	PlayerActor_Nero(PlayerActor_Nero&& _Other) noexcept = delete;
	PlayerActor_Nero& operator=(const PlayerActor_Nero& _Other) = delete;
	PlayerActor_Nero& operator=(PlayerActor_Nero&& _Other) noexcept = delete;

	void SetFSMStateValue(int _StateValue) override
	{
		FSM.ChangeState(_StateValue);
	}

	void SinglePlayLoad();

protected:
	void Start() override;
	void PlayerLoad();
	void NetLoad();
	void Update_Character(float _DeltaTime) override;


private:
	GameEngineFSM FSM;
	float WalkSpeed = 300;
	float RunSpeed = 550;
	float DashSpeed = 700;
	float DashTimer = 0;
	float JumpForce = 7000.0f;

	bool InputCheck = false;	// 애니메이션 재생중 다른 입력을 받아 FSM변경이 가능한지 여부
	bool MoveCheck = false;		// 애니메이션 재생중 이동 입력을 받아 FSM변경이 가능한지 여부
	bool DelayCheck = false;	// 다른 콤보로 연결되기 위한 딜레이 여부

	bool LoadCheck = false;
	void ChangeState(FSM_State_Nero _StateValue);

	//NeRoUI;
	void RedQueenOn();
	void RedQueenOff();
	void BlueRoseOn();
	void WeaponIdle();
	void SetHuman();
	void SetDemon();
	void SetOverture();
};

