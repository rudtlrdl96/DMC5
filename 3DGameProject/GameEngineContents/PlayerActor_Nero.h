#pragma once
#include "BasePlayerActor.h"
#include <GameEngineCore/GameEngineFSM.h>
#include "Poolable.h"

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
	Nero_Jump_Back,
	Nero_Jump_Fly,
	Nero_2nd_Jump,
	Nero_2nd_Jump_Back,
	Nero_EnemyStep_Jump,
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
	Nero_RQ_Skill_Shuffle,
	Nero_RQ_Skill_Stleak1,
	Nero_RQ_Skill_Stleak2,
	Nero_RQ_Skill_Stleak3,


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
	Nero_Overture_AirShoot,

	Nero_Gerbera_Back,	// �ź��� �ڷν� (������ �̵�)
	Nero_Gerbera_Front,
	Nero_Gerbera_Left,
	Nero_Gerbera_Right,

	Nero_Buster_Catch,
	Nero_Buster_Strike,
	Nero_Buster_Repelled,

	Nero_Buster_Catch_Air,
	Nero_Buster_Strike_Air,
	Nero_Buster_Repelled_Air,

	Nero_Buster_em5501,

	Nero_Snatch_Shoot,
	Nero_Snatch_Pull,
	Nero_Snatch_Repel,

	Nero_Snatch_Shoot_Air,
	Nero_Snatch_Pull_Air,
	Nero_Snatch_Repel_Air,

	// ����Ʈ���� ���
	Nero_DT_Start,
	Nero_DT_AirStart,
	Nero_GT_Bomb,
	Nero_GT_AirBomb,
	Nero_Provocation,
	Nero_Provocation_Air,
	Nero_EnemyStep,

	Nero_Damage_Light,
	Nero_Damage_Heavy,

	Nero_Damage_Fly,
	Nero_Damage_Fall,
	Nero_Damage_Ground,
	Nero_Damage_GetUp,

	Nero_Damage_Death_1,
	Nero_Damage_Death_2,
	Nero_Damage_Fly_Death,

};
enum NeroRenderUnits
{
	NeroRenderUnit_Human0,
	NeroRenderUnit_Human1,
	NeroRenderUnit_Human2,
	NeroRenderUnit_BusterArm0,
	NeroRenderUnit_BusterArm1,
	NeroRenderUnit_RQHand,
	NeroRenderUnit_DT0,
	NeroRenderUnit_DT1,
	NeroRenderUnit_Human3,
	NeroRenderUnit_Human4,
	NeroRenderUnit_Gerbera0,
	NeroRenderUnit_Gerbera1,
	NeroRenderUnit_BR,
	NeroRenderUnit_Human5,
	NeroRenderUnit_Overture,
	NeroRenderUnit_DT2,
	NeroRenderUnit_DT3,
	NeroRenderUnit_RQBack,
	NeroRenderUnit_Human6,
	NeroRenderUnit_Human7,
	NeroRenderUnit_Human8,
	NeroRenderUnit_Human9,
	NeroRenderUnit_DT4,
	NeroRenderUnit_DT5,
	NeroRenderUnit_DT6,
};
class PlayerActor_Nero : public BasePlayerActor, public Poolable<PlayerActor_Nero>
{
public:
	// constrcuter destructer
	PlayerActor_Nero()
	{
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
	void SetDT(unsigned int _DTValue) override;

	void SetArm(unsigned int _ArmValue) override
	{
		if (_ArmValue == 0)
		{
			OffDevilBraeker();
		}
		else if (_ArmValue == 1)
		{
			SetOverture();
		}
		else if (_ArmValue == 2)
		{
			SetGerbera();
		}
		else if (_ArmValue == 3)
		{
			SetBusterArm();
		}
	}

	static std::list<DevilBreaker>* GetBreakerListPointer()
	{
		return &BreakerList;
	}

protected:
	void Start() override;
	void PlayerLoad();
	void NetLoad();
	void SoundLoad();
	void Update_Character(float _DeltaTime) override;
	void ItemColCheck();
	void LightDamage() override;
	void HeavyDamage() override;
	void AddDTGauge(float _Value) override;
	void ShopOn() override;
	void ShopOff() override;
private:
	std::shared_ptr<class GameEngineFBXRenderer> Renderer_Overture = nullptr;
	std::shared_ptr<class Player_Snatch> Snatch = nullptr;
	class ZoomEffect* Effect_Zoom = nullptr;
	GameEngineFSM FSM;
	float4 LockOnPosition = float4::ZERO;
	int ExceedLevel = 0;	// �ͽõ� �ܰ�
	float WalkSpeed = 300;
	float RunSpeed = 550;
	float DashSpeed = 700;
	float DashTimer = 0;
	float JumpMoveForce = 7000.0f;
	float FlyDownForce = 2400.0f;

	bool InputCheck = false;	// �ִϸ��̼� ����� �ٸ� �Է��� �޾� FSM������ �������� ����
	bool MoveCheck = false;		// �ִϸ��̼� ����� �̵� �Է��� �޾� FSM������ �������� ����
	bool DelayCheck = false;	// �ٸ� �޺��� ����Ǳ� ���� ������ ����
	bool LoadCheck = false;		// FBX �ε尡 �Ϸ�Ǿ�����
	bool UseDoubleJump = false;
	bool UseCaliber = false;
	bool IsExActTiming = false;	// �ͽõ� Ÿ�̹� ����
	bool IsMaxActTiming = false;	// �߽� ��Ʈ Ÿ�̹� ����
	bool IsActFaild = false;		// ��Ʈ ���� ����

	bool IsStreak =			true;
	bool IsSplit =			true;
	bool IsRedqueen =		true;
	bool IsRouletteSpin =	true;
	bool IsShufle =			true;
	bool IsCalibur =		true;
	bool IsDelayBomb =		true;

	DevilBreaker CurDevilBreaker = DevilBreaker::None;
	static std::list<DevilBreaker> BreakerList;

	// �� ��ư�� �Է��� ��� ����
	bool Input_SwordCheck(int AddState = 0);
	// �� ��ư�� ���߿��� �Է��� ��� ����
	bool Input_SwordCheckFly(int AddState = 0);
	// �� ��ư�� �Է��� ��� ����
	bool Input_GunCheck();
	// �� ��ư�� ���߿��� �Է��� ��� ����
	bool Input_GunCheckFly();
	// ���� ��ư�� �Է��� ��� ����
	bool Input_JumpCheck();
	// ���� ��ư�� ���߿��� �Է��� ��� ����
	bool Input_JumpCheckFly();
	// �����극��Ŀ ��ư�� �Է��� ��� ����
	bool Input_DevilBreakerCheck();
	// �����극��Ŀ ��ư�� ���߿��� �Է��� ��� ����
	bool Input_DevilBreakerCheckFly();

	// � �������̳� ������� ĵ���Ͽ� ������ ���
	bool Input_SpecialCheck();
	bool Input_SpecialCheckFly();

	//void ChangeState(FSM_State_Nero _StateValue);	// FSM ���� �Լ�
	void ChangeState(int _StateValue);	// FSM ���� �Լ�

	void RedQueenOn();	// �������� ���� ����
	void RedQueenOff();	// �������� � �� ����
	void BlueRoseOn();	// ������ ����
	void WeaponIdle();	// Idle ���½��� ����� ����
	void SetHuman();	// �ΰ� ���
	void SetDemon();	// ����Ʈ���� ���

	void SetOverture();	// �����߾ ������ ������� ����
	void SetGerbera();	// �ź��� ������ ������� ����
	void SetBusterArm();	// �����;��� ������ ������� ����
	void SetOvertureAnimation();	// �����߾� �߻� �ִϸ��̼����� ����
	void OnDevilBraeker();
	void OffDevilBraeker();

	void AddBreaker(DevilBreaker _Breaker);
	void DestroyBreaker();

	void SetExActTiming();
	void UseExceed(int _Level = 1);

	int DamageCalculate(int _Damage, bool _IsSkill = false);
	std::shared_ptr<class NeroHPUI> HUD = nullptr;
	std::shared_ptr<class NeroItemGlass> HUD2 = nullptr;

};

