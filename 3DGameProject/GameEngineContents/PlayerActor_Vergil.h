#pragma once
#include "BasePlayerActor.h"
#include <GameEngineCore/GameEngineFSM.h>

enum FSM_State_Vergil
{
	Vergil_None,
	Vergil_Idle,
	Vergil_IdleLockOn,
	Vergil_Walk,
	Vergil_WalkFront,
	Vergil_WalkLeft,
	Vergil_WalkBack,
	Vergil_WalkRight,
	Vergil_RunStart,
	Vergil_Run,
	Vergil_RunStop,
	Vergil_Jump_Vertical,
	Vergil_Jump_Fly,
	Vergil_Landing,

	Vergil_Warp_Left,
	Vergil_Warp_Right,

	Vergil_yamato_Combo_1,
	Vergil_yamato_Combo_2,
	Vergil_yamato_Combo_3,
	Vergil_yamato_Combo_4,
	Vergil_yamato_ComboC_1,
	Vergil_yamato_ComboC_2,
	Vergil_yamato_ComboC_3,

	Vergil_yamato_JudgementCutEnd_1,
	Vergil_yamato_JudgementCutEnd_2,

	Vergil_Demon_Start,
	Vergil_Demon_End,
};

class PlayerActor_Vergil : public BasePlayerActor
{
public:
	// constrcuter destructer
	PlayerActor_Vergil()
	{
		SetControll(NetControllType::UserControll);
	}
	~PlayerActor_Vergil();
	PlayerActor_Vergil(NetControllType _ControllType)
	{
		SetControll(_ControllType);
	}

	// delete Function
	PlayerActor_Vergil(const PlayerActor_Vergil& _Other) = delete;
	PlayerActor_Vergil(PlayerActor_Vergil&& _Other) noexcept = delete;
	PlayerActor_Vergil& operator=(const PlayerActor_Vergil& _Other) = delete;
	PlayerActor_Vergil& operator=(PlayerActor_Vergil&& _Other) noexcept = delete;

	void SetFSMStateValue(int _StateValue) override
	{
		FSM.ChangeState(_StateValue);
	}
	void SetDT(unsigned int _DTValue) override
	{

	}
protected:
	void Start() override;
	void VergilLoad();
	void NetLoad();
	void Update_Character(float _DeltaTime) override;

private:
	GameEngineFSM FSM;
	float WalkSpeed = 300;
	float RunSpeed = 550;
	float JumpForce = 7000.0f;

	bool InputCheck = false;	// 애니메이션 재생중 다른 입력을 받아 FSM변경이 가능한지 여부
	bool MoveCheck = false;		// 애니메이션 재생중 이동 입력을 받아 FSM변경이 가능한지 여부
	bool DelayCheck = false;	// 다른 콤보로 연결되기 위한 딜레이 여부
	bool IsDevilTrigger = false;

	void ChangeState(FSM_State_Vergil _StateValue);

	// 검 버튼을 입력한 경우 True 리턴 및 State 변경
	bool Input_SwordCheck(int AddState = 0);
	// 검 버튼을 공중에서 입력한 경우 True 리턴 및 State 변경
	bool Input_SwordCheckFly(int AddState = 0);
	// 총 버튼을 입력한 경우 True 리턴 및 State 변경
	bool Input_GunCheck();
	// 총 버튼을 공중에서 입력한 경우 True 리턴 및 State 변경
	bool Input_GunCheckFly();
	// 점프 버튼을 입력한 경우 True 리턴 및 State 변경
	bool Input_JumpCheck();
	// 점프 버튼을 공중에서 입력한 경우 True 리턴 및 State 변경
	bool Input_JumpCheckFly();
	// 워프 버튼을 입력한 경우 True 리턴 및 State 변경
	bool Input_WarpCheck();
	// 워프 버튼을 공중에서 입력한 경우 True 리턴 및 State 변경
	bool Input_WarpCheckFly();

	// 어떤 프레임이나 상관없이 캔슬하여 나가는 기술
	bool Input_SpecialCheck();
	bool Input_SpecialCheckFly();

	void SetHuman();
	void SetMajin();
	void YamatoOff();
	void YamatoOn();

};

