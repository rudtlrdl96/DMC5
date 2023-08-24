#pragma once
#include "BasePlayerActor.h"
#include <GameEngineCore/GameEngineFSM.h>

enum FSM_State_Nero
{
	None,
	Idle,
	Walk,
	Run,


	RQ_ComboA_1,
	RQ_ComboA_2,
	RQ_ComboA_3,
	RQ_ComboA_4,

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

	void ChangeFSM(int _State)
	{
		FSM.ChangeState(_State);
	}

	void InputCheckOn()
	{
		InputCheck = true;
	}

protected:
	void Start() override;
	void Update_Character(float _DeltaTime) override;

private:
	GameEngineFSM FSM;
	float MoveSpeed = 500.0f;

	bool InputCheck = false;
};

