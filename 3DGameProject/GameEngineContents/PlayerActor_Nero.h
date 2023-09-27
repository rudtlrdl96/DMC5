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

	// 하이룰러 (뛰우는 공격)
	Nero_RQ_Skill_HR,
	Nero_RQ_Skill_Shuffle,
	Nero_RQ_Skill_Stleak1,
	Nero_RQ_Skill_Stleak2,
	Nero_RQ_Skill_Stleak3,


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

	Nero_Overture_Shoot,
	Nero_Overture_AirShoot,

	Nero_Gerbera_Back,	// 거베라 뒤로쏨 (앞으로 이동)
	Nero_Gerbera_Front,
	Nero_Gerbera_Left,
	Nero_Gerbera_Right,

	Nero_Buster_Catch,
	Nero_Buster_Strike,
	Nero_Buster_Repelled,

	Nero_Buster_Catch_Air,
	Nero_Buster_Strike_Air,
	Nero_Buster_Repelled_Air,

	Nero_Snatch_Shoot,
	Nero_Snatch_Pull,
	Nero_Snatch_Repel,

	Nero_Snatch_Shoot_Air,
	Nero_Snatch_Pull_Air,
	Nero_Snatch_Repel_Air,

	// 데빌트리거 사용
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

class PlayerActor_Nero : public BasePlayerActor
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
	void SetDT(unsigned int _DTValue) override
	{
		if (_DTValue == 1)
		{
			SetDemon();
		}
		else
		{
			SetHuman();
		}
	}
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
	void Update_Character(float _DeltaTime) override;

	void LightDamage() override;
	void HeavyDamage() override;
private:
	std::shared_ptr<class GameEngineFBXRenderer> Renderer_Overture = nullptr;
	std::shared_ptr<class FXSystem> EffectSystem = nullptr;

	GameEngineFSM FSM;
	float WalkSpeed = 300;
	float RunSpeed = 550;
	float DashSpeed = 700;
	float DashTimer = 0;
	float JumpMoveForce = 7000.0f;
	float FlyDownForce = 2400.0f;

	bool InputCheck = false;	// 애니메이션 재생중 다른 입력을 받아 FSM변경이 가능한지 여부
	bool MoveCheck = false;		// 애니메이션 재생중 이동 입력을 받아 FSM변경이 가능한지 여부
	bool DelayCheck = false;	// 다른 콤보로 연결되기 위한 딜레이 여부
	bool LoadCheck = false;		// FBX 로드가 완료되었는지
	bool UseDoubleJump = false;
	bool IsDevilTrigger = false;
	DevilBreaker CurDevilBreaker = DevilBreaker::Overture;
	static std::list<DevilBreaker> BreakerList;

	// 검 버튼을 입력한 경우 실행
	bool Input_SwordCheck(int AddState = 0);
	// 검 버튼을 공중에서 입력한 경우 실행
	bool Input_SwordCheckFly(int AddState = 0);
	// 총 버튼을 입력한 경우 실행
	bool Input_GunCheck();
	// 총 버튼을 공중에서 입력한 경우 실행
	bool Input_GunCheckFly();
	// 점프 버튼을 입력한 경우 실행
	bool Input_JumpCheck();
	// 점프 버튼을 공중에서 입력한 경우 실행
	bool Input_JumpCheckFly();
	// 데빌브레이커 버튼을 입력한 경우 실행
	bool Input_DevilBreakerCheck();
	// 데빌브레이커 버튼을 공중에서 입력한 경우 실행
	bool Input_DevilBreakerCheckFly();

	// 어떤 프레임이나 상관없이 캔슬하여 나가는 기술
	bool Input_SpecialCheck();
	bool Input_SpecialCheckFly();

	//void ChangeState(FSM_State_Nero _StateValue);	// FSM 변경 함수
	void ChangeState(int _StateValue);	// FSM 변경 함수

	void RedQueenOn();	// 레드퀸을 꺼낸 상태
	void RedQueenOff();	// 레드퀸을 등에 둔 상태
	void BlueRoseOn();	// 블루로즈 장착
	void WeaponIdle();	// Idle 상태시의 무기로 변경
	void SetHuman();	// 인간 모습
	void SetDemon();	// 데빌트리거 모습

	void SetOverture();	// 오버추어를 장착한 모습으로 지정
	void SetGerbera();	// 거베라를 장착한 모습으로 지정
	void SetBusterArm();	// 버스터암을 장착한 모습으로 지정
	void SetOvertureAnimation();	// 오버추어 발사 애니메이션으로 지정
	void OnDevilBraeker();
	void OffDevilBraeker();

	void AddBreaker(DevilBreaker _Breaker);
	void DestroyBreaker();

	std::shared_ptr<class NeroHPUI> HPRender = nullptr;

};

