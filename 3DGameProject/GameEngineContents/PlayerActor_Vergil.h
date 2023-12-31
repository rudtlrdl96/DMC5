#pragma once
#include "BasePlayerActor.h"
#include <GameEngineCore/GameEngineFSM.h>
#include "Poolable.h"

enum FSM_State_Vergil
{
	// 기본 움직임
	Vergil_None,
	// Idle
	Vergil_Idle,
	Vergil_IdleLockOn,
	// 걷기 (락온 시 이동)
	Vergil_Walk,
	Vergil_WalkFront,
	Vergil_WalkLeft,
	Vergil_WalkBack,
	Vergil_WalkRight,
	// 달리기
	Vergil_RunStart,
	Vergil_Run,
	Vergil_RunStop,
	// 점프
	Vergil_Jump_Vertical,	// 점프시작
	Vergil_Jump_Fly,		// 공중상태
	Vergil_Landing,			// 착지
	// 워프 기술
	Vergil_Warp_Front_1,
	Vergil_Warp_Front_2,
	Vergil_Warp_Back_1,
	Vergil_Warp_Back_2,
	Vergil_Warp_Left_1,
	Vergil_Warp_Left_2,
	Vergil_Warp_Right_1,
	Vergil_Warp_Right_2,
	Vergil_Warp_AirTrick,	// 상대방에게 날라가는 기술
	Vergil_Warp_TrickDown,

	// 야마토 기술
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

	pl0300_yamato_Sissonal_1,	// 대시공격
	pl0300_yamato_Sissonal_2,	
	pl0300_yamato_Sissonal_3,
	pl0300_yamato_Sissonal_Up,

	pl0300_yamato_Upper_1,
	pl0300_yamato_Upper_2,	// 어퍼하면서 위로
	pl0300_yamato_Upper_3,	// 지상에서 어퍼

	Vergil_yamato_JudgementCut_1,
	Vergil_yamato_JudgementCut_2,
	Vergil_yamato_JudgementCut_3,

	Vergil_yamato_JudgementCutAir_1,
	Vergil_yamato_JudgementCutAir_2,
	Vergil_yamato_JudgementCutAir_3,

	Vergil_yamato_JudgementCutEnd_1,
	Vergil_yamato_JudgementCutEnd_2,

	// 데빌트리거 기술
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
	float DashSpeed = 700;
	float JumpMoveForce = 7000.0f;
	float FlyDownForce = 2400.0f;

	bool InputCheck = false;	// 애니메이션 재생중 다른 입력을 받아 FSM변경이 가능한지 여부
	bool MoveCheck = false;		// 애니메이션 재생중 이동 입력을 받아 FSM변경이 가능한지 여부
	bool DelayCheck = false;	// 다른 콤보로 연결되기 위한 딜레이 여부
	bool LoadCheck = false;		// FBX 로드가 완료되었는지

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

	// 검 버튼을 입력한 경우 True 리턴 및 State 변경
	bool Input_SwordCheck(int AddState = 0);
	// 검 버튼을 공중에서 입력한 경우 True 리턴 및 State 변경
	bool Input_SwordCheckFly(int AddState = 0);
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
	void SetDemon();
	void WeaponIdle();
	void YamatoOff();
	void YamatoOn();

	int DamageCalculate(int _Damage);

	void DrawEditor() override;

	void Cheat() override;
};

