#pragma once
#include "BasePlayerActor.h"
#include <GameEngineCore/GameEngineFSM.h>

enum FSM_State_Vergil
{
	Vergil_None,
	Vergil_Idle,
	Vergil_IdleLockOn,
	Vergil_Walk,
	Vergil_RunStart,
	Vergil_Run,
	Vergil_RunStop,
	Vergil_Jump_Vertical,
	Vergil_Jump_Fly,
	Vergil_Landing,
	Vergil_Evade_Left,
	Vergil_Evade_Right,

	Vergil_yamato_JudgementCutEnd_1,
	Vergil_yamato_JudgementCutEnd_2,

};

class PlayerActor_Vergil : public BasePlayerActor
{
public:
	// constrcuter destructer
	PlayerActor_Vergil();
	~PlayerActor_Vergil();

	// delete Function
	PlayerActor_Vergil(const PlayerActor_Vergil& _Other) = delete;
	PlayerActor_Vergil(PlayerActor_Vergil&& _Other) noexcept = delete;
	PlayerActor_Vergil& operator=(const PlayerActor_Vergil& _Other) = delete;
	PlayerActor_Vergil& operator=(PlayerActor_Vergil&& _Other) noexcept = delete;

protected:
	void Start() override;
	void VergilLoad();
	void Update_Character(float _DeltaTime) override;

private:
	GameEngineFSM FSM;
	float WalkSpeed = 300;
	float RunSpeed = 450;
	float JumpForce = 7000.0f;
	bool InputCheck = false;
	char CurDir = 'n';

	void SetHuman();
	void SetMajin();
	void YamatoOff();
	void YamatoOn();

};

