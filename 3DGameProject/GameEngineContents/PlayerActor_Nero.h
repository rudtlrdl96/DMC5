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




	BR_Switch_Idle_to_Lockon,	// �⺻�ڼ����� ��������
	BR_Switch_Lockon_to_Idle,	// ���¿��� �⺻�ڼ���
	BR_Lockon_Front,			// ���� ����
	BR_Strafe,		// �����ϸ� ������
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

	/*void ChangeFSM(int _State)
	{
		FSM.ChangeState(_State);
	}*/

	//void InputCheckOn()
	//{
	//	InputCheck = true;
	//}

	void SetPush(const float4& _Value);
	void SetSpeed(float _Speed)
	{
		WalkSpeed = _Speed;
	}


protected:
	void Start() override;
	void Update_Character(float _DeltaTime) override;

private:
	GameEngineFSM FSM;
	float WalkSpeed = 1500000.0f;
	float DashSpeed = 1500000.0f;

	bool InputCheck = false;
	char CurDir = 'n';

	void RedQueenOn();
	void RedQueenOff();
	void BlueRoseOn();
	void WeaponIdle();
};

