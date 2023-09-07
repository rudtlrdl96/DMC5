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

	// ���̷귯 (�ٿ�� ����)
	Nero_RQ_Skill_HR,


	// ���Ǹ� (����)
	Nero_RQ_Skill_Split_1,
	Nero_RQ_Skill_Split_2,
	Nero_RQ_Skill_Split_3,

	Nero_RQ_Skill_Caliber_1,
	Nero_RQ_Skill_Caliber_2,



	Nero_BR_Switch_Idle_to_Lockon,	// �⺻�ڼ����� ��������
	Nero_BR_Switch_Lockon_to_Idle,	// ���¿��� �⺻�ڼ���
	Nero_BR_Lockon_Front,			// ���� ����
	Nero_BR_Strafe,		// �����ϸ� ������
	Nero_BR_Strafe_F,		// �����ϸ� ������
	Nero_BR_Strafe_FL,		// �����ϸ� ������
	Nero_BR_Strafe_L,		// �����ϸ� ������
	Nero_BR_Strafe_BL,		// �����ϸ� ������
	Nero_BR_Strafe_B,		// �����ϸ� ������
	Nero_BR_Strafe_BR,		// �����ϸ� ������
	Nero_BR_Strafe_R,		// �����ϸ� ������
	Nero_BR_Shoot,		// ���
	Nero_BR_AirShoot,		// ���߻��

	Nero_Overture_Shoot,
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
	std::shared_ptr<class GameEngineFBXRenderer> Renderer_EffectMesh = nullptr;
	std::shared_ptr<class GameEngineFBXRenderer> Renderer_Overture = nullptr;

	GameEngineFSM FSM;
	float WalkSpeed = 300;
	float RunSpeed = 550;
	float DashSpeed = 700;
	float DashTimer = 0;
	float JumpForce = 7000.0f;

	bool InputCheck = false;	// �ִϸ��̼� ����� �ٸ� �Է��� �޾� FSM������ �������� ����
	bool MoveCheck = false;		// �ִϸ��̼� ����� �̵� �Է��� �޾� FSM������ �������� ����
	bool DelayCheck = false;	// �ٸ� �޺��� ����Ǳ� ���� ������ ����

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
	void OffDevilBreaker();
};

