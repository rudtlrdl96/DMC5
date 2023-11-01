#pragma once
#include "BasePlayerActor.h"
#include <GameEngineCore/GameEngineFSM.h>
#include "Poolable.h"

enum FSM_State_Vergil
{
	// �⺻ ������
	Vergil_None,
	// Idle
	Vergil_Idle,
	Vergil_IdleLockOn,
	// �ȱ� (���� �� �̵�)
	Vergil_Walk,
	Vergil_WalkFront,
	Vergil_WalkLeft,
	Vergil_WalkBack,
	Vergil_WalkRight,
	// �޸���
	Vergil_RunStart,
	Vergil_Run,
	Vergil_RunStop,
	// ����
	Vergil_Jump_Vertical,	// ��������
	Vergil_Jump_Fly,		// ���߻���
	Vergil_Landing,			// ����
	// ���� ���
	Vergil_Warp_Front_1,
	Vergil_Warp_Front_2,
	Vergil_Warp_Back_1,
	Vergil_Warp_Back_2,
	Vergil_Warp_Left_1,
	Vergil_Warp_Left_2,
	Vergil_Warp_Right_1,
	Vergil_Warp_Right_2,
	Vergil_Warp_AirTrick,	// ���濡�� ���󰡴� ���
	Vergil_Warp_TrickDown,

	// �߸��� ���
	Vergil_yamato_Combo_1,
	Vergil_yamato_Combo_2,
	Vergil_yamato_Combo_3,
	Vergil_yamato_Combo_4,
	Vergil_yamato_ComboC_1,
	Vergil_yamato_ComboC_2,
	Vergil_yamato_ComboC_3,

	Vergil_yamato_Air_Combo_1,
	Vergil_yamato_Air_Combo_2,
	Vergil_yamato_Air_Combo_3,
	Vergil_yamato_Air_ComboB_1,
	Vergil_yamato_Air_ComboB_2,

	Vergil_yamato_Raid1,
	Vergil_yamato_Raid2,
	Vergil_yamato_Raid3,

	pl0300_yamato_Sissonal_1,	// ��ð���
	pl0300_yamato_Sissonal_2,	
	pl0300_yamato_Sissonal_3,
	pl0300_yamato_Sissonal_Up,

	pl0300_yamato_Upper_1,
	pl0300_yamato_Upper_2,	// �����ϸ鼭 ����
	pl0300_yamato_Upper_3,	// ���󿡼� ����

	Vergil_yamato_JudgementCut_1,
	Vergil_yamato_JudgementCut_2,
	Vergil_yamato_JudgementCut_3,

	Vergil_yamato_JudgementCutAir_1,
	Vergil_yamato_JudgementCutAir_2,
	Vergil_yamato_JudgementCutAir_3,

	Vergil_yamato_JudgementCutEnd_1,
	Vergil_yamato_JudgementCutEnd_2,

	// ����Ʈ���� ���
	Vergil_DT_Start,
	Vergil_DT_End,

	Vergil_Damage_Light,
	Vergil_Damage_Heavy,

	Vergil_Damage_Fly,
	Vergil_Damage_Fall,
	Vergil_Damage_Ground,

	Vergil_Provocation,

	Vergil_Damage_Death,
	Vergil_Damage_Fly_Death,
};

enum FSM_State_MirageBlade
{
	MirageBlade_None,
	MirageBlade_Idle,
	MirageBlade_Shoot,
	MirageBlade_Spiral,
	MirageBlade_SpiralShoot,
	MirageBlade_Storm,
	MirageBlade_Blistering,
	MirageBlade_HeavyRain,
};

class PlayerActor_Vergil : public BasePlayerActor, public Poolable<PlayerActor_Vergil>
{
public:
	// constrcuter destructer
	PlayerActor_Vergil()
	{
	}
	~PlayerActor_Vergil();

	// delete Function
	PlayerActor_Vergil(const PlayerActor_Vergil& _Other) = delete;
	PlayerActor_Vergil(PlayerActor_Vergil&& _Other) noexcept = delete;
	PlayerActor_Vergil& operator=(const PlayerActor_Vergil& _Other) = delete;
	PlayerActor_Vergil& operator=(PlayerActor_Vergil&& _Other) noexcept = delete;

	void SetFSMStateValue(int _StateValue) override
	{
		FSM.ChangeState(_StateValue);
	}
	void SetDT(unsigned int _DTValue) override;
	void SetArm(unsigned int _ArmValue) override
	{
		return;
	}

protected:
	void Start() override;
	void PlayerLoad();
	void NetLoad();
	void SoundLoad();
	void Update_Character(float _DeltaTime) override;

	void LightDamage() override;
	void HeavyDamage() override;
	void AddDTGauge(float _Value) override;

	void ShopOn() override;
	void ShopOff() override;
private:
	std::vector<std::shared_ptr<class Player_MirageBlade>> AllMirageBlades;
	std::shared_ptr<class FXSystem> EffectSystem_Target = nullptr;
	class JudgementCut* Effect_JC = nullptr;
	class ColorEffect* Effect_Color = nullptr;
	GameEngineFSM FSM;
	GameEngineFSM FSM_MirageBlade;
	float4 LockOnPosition = float4::ZERO;
	float WalkSpeed = 300;
	float RunSpeed = 550;
	float JumpMoveForce = 7000.0f;
	float FlyDownForce = 2400.0f;

	bool InputCheck = false;	// �ִϸ��̼� ����� �ٸ� �Է��� �޾� FSM������ �������� ����
	bool MoveCheck = false;		// �ִϸ��̼� ����� �̵� �Է��� �޾� FSM������ �������� ����
	bool DelayCheck = false;	// �ٸ� �޺��� ����Ǳ� ���� ������ ����
	bool LoadCheck = false;		// FBX �ε尡 �Ϸ�Ǿ�����

	bool IsRapidSlash =		false;
	bool IsUpperSlash =		false;
	bool IsAerialRave =		false;
	bool IsYamatoCombo =	false;
	bool IsJudgmentCutEnd = false;
	bool IsSpiralBlade =	false;
	bool IsStormBlade =		false;
	bool IsLesteringBlade = false;
	bool IsHeavyRainBlade = false;

	void ChangeState(int _StateValue);
	void CreateMirageBlade();

	// �� ��ư�� �Է��� ��� True ���� �� State ����
	bool Input_SwordCheck(int AddState = 0);
	// �� ��ư�� ���߿��� �Է��� ��� True ���� �� State ����
	bool Input_SwordCheckFly(int AddState = 0);
	// ���� ��ư�� �Է��� ��� True ���� �� State ����
	bool Input_JumpCheck();
	// ���� ��ư�� ���߿��� �Է��� ��� True ���� �� State ����
	bool Input_JumpCheckFly();
	// ���� ��ư�� �Է��� ��� True ���� �� State ����
	bool Input_WarpCheck();
	// ���� ��ư�� ���߿��� �Է��� ��� True ���� �� State ����
	bool Input_WarpCheckFly();

	// � �������̳� ������� ĵ���Ͽ� ������ ���
	bool Input_SpecialCheck();
	bool Input_SpecialCheckFly();

	void SetHuman();
	void SetDemon();
	void WeaponIdle();
	void YamatoOff();
	void YamatoOn();

	int DamageCalculate(int _Damage);

	void DrawEditor() override;

	void Cheat() override;
};

