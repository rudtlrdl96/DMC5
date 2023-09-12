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

	bool InputCheck = false;	// �ִϸ��̼� ����� �ٸ� �Է��� �޾� FSM������ �������� ����
	bool MoveCheck = false;		// �ִϸ��̼� ����� �̵� �Է��� �޾� FSM������ �������� ����
	bool DelayCheck = false;	// �ٸ� �޺��� ����Ǳ� ���� ������ ����
	bool IsDevilTrigger = false;

	void ChangeState(FSM_State_Vergil _StateValue);

	void SetHuman();
	void SetMajin();
	void YamatoOff();
	void YamatoOn();

};

