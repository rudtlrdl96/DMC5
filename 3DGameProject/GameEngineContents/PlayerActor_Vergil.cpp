#include "PrecompileHeader.h"
#include "PlayerActor_Vergil.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include "AnimationEvent.h"
#include "PlayerController.h"
#include "AttackCollision.h"
#include "NetworkManager.h"
#include "FXSystem.h"
#include "JudgementCut.h"
#include "ColorEffect.h"
#include "PlayerCamera.h"
#include "RankUI.h"
#include "BGMPlayer.h"
#include "PlayerHPUI.h"
#include "UI_DTGauge.h"
#include "Vergil_ShopUI.h"
#include "Shop_VergilYamatoUI.h"
#include "Shop_VergilMirgeUI.h"
PlayerActor_Vergil::~PlayerActor_Vergil()
{
}

void PlayerActor_Vergil::SetDT(unsigned int _DTValue)
{
	if (_DTValue == 1)
	{
		SetDemon();
	}
	else
	{
		DTOffEffect->Play();
		Sound.Play("DT_Off");
		SetHuman();
	}
}


void PlayerActor_Vergil::Start()
{
	BasePlayerActor::Start();
	Col_Attack->SetParryCallBack([=]
		{
			RankUI::GetRankInst()->AddRankScore(100);
			Sound.Play("Vergil_Parry");
			SetTimeScale(0.2f);
			GetLevel()->TimeEvent.AddEvent(0.4f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					SetTimeScale(0.4f);
				});
			GetLevel()->TimeEvent.AddEvent(0.6f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					SetTimeScale(0.6f);
				});
			GetLevel()->TimeEvent.AddEvent(0.8f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					SetTimeScale(0.8f);
				});
			GetLevel()->TimeEvent.AddEvent(1.f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					SetTimeScale(1.f);
				});
		});
	Col_Attack->SetDamageSoundType(DamageSoundType::Katana);

	SetNetObjectType(Net_ActorType::Vergil);
	GetLevel()->CreateActor<RankUI>();
	Effect_JC = JudgementCut::GetJudgementCutEffect();
	Effect_Color = ColorEffect::GetColorEffect();
	//NetControllType::NetControll으로 변경될 때 아래 콜백이 실행됩니다. 
	SetControllCallBack(NetControllType::PassiveControll, [=]()
		{
			NetControllLoad();
			NetLoad();
			SoundLoad();
			LoadCheck = true;
		});

	//NetControllType::UserControll으로 변경될 때 아래 콜백이 실행됩니다.
	SetControllCallBack(NetControllType::ActiveControll, [=]()
		{
			UserControllLoad();
			PlayerLoad();
			SoundLoad();
			LoadCheck = true;
		});
	LinkData_UpdatePacket<float>(LockOnPosition.x);
	LinkData_UpdatePacket<float>(LockOnPosition.y);
	LinkData_UpdatePacket<float>(LockOnPosition.z);

	Sound.SetVoiceName("Vergil_V_");
	Sound.SFXVolume = 0.3f;
}

void PlayerActor_Vergil::PlayerLoad()
{
	if (true == LoadCheck) { return; }
	BGMPlayer::SetCharater(PlayerType::Vergil);
	HUD = GetLevel()->CreateActor<PlayerHPUI>();
	HUD->SetVergilUI();
	HUD->SetPlayerHP(HP);
	Shop = GetLevel()->CreateActor<Vergil_ShopUI>();
	Shop->ShopOff();

	// Effect 생성
	{
		EffectSystem = CreateComponent<FXSystem>();
		EffectSystem_Target = CreateComponent<FXSystem>();

		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Effect");
		NewDir.Move("Vergil");

		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".effect" });
		for (GameEngineFile File : Files)
		{
			if (nullptr == FXData::Find(File.GetFileName()))
			{
				FXData::Load(File.GetFullPath());
			}
			EffectSystem->CreateFX(FXData::Find(File.GetFileName()));
		}
		EffectSystem_Target->CreateFX(FXData::Find("Yamato_JC_Effect.effect"));

		NewDir.MoveParent();
		NewDir.Move("Damage");
		Files = NewDir.GetAllFile({ ".effect" });
		for (GameEngineFile File : Files)
		{
			if (nullptr == FXData::Find(File.GetFileName()))
			{
				FXData::Load(File.GetFullPath());
			}
			EffectSystem->CreateFX(FXData::Find(File.GetFileName()));
		}
		DTOffEffect->CreateFX(FXData::Find("Vergil_DT_Off.effect"));
		DTOffEffect->ChangeFX("Vergil_DT_Off.effect");
		DTOffEffect->Off();

		NewDir.MoveParent();
		NewDir.Move("Mirage");
		Files = NewDir.GetAllFile({ ".effect" });
		for (GameEngineFile File : Files)
		{
			if (nullptr == FXData::Find(File.GetFileName()))
			{
				FXData::Load(File.GetFullPath());
			}
			EffectSystem->CreateFX(FXData::Find(File.GetFileName()));
		}
	}
	// Renderer 생성
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Character");
		NewDir.Move("Player");
		NewDir.Move("Vergil");
		NewDir.Move("Mesh");
		if (nullptr == GameEngineFBXMesh::Find("Vergil.FBX"))
		{
			GameEngineFBXMesh::Load(NewDir.GetPlusFileName("Vergil.fbx").GetFullPath());
			GameEngineTexture::Load(NewDir.GetPlusFileName("High\\pl0300_03_atos.texout.png").GetFullPath());
			GameEngineTexture::Load(NewDir.GetPlusFileName("High\\pl0310_trans_atos.texout.png").GetFullPath());
		}
		NewDir.MoveParent();
		NewDir.Move("Animation");

		Renderer = CreateComponent<GameEngineFBXRenderer>();
		Renderer->GetTransform()->SetLocalPosition({ 0, -75, 0 });

		//WingRender->GetTransform()->SetLocalPosition({ 0, -75, 0 });

		switch (GameEngineOption::GetOption("Shader"))
		{
		case GameEngineOptionValue::Low:
		{
			Renderer->SetFBXMesh("Vergil.FBX", "AniFBX_Low");
		}
		break;
		case GameEngineOptionValue::High:
		{
			Renderer->SetFBXMesh("Vergil.FBX", "AniFBX");
		}
		break;
		default:
			break;
		}

		Renderer->ShadowOn();
		Renderer->SetDynamic();
		// 모션블러 미적용옵션
		Renderer->GetRenderBaseValueRef().IsBlurMask = 1;

		//Renderer->SetMaterial("pl0300_03_albm.texout.png", "AniFBX_2DAlpha", nullptr);
		Renderer->SetMaterial("pl0310_trans_00_alba.texout.png", "AniFBX_2DAlpha", nullptr);
		Renderer->SetMaterial("pl0310_trans_01_alba.texout.png", "AniFBX_2DAlpha", nullptr);
		Renderer->SetSpecularTexture("pl0300_03_albm.texout.png", "pl0300_03_atos.texout.png");
		Renderer->SetSpecularTexture("pl0310_trans_00_alba.texout.png", "pl0310_trans_atos.texout.png");
		Renderer->SetSpecularTexture("pl0310_trans_01_alba.texout.png", "pl0310_trans_atos.texout.png");
		//Renderer->SetSpecularTexture("pl0300_03_albm.texout.png", "pl0300_03_atos.texout.png");

		AnimationEvent::LoadAll({ .Dir = NewDir.GetFullPath().c_str(), .Renderer = Renderer, .RendererLocalPos = {0, -75, 0},
			.Objects = { (GameEngineObject*)Col_Attack.get() },
			.CallBacks_void = {
				std::bind([=] {InputCheck = true; }),			// 0
				std::bind(&PlayerActor_Vergil::YamatoOn, this),
				std::bind(&PlayerActor_Vergil::YamatoOff, this),
				std::bind(&PlayerActor_Vergil::SetHuman, this),
				std::bind(&PlayerActor_Vergil::SetDemon, this),
				std::bind(&PhysXCapsuleComponent::SetLinearVelocityZero, PhysXCapsule.get()),		// 5
				std::bind([=] {MoveCheck = true; }),
				std::bind([=] {DelayCheck = true; }),
				std::bind(&PhysXCapsuleComponent::TurnOnGravity, PhysXCapsule.get()),		// 8 
				std::bind(&PlayerCamera::ShakeLight),	//9
				std::bind(&PlayerCamera::ShakeMiddle),	//10
				std::bind(&PlayerCamera::ShakeHeavy),	//11
			},
			.CallBacks_int = {
				std::bind(&PlayerActor_Vergil::ChangeState, this, std::placeholders::_1),
				std::bind(&SoundController::Play, &Sound, "Yamato_", std::placeholders::_1),
				std::bind(&SoundController::Play, &Sound, "FootStep_", std::placeholders::_1),
				std::bind(&RankUI::AddRankScore, RankUI::GetRankInst(), std::placeholders::_1),
			},
			.CallBacks_float = {
				std::bind(&BasePlayerActor::RotationToTarget, this, std::placeholders::_1),
				std::bind(&BasePlayerActor::RotationToMoveVector, this, std::placeholders::_1),
				std::bind(&BasePlayerActor::SetInvincibility, this, std::placeholders::_1)
			},
			.CallBacks_float4 = {
				std::bind(&BasePlayerActor::SetForce, this, std::placeholders::_1),
				std::bind(&BasePlayerActor::SetPush, this, std::placeholders::_1),
				std::bind(&BasePlayerActor::SetMove, this, std::placeholders::_1)
			}
			});
		SetHuman();
		WeaponIdle();
	}

	/* 기본 움직임 */
	{
		// Idle
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Idle,
			.Start = [=] {
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_Idle_Normal");
			},
			.Update = [=](float _DeltaTime) {
				PhysXCapsule->SetLinearVelocityZero();
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
				if (true == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Vergil::Vergil_IdleLockOn);
					return;
				}
			},
			.End = [=] {

			}
			});
		// Idle LockOn
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_IdleLockOn,
			.Start = [=] {
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_Idle_Normal");
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_Walk);
					return;
				}
				if (false == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Vergil::Vergil_Idle);
					return;
				}
			},
			.End = [=] {

			}
			});
		// Walk
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Walk,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOnGravity();
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_IdleLockOn);
					return;
				}
				if (false == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}

				LookTarget();
				float4 MoveDir;

				switch (Controller->MoveVectorToChar4(Controller->GetMoveVector()))
				{
				case '8':
					Renderer->ChangeAnimation("pl0300_Walk_Front");
					FSMValue = FSM_State_Vergil::Vergil_WalkFront;
					break;
				case '4':
					Renderer->ChangeAnimation("pl0300_Walk_Left");
					FSMValue = FSM_State_Vergil::Vergil_WalkLeft;
					break;
				case '2':
					Renderer->ChangeAnimation("pl0300_Walk_Back");
					FSMValue = FSM_State_Vergil::Vergil_WalkBack;
					break;
				case '6':
					Renderer->ChangeAnimation("pl0300_Walk_Right");
					FSMValue = FSM_State_Vergil::Vergil_WalkRight;
					break;
				}
				NetworkManager::SendFsmChangePacket(this, FSMValue);
				PhysXCapsule->SetMove(Controller->GetMoveVector() * WalkSpeed);

			},
			.End = [=] {

			}
			});
		// Run Start
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_RunStart,
			.Start = [=] {
				PhysXCapsule->TurnOnGravity();
				Renderer->ChangeAnimation("pl0300_Run_Start");
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStop);
					return;
				}
				if (true == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Vergil::Vergil_Walk);
					return;
				}
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_Run);
					return;
				}
				LookDir(Controller->GetMoveVector());
				float4 MoveDir = Controller->GetMoveVector() * RunSpeed;
				PhysXCapsule->SetMove(MoveDir);
			},
			.End = [=] {

			}
			});
		// Run
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Run,
			.Start = [=] {
				PhysXCapsule->TurnOnGravity();
				Renderer->ChangeAnimation("pl0300_Run_Loop");
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStop);
					return;
				}
				if (true == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Vergil::Vergil_Walk);
					return;
				}

				LookDir(Controller->GetMoveVector());
				float4 MoveDir = Controller->GetMoveVector() * RunSpeed;
				PhysXCapsule->SetMove(MoveDir);
			},
			.End = [=] {
			}
			});
		// RunStop
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_RunStop,
			.Start = [=] {
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				MoveCheck = false;
				Renderer->ChangeAnimation("pl0300_Run_Stop");
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {

			}
			});
		// Jump Vertical
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Jump_Vertical,
		.Start = [=] {
			Sound.Play("Jump");
			PhysXCapsule->TurnOnGravity();
			PhysXCapsule->SetLinearVelocityZero();
			PhysXCapsule->SetMove(Controller->GetMoveVector() * 500);
			InputCheck = false;
			MoveCheck = false;
			SetInvincibility(0.5f);
			WeaponIdle();
			Renderer->ChangeAnimation("pl0300_Jump_Vertical", true);
		},
		.Update = [=](float _DeltaTime) {
			if (Renderer->IsAnimationEnd())
			{
				ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
			}

			if (true == Input_SpecialCheckFly()) { return; }
			if (false == InputCheck) { return; }

			if (true == Input_JumpCheckFly()) { return; }
			if (true == Input_SwordCheckFly()) { return; }

			if (true == Input_WarpCheckFly()) { return; }

			PhysXCapsule->SetForce(Controller->GetMoveVector() * JumpMoveForce);

			if (false == MoveCheck) { return; }
			if (true == FloorCheck())
			{
				ChangeState(FSM_State_Vergil::Vergil_Landing);
				return;
			}

		},
		.End = [=] {

		}
			});
		// Jump Fly
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Jump_Fly,
			.Start = [=] {
				PhysXCapsule->SetPush(float4::DOWN * FlyDownForce);
				PhysXCapsule->TurnOnGravity();
				Renderer->ChangeAnimation("pl0300_Jump_Vertical_Fly");
			},
			.Update = [=](float _DeltaTime) {
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Landing);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }

				if (true == Input_WarpCheckFly()) { return; }
				PhysXCapsule->SetForce(Controller->GetMoveVector() * JumpMoveForce);

			},
			.End = [=] {
				PhysXCapsule->TurnOffGravity();
			}
			});
		// Landing
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Landing,
			.Start = [=] {
				SetFloorPos();
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_Jump_Vertical_Landing");
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				PhysXCapsule->SetLinearVelocityZero();
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }

				if (true == Input_WarpCheck()) { return; }
				if (false == MoveCheck) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {

			}
			});
	}
	{}
	/* 야마토 */
	{
		// Combo1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Combo_1,
			.Start = [=] {
				Sound.PlayVoiceRandom(0, 2, DTValue);
				Sound.Play("Yamato_", 0);
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(75));
				Col_Attack->SetDamageSoundType(DamageSoundType::Blunt);
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				RotationToTarget();
				EffectSystem->PlayFX("Yamato_Combo_1.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Combo_1");
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }

				if (InputCheck == false) {
					return;
				}
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck(1)) { return; }
				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				Col_Attack->SetDamageSoundType(DamageSoundType::Katana);
				WeaponIdle();
			}
			});
		// Combo2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Combo_2,
			.Start = [=] {
				Sound.Play("Yamato_", 1);
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(93));
				Col_Attack->SetDamageSoundType(DamageSoundType::Blunt);
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_Combo_2.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Combo_2");
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck(2)) { return; }

				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				Col_Attack->SetDamageSoundType(DamageSoundType::Katana);
				WeaponIdle();
			}
			});
		// Combo3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Combo_3,
			.Start = [=] {
				Sound.PlayVoiceRandom(3, 5, DTValue);
				Col_Attack->SetAttackData(DamageType::VergilLight, DamageCalculate(103));
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_Combo_3.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Combo_3");
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
				DelayCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == DelayCheck && true == IsYamatoCombo )
				{
					if (true == Input_SwordCheck(4)) { return; }
				}
				else if (true == Input_SwordCheck(3)) { return; }

				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Combo4
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Combo_4,
			.Start = [=] {
				TimeEvent.AddEvent(0.2f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Sound.PlayVoiceRandom(7, 8, DTValue);
				});
				Col_Attack->SetAttackData(DamageType::Heavy, DamageCalculate(187));
				YamatoOn();
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_Combo_4.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Combo_4");
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }

				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		static int ComboLoopCount = 0;
		// ComboC1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_ComboC_1,
			.Start = [=] {
				Sound.PlayVoiceRandom(2, 3, DTValue);
				YamatoOn();
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_yamato_ComboC_1");
				RotationToTarget(30.0f);
				ComboLoopCount = 0;
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheck()) { return; }
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_yamato_ComboC_2);
					return;
				}
			},
			.End = [=] {
			}
			});
		// ComboC2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_ComboC_2,
			.Start = [=] {
				Col_Attack->SetAttackData(DamageType::VergilLight, DamageCalculate(28));
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_Combo_C_2.effect");
				Renderer->ChangeAnimation("pl0300_yamato_ComboC_2_Loop", true);
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }

				if (Controller->GetIsSword())
				{
					InputCheck = true;
				}
				if (Renderer->IsAnimationEnd())
				{
					if (InputCheck == true && ComboLoopCount < 3)
					{
						ComboLoopCount++;
						ChangeState(FSM_State_Vergil::Vergil_yamato_ComboC_2);
						return;
					}
					ChangeState(FSM_State_Vergil::Vergil_yamato_ComboC_3);
					return;
				}
			},
			.End = [=] {
			}
			});
		// ComboC3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_ComboC_3,
			.Start = [=] {
				Sound.PlayVoiceRandom(7, 9, DTValue);
				Col_Attack->SetAttackData(DamageType::Heavy, DamageCalculate(300));
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_Combo_C_3.effect");
				Renderer->ChangeAnimation("pl0300_yamato_ComboC_3");
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }

				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Sissonal1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Sissonal_1,
			.Start = [=] {
				Sound.Play("Yamato_", 16);
				RotationToTarget();
				PhysXCapsule->TurnOnGravity();
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_Sissonal_1.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Sissonal_1");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheck()) { return; }
				if (false == FloorCheck())
				{
					WeaponIdle();
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::pl0300_yamato_Sissonal_2);
					return;
				}
			},
			.End = [=] {
			}
			});
		static float SissonalTimer = 0;
		// Sissonal2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Sissonal_2,
			.Start = [=] {
				SissonalTimer = 0;
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_yamato_Sissonal_2_loop");
				Col_EnemyStepCheck->GetTransform()->SetLocalScale({ 300, 275, 300 });
				PhysXCapsule->Off();
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheck()) { return; }
				if (true == DTValue && Controller->GetIsFrontSword())
				{
					ChangeState(FSM_State_Vergil::pl0300_yamato_Sissonal_1);
					return;
				}
				std::vector<std::shared_ptr<GameEngineCollision>> Cols;
				if (true == Controller->GetIsSwordPress() && true == Col_EnemyStepCheck->CollisionAll(CollisionOrder::Enemy, Cols))
				{
					ChangeState(FSM_State_Vergil::pl0300_yamato_Sissonal_Up);
					return;
				}
				SissonalTimer += _DeltaTime;
				if (0.1f < SissonalTimer)
				{
					ChangeState(FSM_State_Vergil::pl0300_yamato_Sissonal_3);
					return;
				}
				float4 Pos;
				if (true == GetLevel()->RayCast(GetTransform()->GetWorldPosition(), GetTransform()->GetWorldForwardVector(), Pos, 100.0f))
				{
					ChangeState(FSM_State_Vergil::pl0300_yamato_Sissonal_3);
					return;
				}
				GetTransform()->AddWorldPosition(GetTransform()->GetWorldForwardVector() * _DeltaTime * 2300);
			},
			.End = [=] {
				PhysXCapsule->On();
				SetWorldPosition(GetTransform()->GetWorldPosition());
				Col_EnemyStepCheck->GetTransform()->SetLocalScale({ 200, 175, 200 });
				PhysXCapsule->SetLinearVelocityZero();
				Controller->SwordChargeTimer = -1.0f;
			}
			});
		// Sissonal3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Sissonal_3,
			.Start = [=] {
				Sound.Play("Yamato_", 7);
				Sound.PlayVoiceRandom(16, 18, DTValue);
				Sound.NoSkipOn();
				Col_Attack->SetAttackData(DamageType::Heavy, DamageCalculate(600));
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->SetMove(GetTransform()->GetWorldForwardVector() * 800);
				SetFloorPos();
				//EffectSystem->PlayFX("Yamato_Sissonal_3.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Sissonal_3");
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {

				if (true == Input_SpecialCheck()) { return; }

				if (true == DTValue && Controller->GetIsFrontSword())
				{
					ChangeState(FSM_State_Vergil::pl0300_yamato_Sissonal_1);
					return;
				}
				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }

				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				SetWorldPosition(GetTransform()->GetWorldPosition());
				WeaponIdle();
				Rot.y += 180.0f;
				PhysXCapsule->AddWorldRotation({ 0, 180, 0 });
				GetTransform()->AddWorldRotation({ 0, 180, 0 });
			}
			});
		// Sissonal Up
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Sissonal_Up,
			.Start = [=] {
				Sound.PlayVoiceRandom(18, 19, DTValue);
				Sound.NoSkipOn();
				RotationToTarget();
				Col_Attack->SetAttackData(DamageType::Air, DamageCalculate(150));
				TimeEvent.AddEvent(0.5f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(131));
				});
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_Sissonal_Up.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Sissonal_Up");
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }

				if (true == Input_WarpCheckFly()) { return; }
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Upper 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Upper_1,
			.Start = [=] {
				Sound.PlayVoiceRandom(18, 19, DTValue);
				Sound.NoSkipOn();
				RotationToTarget();
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_yamato_AttackUp_1");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheck()) { return; }
				if (Renderer->IsAnimationEnd())
				{
					if (true == Controller->GetIsSwordPress())
					{
						ChangeState(FSM_State_Vergil::pl0300_yamato_Upper_2);
						return;
					}
					ChangeState(FSM_State_Vergil::pl0300_yamato_Upper_3);
					return;
				}
			},
			.End = [=] {
				Controller->SwordChargeTimer = -1.0f;
			}
			});
		// Upper 2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Upper_2,
			.Start = [=] {
				Sound.Play("Yamato_", 6);
				YamatoOn();
				Col_Attack->SetAttackData(DamageType::Air, DamageCalculate(168));
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_AttackUp_2.effect");
				Renderer->ChangeAnimation("pl0300_yamato_AttackUp_2");
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }

				if (true == Input_WarpCheckFly()) { return; }
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Upper3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Upper_3,
			.Start = [=] {
				Sound.Play("Yamato_", 7);
				YamatoOn();
				Col_Attack->SetAttackData(DamageType::Air, DamageCalculate(168));
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_AttackUp_3.effect");
				Renderer->ChangeAnimation("pl0300_yamato_AttackUp_3");
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }

				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});

		// Air Combo 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Air_Combo_1,
			.Start = [=] {
				Sound.Play("Yamato_", 2);
				Sound.PlayVoiceRandom(1, 2, DTValue);
				RotationToTarget();
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(93));
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_Air_Combo_1.effect");
				Renderer->ChangeAnimation("pl0300_yamato_AirCombo_1");
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Landing);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly(1)) { return; }

				if (true == Input_WarpCheckFly()) { return; }
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Air Combo 2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Air_Combo_2,
			.Start = [=] {
				Sound.Play("Yamato_", 4);
				YamatoOn();
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(112));
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_Air_Combo_2.effect");
				Renderer->ChangeAnimation("pl0300_yamato_AirCombo_2");
				InputCheck = false;
				DelayCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Landing);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == DelayCheck)
				{
					if (true == Input_SwordCheckFly(3)) { return; }
				}
				else if (true == Input_SwordCheckFly(2)) { return; }

				if (true == Input_WarpCheckFly()) { return; }
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Air Combo 3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Air_Combo_3,
			.Start = [=] {
				Sound.Play("Yamato_", 3);
				Sound.PlayVoiceRandom(5, 6, DTValue);
				YamatoOn();
				Col_Attack->SetAttackData(DamageType::Heavy, DamageCalculate(225));
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_Air_Combo_3.effect");
				Renderer->ChangeAnimation("pl0300_yamato_AirCombo_3");
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Landing);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }

				if (true == Input_WarpCheckFly()) { return; }
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Air Combo B 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Air_ComboB_1,
			.Start = [=] {
				Sound.PlayVoiceRandom(5, 6, DTValue);
				RotationToTarget();
				YamatoOn();
				Col_Attack->SetAttackData(DamageType::Air, DamageCalculate(375));
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_Air_ComboB_1.effect");
				Renderer->ChangeAnimation("pl0300_yamato_AirComboB_1");
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Landing);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly(4)) { return; }

				if (true == Input_WarpCheckFly()) { return; }
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Air Combo B 2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Air_ComboB_2,
			.Start = [=] {
				Sound.Play("Yamato_", 7);
				Sound.PlayVoiceRandom(7, 8, DTValue);
				YamatoOn();
				Col_Attack->SetAttackData(DamageType::Slam, DamageCalculate(187));
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_Air_Combo_3.effect");
				Renderer->ChangeAnimation("pl0300_yamato_AirComboB_2");
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Landing);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }

				if (true == Input_WarpCheckFly()) { return; }
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Raid1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Raid1,
			.Start = [=] {
				Sound.PlayVoiceRandom(12, 13, DTValue);
				RotationToTarget();
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				RotationToTarget();
				Renderer->ChangeAnimation("pl0300_yamato_Raid1");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheck()) { return; }
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_yamato_Raid2);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Raid2
		static float4 RaidTargetPos;
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Raid2,
			.Start = [=] {
				Col_Attack->SetAttackData(DamageType::Slam, DamageCalculate(525));
				YamatoOn();
				Renderer->ChangeAnimation("pl0300_yamato_Raid2_Loop");
				GetLevel()->RayCast(GetTransform()->GetWorldPosition(), float4::DOWN, RaidTargetPos, 9999.0f);
				RaidTargetPos += float4::UP * 100;
				PhysXCapsule->Off();
			},
			.Update = [=](float _DeltaTime) {
				GetTransform()->AddWorldPosition(float4::DOWN * 3000 * _DeltaTime);
				//PhysXCapsule->SetMove(float4::DOWN * 500);
				if (GetTransform()->GetWorldPosition().y < RaidTargetPos.y)
				{
					PhysXCapsule->SetWorldPosition(RaidTargetPos);
					ChangeState(FSM_State_Vergil::Vergil_yamato_Raid3);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }

				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_yamato_Raid3);
					return;
				}
			},
			.End = [=] {
				PhysXCapsule->On();
				WeaponIdle();
			}
			});
		// Raid3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Raid3,
			.Start = [=] {
				Sound.Play("Yamato_", 8);
				Controller->SwordChargeTimer = 0.0f;
				YamatoOn();
				SetFloorPos();
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				EffectSystem->PlayFX("Yamato_Raid_3.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Raid3");
			},
			.Update = [=](float _DeltaTime) {
				PhysXCapsule->SetLinearVelocityZero();

				if (true == Input_SpecialCheck()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }

				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});

		// JudgementCut 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCut_1,
			.Start = [=] {
				RotationToTarget();
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_yamato_JudgementCut_1");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheck()) { return; }
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCut_2);
				}
			},
			.End = [=] {

			}
			});
		// Vergil_yamato_JudgementCut_2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCut_2,
			.Start = [=] {
				Sound.Play("Yamato_", 13);
				Sound.Play("Yamato_", 14);
				Sound.PlayVoiceRandom(13, 15, DTValue);
				Sound.NoSkipOn();
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(450));
				EffectSystem->PlayFX("Yamato_JudgementCut.effect");

				if (nullptr != LockOnEnemyTransform)
				{
					EffectSystem_Target->GetTransform()->SetWorldPosition(LockOnEnemyTransform->GetWorldPosition());
					Col_Attack->GetTransform()->SetWorldPosition(LockOnEnemyTransform->GetWorldPosition());
				}
				else
				{
					EffectSystem_Target->GetTransform()->SetLocalPosition({0, 0, 200});
					Col_Attack->GetTransform()->SetLocalPosition({ 0, 0, 200 });
				}
				LockOnPosition = EffectSystem_Target->GetTransform()->GetWorldPosition();
				Col_Attack->GetTransform()->SetLocalScale({300, 300, 300});
				Col_Attack->On();
				TimeEvent.AddEvent(0.08f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Col_Attack->Off();
				});
				EffectSystem_Target->PlayFX("Yamato_JC_Effect.effect");
				Renderer->ChangeAnimation("pl0300_yamato_JudgementCut_2_Loop");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheck()) { return; }
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCut_3);
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Vergil_yamato_JudgementCut_3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCut_3,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0300_yamato_JudgementCut_3");
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {

				if (true == Input_SpecialCheck()) { return; }

				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }

				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
			}
			});
		// Vergil_yamato_JudgementCutAir_1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCutAir_1,
			.Start = [=] {
				RotationToTarget();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0300_yamato_Air_JudgementCut_1");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheckFly()) { return; }
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCutAir_2);
				}
			},
			.End = [=] {

			}
			});
		// Vergil_yamato_JudgementCutAir_2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCutAir_2,
			.Start = [=] {
				Sound.Play("Yamato_", 13);
				Sound.Play("Yamato_", 14);
				Sound.PlayVoiceRandom(13, 15, DTValue);
				Sound.NoSkipOn();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(450));
				if (nullptr != LockOnEnemyTransform)
				{
					EffectSystem_Target->GetTransform()->SetWorldPosition(LockOnEnemyTransform->GetWorldPosition());
					Col_Attack->GetTransform()->SetWorldPosition(LockOnEnemyTransform->GetWorldPosition());
				}
				else
				{
					EffectSystem_Target->GetTransform()->SetLocalPosition({ 0, 0, 200 });
					Col_Attack->GetTransform()->SetLocalPosition({ 0, 0, 200 });
				}
				LockOnPosition = EffectSystem_Target->GetTransform()->GetWorldPosition();
				Col_Attack->GetTransform()->SetLocalScale({ 300, 300, 300 });
				Col_Attack->On();
				TimeEvent.AddEvent(0.08f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
					{
						Col_Attack->Off();
					});
				EffectSystem_Target->PlayFX("Yamato_JC_Effect.effect");
				EffectSystem->PlayFX("Yamato_JudgementCut_Air.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Air_JudgementCut_2_Loop");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheckFly()) { return; }
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCutAir_3);
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Vergil_yamato_JudgementCutAir_3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCutAir_3,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0300_yamato_Air_JudgementCut_3");
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Landing);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }

				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }

				if (true == Input_WarpCheckFly()) { return; }

			},
			.End = [=] {
			}
			});
		// JudgementCutEnd 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_1,
			.Start = [=] {
				Sound.Play("Yamato_", 15);
				Sound.Play("Yamato_", 17);
				Sound.PlayVoice(21, DTValue);
				RotationToTarget();
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				SetInvincibility(6.0f);
				EffectSystem->PlayFX("Yamato_JudgementCut_End_1.effect");
				Renderer->ChangeAnimation("pl0300_yamato_JudgementCutEnd_1");
				TimeEvent.AddEvent(1.58f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Sound.Play("Yamato_", 16);
					Renderer->Off();
					Effect_Color->SetStartColor(float4::ZERO);
					Effect_Color->SetEndColor({-0.15f, -0.15f, -0.11f});
					Effect_Color->SetSpeed({ 10.0f, 10.0f, 10.0f });
					Effect_Color->ColorEffectOn();
				});
				TimeEvent.AddEvent(1.7f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Col_Attack->GetTransform()->SetLocalPosition(float4::ZERO);
					Col_Attack->GetTransform()->SetLocalScale({ 2000, 2000, 2000 });
					Col_Attack->SetAttackData(DamageType::Stun, 0);
					Col_Attack->On();
				});
				TimeEvent.AddEvent(1.78f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Col_Attack->Off();
					PlayerCamera::Shake(10.0f, 0.5f, 0.1f, 0.1f);
				});
				TimeEvent.AddEvent(1.83f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Effect_JC->JudgementCutOn();
				});
				TimeEvent.AddEvent(2.8f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Rot.y += 180.0f;
					PhysXCapsule->AddWorldRotation({ 0, 180, 0 });
					GetTransform()->AddWorldRotation({ 0, 180, 0 });
					ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_2);
				});

				// 사운드
				TimeEvent.AddEvent(1.56f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
					{
						Sound.Play("Yamato_", 9);
					});
				TimeEvent.AddEvent(1.7f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
					{
						Sound.Play("Yamato_", 6);
					});
				TimeEvent.AddEvent(1.8f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
					{
						Sound.Play("Yamato_", 19);
					});
				TimeEvent.AddEvent(1.95f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
					{
						Sound.Play("Yamato_", 8);
					});

			},
			.Update = [=](float _DeltaTime) {
				PhysXCapsule->SetLinearVelocityZero();
			},
			.End = [=] {

			}
			});
		// Vergil_yamato_JudgementCutEnd_2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_2,
			.Start = [=] {
				InputCheck = false;
				MoveCheck = false;
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->On();
				EffectSystem->PlayFX("Yamato_JudgementCut_End_2.effect");
				Renderer->ChangeAnimation("pl0300_yamato_JudgementCutEnd_2");
				TimeEvent.AddEvent(1.82f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Sound.Play("Yamato_", 18);
					Sound.Play("Yamato_", 20);
					Effect_JC->JudgementCutOff();
					Effect_Color->SetStartColor(float4::ONE * 0.3f);
					Effect_Color->SetEndColor(float4::ZERO);
					Effect_Color->SetSpeed({ 10.0f, 10.0f, 10.0f });
					Effect_Color->ColorEffectOn();
					Col_Attack->SetAttackData(DamageType::Heavy, 3000);
					Col_Attack->On();
				});
				TimeEvent.AddEvent(1.9f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Col_Attack->Off();
				});
			},
			.Update = [=](float _DeltaTime) {
				PhysXCapsule->SetLinearVelocityZero();

				if (InputCheck == false) { return; }
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
	}
	{}
	/* 워프 */
	{
		static float4 WarpPos;

		// Warp Front 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Front_1,
		.Start = [=] {
			Sound.Play("Vergil_Warp");
			WeaponIdle();
			RotationToTarget();
			SetInvincibility(0.5f);
			IsEvade = true;
			PhysXCapsule->TurnOnGravity();
			PhysXCapsule->SetLinearVelocityZero();
			Renderer->ChangeAnimation("pl0300_Warp_Front_1", true);
			EffectSystem->PlayFX("Vergil_Warp_1.effect");
			TimeEvent.AddEvent(0.116f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Renderer->Off();
					EffectSystem->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
				});
			TimeEvent.AddEvent(0.3f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					EffectSystem->GetTransform()->SetLocalPosition(float4::ZERO);
					ChangeState(FSM_State_Vergil::Vergil_Warp_Front_2);
				});
			TimeEvent.AddEvent(0.35f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Renderer->On();
				});
			},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
			});
		// Warp Front 2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Front_2,
		.Start = [=] {
			SetFloorPos();
			EffectSystem->PlayFX("Vergil_Warp_2.effect");
			Renderer->ChangeAnimation("pl0300_Warp_Front_2");
			PhysXCapsule->TurnOffGravity();
			InputCheck = false;
			MoveCheck = false;
		},
		.Update = [=](float _DeltaTime) {
			if (true == Renderer->IsAnimationEnd())
			{
				ChangeState(FSM_State_Vergil::Vergil_Idle);
			}

			if (true == FloorCheck())
			{
				if (true == Input_SpecialCheck()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }
			}
			else
			{
				if (true == Input_SpecialCheckFly()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_WarpCheckFly()) { return; }
			}
			if (MoveCheck == false) { return; }
			if (false == FloorCheck())
			{
				ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
				return;
			}
			if (Controller->GetMoveVector() != float4::ZERO)
			{
				ChangeState(FSM_State_Vergil::Vergil_RunStart);
				return;
			}
		},
		.End = [=] {
			IsEvade = false;
		} });
		// Warp Back 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Back_1,
		.Start = [=] {
			Sound.Play("Vergil_Warp");
			WeaponIdle();
			RotationToTarget();
			SetInvincibility(0.5f);
			IsEvade = true;
			Renderer->ChangeAnimation("pl0300_Warp_Back_1");
			EffectSystem->PlayFX("Vergil_Warp_1.effect");
			PhysXCapsule->TurnOnGravity();
			PhysXCapsule->SetLinearVelocityZero();
			TimeEvent.AddEvent(0.116f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Renderer->Off();
					EffectSystem->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
				});
			TimeEvent.AddEvent(0.3f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					EffectSystem->GetTransform()->SetLocalPosition(float4::ZERO);
					ChangeState(FSM_State_Vergil::Vergil_Warp_Back_2);
				});
			TimeEvent.AddEvent(0.35f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Renderer->On();
				});
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
			});
		// Warp Back 2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Back_2,
		.Start = [=] {
			SetFloorPos();
			PhysXCapsule->TurnOnGravity();
			Renderer->ChangeAnimation("pl0300_Warp_Back_2");
			EffectSystem->PlayFX("Vergil_Warp_2.effect");
			PhysXCapsule->TurnOffGravity();
			InputCheck = false;
			MoveCheck = false;
		},
		.Update = [=](float _DeltaTime) {
			if (false == FloorCheck())
			{
				ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
				return;
			}

			if (true == Input_SpecialCheck()) { return; }

			if (InputCheck == false) { return; }
			if (true == Input_JumpCheck()) { return; }
			if (true == Input_SwordCheck()) { return; }
			if (true == Input_WarpCheck()) { return; }

			if (MoveCheck == false) { return; }
			if (Controller->GetMoveVector() != float4::ZERO)
			{
				ChangeState(FSM_State_Vergil::Vergil_RunStart);
				return;
			}
		},
		.End = [=] {
			IsEvade = false;
		}
			});
		// Warp Left 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Left_1,
		.Start = [=] {
			Sound.Play("Vergil_Warp");
			WeaponIdle();
			RotationToTarget();
			SetInvincibility(0.5f);
			IsEvade = true;
			Renderer->ChangeAnimation("pl0300_Warp_Left_1");
			EffectSystem->PlayFX("Vergil_Warp_1.effect");
			PhysXCapsule->TurnOnGravity();
			PhysXCapsule->SetLinearVelocityZero();
			TimeEvent.AddEvent(0.116f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Renderer->Off();
					EffectSystem->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
				});
			TimeEvent.AddEvent(0.3f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					EffectSystem->GetTransform()->SetLocalPosition(float4::ZERO);
					ChangeState(FSM_State_Vergil::Vergil_Warp_Left_2);
				});
			TimeEvent.AddEvent(0.35f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Renderer->On();
				});
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
			});
		// Warp Left 2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Left_2,
		.Start = [=] {
			SetFloorPos();
			RotationToTarget();
			Renderer->ChangeAnimation("pl0300_Warp_Left_2");
			EffectSystem->PlayFX("Vergil_Warp_2.effect");
			PhysXCapsule->TurnOffGravity();
			InputCheck = false;
			MoveCheck = false;
		},
		.Update = [=](float _DeltaTime) {
			if (false == FloorCheck())
			{
				ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
				return;
			}

			if (true == Input_SpecialCheck()) { return; }

			if (InputCheck == false) { return; }
			if (true == Input_JumpCheck()) { return; }
			if (true == Input_SwordCheck()) { return; }
			if (true == Input_WarpCheck()) { return; }

			if (MoveCheck == false) { return; }
			if (Controller->GetMoveVector() != float4::ZERO)
			{
				ChangeState(FSM_State_Vergil::Vergil_RunStart);
				return;
			}
		},
		.End = [=] {
			IsEvade = false;
		}
			});
		// Warp Right 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Right_1,
		.Start = [=] {
			Sound.Play("Vergil_Warp");
			WeaponIdle();
			RotationToTarget();
			SetInvincibility(0.5f);
			IsEvade = true;
			Renderer->ChangeAnimation("pl0300_Warp_Right_1", true);
			EffectSystem->PlayFX("Vergil_Warp_1.effect");
			PhysXCapsule->TurnOnGravity();
			PhysXCapsule->SetLinearVelocityZero();
			TimeEvent.AddEvent(0.116f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Renderer->Off();
					EffectSystem->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
				});
			TimeEvent.AddEvent(0.3f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					EffectSystem->GetTransform()->SetLocalPosition(float4::ZERO);
					ChangeState(FSM_State_Vergil::Vergil_Warp_Right_2);
				});
			TimeEvent.AddEvent(0.35f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Renderer->On();
				});

			},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
			});
		// Warp Right 2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Right_2,
		.Start = [=] {
			SetFloorPos();
			RotationToTarget();
			Renderer->ChangeAnimation("pl0300_Warp_Right_2");
			EffectSystem->PlayFX("Vergil_Warp_2.effect");
			PhysXCapsule->TurnOffGravity();
			InputCheck = false;
			MoveCheck = false;
		},
		.Update = [=](float _DeltaTime) {
			if (false == FloorCheck())
			{
				ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
				return;
			}

			if (true == Input_SpecialCheck()) { return; }

			if (InputCheck == false) { return; }
			if (true == Input_JumpCheck()) { return; }
			if (true == Input_SwordCheck()) { return; }
			if (true == Input_WarpCheck()) { return; }

			if (MoveCheck == false) { return; }
			if (Controller->GetMoveVector() != float4::ZERO)
			{
				ChangeState(FSM_State_Vergil::Vergil_RunStart);
				return;
			}
		},
		.End = [=] {
			IsEvade = false;
		} });

		// Warp Up
		static float4 BeforePos;
		static GameEngineTransform* WarpTarget;
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_AirTrick,
		.Start = [=] {
			Sound.Play("Vergil_Warp");
			WeaponIdle();
			PhysXCapsule->TurnOffGravity();
			PhysXCapsule->SetLinearVelocityZero();
			SetInvincibility(0.5f);
			WarpPos = float4::ZERO;
			if (nullptr != LockOnEnemyTransform)
			{
				WarpTarget = LockOnEnemyTransform;
				RotationToTarget();
				WarpPos = LockOnEnemyTransform->GetWorldPosition();
				BeforePos = WarpPos;
				float4 _Dir = GetTransform()->GetWorldPosition() - LockOnEnemyTransform->GetWorldPosition();
				_Dir.y = 0;
				WarpPos = WarpPos + _Dir.NormalizeReturn() * 150;
			}
			else
			{
				WarpTarget = nullptr;
				WarpPos = GetTransform()->GetWorldPosition();
			}
			Renderer->ChangeAnimation("pl0300_Warp_Up", true);

			EffectSystem->PlayFX("Vergil_Warp_1.effect");
			TimeEvent.AddEvent(0.116f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
			{
				Renderer->Off();
				EffectSystem->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
				if (nullptr != WarpTarget)
				{
					WarpPos += (WarpTarget->GetWorldPosition() - BeforePos) * 1.5f;
				}
				SetWorldPosition(WarpPos);
			});
			TimeEvent.AddEvent(0.3f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
			{
				EffectSystem->GetTransform()->SetLocalPosition(float4::ZERO);
				ChangeState(FSM_State_Vergil::Vergil_Warp_Front_2);
			});
			TimeEvent.AddEvent(0.35f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
			{
				Renderer->On();
			});

			},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {
			PhysXCapsule->TurnOnGravity();
		}
			});

		// Warp Down
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_TrickDown,
		.Start = [=] {
			Sound.Play("Vergil_Warp");
			WeaponIdle();
			SetInvincibility(0.5f);
			PhysXCapsule->TurnOffGravity();
			PhysXCapsule->SetLinearVelocityZero();
			float4 Dir;
			if (nullptr != LockOnEnemyTransform)
			{
				RotationToTarget();
				Dir = GetTransform()->GetWorldPosition() - LockOnEnemyTransform->GetWorldPosition();
				Dir.y = 0;
				Dir = Dir.NormalizeReturn() * 0.3f + float4::DOWN;
			}
			else
			{
				Dir = GetTransform()->GetWorldBackVector() * 0.3f + float4::DOWN;
			}
			if (true == GetLevel()->RayCast(GetTransform()->GetWorldPosition(), Dir.NormalizeReturn(), WarpPos, 9999.0f))
			{
				WarpPos.y += 75;
			}
			else
			{
				WarpPos = GetTransform()->GetWorldPosition();
			}
			Renderer->ChangeAnimation("pl0300_Warp_Down", true);
			EffectSystem->PlayFX("Vergil_Warp_1.effect");
			TimeEvent.AddEvent(0.116f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Renderer->Off();
					SetWorldPosition(WarpPos);
				});
			TimeEvent.AddEvent(0.3f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					ChangeState(FSM_State_Vergil::Vergil_Warp_Back_2);
				});
			TimeEvent.AddEvent(0.35f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Renderer->On();
				});
			},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {
			SetWorldPosition(WarpPos);
		}
			});
	}
	/* 데빌 트리거 */
	{}
	{
		// Demon Start
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_DT_Start,
		.Start = [=] {
			WeaponIdle();
			HUD->GetDtUI()->SetTransValue(true);
			Sound.Play("DT_On");
			Sound.PlayVoice(31, true);
			Sound.NoSkipOn();
			SetInvincibility(0.5f);
			PhysXCapsule->TurnOffGravity();
			PhysXCapsule->SetLinearVelocityZero();
			Col_Attack->SetAttackData(DamageType::Air, 150);
			Col_Attack->SetDamageSoundType(DamageSoundType::Magic);
			EffectSystem->PlayFX("Vergil_DT_On.effect");
			Renderer->ChangeAnimation("pl0300_Demon_Start", true);
			InputCheck = false;
			MoveCheck = false;
			},
		.Update = [=](float _DeltaTime) {
			if (false == FloorCheck())
			{
				ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
				return;
			}

			if (true == Input_SpecialCheck()) { return; }

			if (InputCheck == false) { return; }
			if (true == Input_JumpCheck()) { return; }
			if (true == Input_SwordCheck()) { return; }
			if (true == Input_WarpCheck()) { return; }

			if (MoveCheck == false) { return; }
			if (Controller->GetMoveVector() != float4::ZERO)
			{
				ChangeState(FSM_State_Vergil::Vergil_RunStart);
				return;
			}
		},
		.End = [=] {
			Col_Attack->SetDamageSoundType(DamageSoundType::Katana);
		}
			});
		// Demon End
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_DT_End,
		.Start = [=] {
			WeaponIdle();
			PhysXCapsule->TurnOffGravity();
			PhysXCapsule->SetLinearVelocityZero();
			Renderer->ChangeAnimation("pl0300_Demon_End", true);
			InputCheck = false;
			MoveCheck = false;
			},
		.Update = [=](float _DeltaTime) {
			if (false == FloorCheck())
			{
				ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
				return;
			}

			if (true == Input_SpecialCheck()) { return; }

			if (InputCheck == false) { return; }
			if (true == Input_JumpCheck()) { return; }
			if (true == Input_SwordCheck()) { return; }
			if (true == Input_WarpCheck()) { return; }

			if (MoveCheck == false) { return; }
			if (Controller->GetMoveVector() != float4::ZERO)
			{
				ChangeState(FSM_State_Vergil::Vergil_RunStart);
				return;
			}
		},
		.End = [=] {

		}
			});
	}
	/* 대미지 */
	{
		// Damage Light
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Damage_Light,
			.Start = [=] {
				Sound.PlayVoiceRandom(22, 24, DTValue);
				SetInvincibility(0.2f);
				WeaponIdle();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				EffectSystem->PlayFX("Damage_Light.effect");
				Renderer->ChangeAnimation("pl0300_Damage_Common", true);
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_Idle);
					return;
				}
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
			}
			});

		// Damage Heavy
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Damage_Heavy,
			.Start = [=] {
				Sound.PlayVoiceRandom(24, 25, DTValue);
				SetInvincibility(0.5f);
				WeaponIdle();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				EffectSystem->PlayFX("Damage_Heavy.effect");
				Renderer->ChangeAnimation("pl0300_damage_Away", true);
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_Damage_Fall);
					return;
				}
				if (InputCheck == false) { return; }
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Damage_Ground);
					return;
				}
			},
			.End = [=] {
			}
			});

		// Damage Fly
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Damage_Fly,
			.Start = [=] {
				Sound.PlayVoiceRandom(24, 25, DTValue);
				SetInvincibility(9999.0f);
				WeaponIdle();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				EffectSystem->PlayFX("Damage_Heavy.effect");
				Renderer->ChangeAnimation("pl0300_Damage_Air", true);
			},
			.Update = [=](float _DeltaTime) {
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_Damage_Fall);
					return;
				}
				if (true == FloorCheck())
				{
					if (HP <= 0)
					{
						ChangeState(FSM_State_Vergil::Vergil_Damage_Fly_Death);
						return;
					}
					ChangeState(FSM_State_Vergil::Vergil_Damage_Ground);
					return;
				}
			},
			.End = [=] {
			}
			});

		// Damage Fall
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Damage_Fall,
			.Start = [=] {
				SetInvincibility(9999.0f);
				PhysXCapsule->SetPush(float4::DOWN * FlyDownForce);
				PhysXCapsule->TurnOnGravity();
				Renderer->ChangeAnimation("pl0300_Damage_Away_Loop");
			},
			.Update = [=](float _DeltaTime) {
				if (true == FloorCheck())
				{
					if (HP <= 0)
					{
						ChangeState(FSM_State_Vergil::Vergil_Damage_Fly_Death);
						return;
					}
					ChangeState(FSM_State_Vergil::Vergil_Damage_Ground);
					return;
				}
			},
			.End = [=] {
			}
			});

		// Damage Ground
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Damage_Ground,
			.Start = [=] {
				SetInvincibility(0.5f);
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0300_Damage_Away_Bound");
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (true == Renderer->IsAnimationEnd())
				{
					Sound.PlayVoice(27, DTValue);
					ChangeState(FSM_State_Vergil::Vergil_Idle);
					return;
				}
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }
			},
			.End = [=] {
			}
			});

		// Damage Death
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Damage_Death,
			.Start = [=] {
				IsDeath = true;
				Sound.PlayVoice(28, DTValue);
				SetInvincibility(9999.0f);
				WeaponIdle();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0300_damage_Death", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage Fly Death
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Damage_Fly_Death,
			.Start = [=] {
				IsDeath = true;
				Sound.PlayVoice(28, DTValue);
				SetInvincibility(9999.0f);
				WeaponIdle();
				Renderer->GetAllRenderUnit()[0][5]->Off();
				Renderer->GetAllRenderUnit()[0][6]->Off();
				Renderer->GetAllRenderUnit()[0][7]->Off();
				Renderer->GetAllRenderUnit()[0][8]->Off();
				Renderer->ChangeAnimation("pl0300_damage_Away_Death", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				Renderer->GetAllRenderUnit()[0][5]->On();
			}
			});
	}

	// Provocation
	FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Provocation,
		.Start = [=] {
			Sound.PlayVoiceRandom(29, 30, DTValue);
			WeaponIdle();
			PhysXCapsule->SetLinearVelocityZero();
			PhysXCapsule->TurnOffGravity();
			Renderer->ChangeAnimation("pl0300_Provocation", true);
		},
		.Update = [=](float _DeltaTime) {
			if (true == Renderer->IsAnimationEnd())
			{
				ChangeState(FSM_State_Vergil::Vergil_Idle);
				return;
			}
			if (false == FloorCheck())
			{
				ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
				return;
			}

			if (true == Input_SpecialCheck()) { return; }
			if (true == Input_JumpCheck()) { return; }
			if (true == Input_SwordCheck()) { return; }
			if (true == Input_WarpCheck()) { return; }
		},
		.End = [=] {
		}
		});

	ChangeState(FSM_State_Vergil::Vergil_Idle);

	CreateMirageBlade();
}

void PlayerActor_Vergil::SoundLoad()
{
	if (nullptr != GameEngineSound::Find("Vergil_V_0.wav")) { return; }

	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Sound");
	NewDir.Move("Voice");
	NewDir.Move("Vergil");
	std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".wav" });
	for (GameEngineFile File : Files)
	{
		GameEngineSound::Load(File.GetFullPath());
	}
	NewDir.MoveParent();
	NewDir.MoveParent();
	NewDir.Move("SFX");
	NewDir.Move("Vergil");
	Files = NewDir.GetAllFile({ ".wav" });
	for (GameEngineFile File : Files)
	{
		GameEngineSound::Load(File.GetFullPath());
	}

	if (nullptr != GameEngineSound::Find("FootStep_0.wav")) { return; }
	NewDir.MoveParent();
	NewDir.Move("Player");
	Files = NewDir.GetAllFile({ ".wav" });
	for (GameEngineFile File : Files)
	{
		GameEngineSound::Load(File.GetFullPath());
	}
}

void PlayerActor_Vergil::Update_Character(float _DeltaTime)
{
	if (LoadCheck == false) { return; }
	FSM.Update(_DeltaTime);
	if (true == IsDeath) { return; }
	if (NetControllType::ActiveControll == GameEngineNetObject::GetControllType())
	{
		if (true == DTValue && IsDeath == false)
		{
			HP = std::clamp(static_cast<int>(HP + 500 * _DeltaTime), 0, MaxHP);
			HUD->SetPlayerHP(HP);
		}
		FSM_MirageBlade.Update(_DeltaTime);
	}
}

void PlayerActor_Vergil::LightDamage()
{
	HUD->SetPlayerHP(HP);
	if (true == FloorCheck())
	{
		if (HP <= 0)
		{
			ChangeState(FSM_State_Vergil::Vergil_Damage_Death);
			return;
		}
		ChangeState(FSM_State_Vergil::Vergil_Damage_Light);
		return;
	}
	else
	{
		ChangeState(FSM_State_Vergil::Vergil_Damage_Fly);
	}
}

void PlayerActor_Vergil::HeavyDamage()
{
	HUD->SetPlayerHP(HP);
	if (true == FloorCheck())
	{
		if (HP <= 0)
		{
			ChangeState(FSM_State_Vergil::Vergil_Damage_Death);
			return;
		}
		ChangeState(FSM_State_Vergil::Vergil_Damage_Heavy);
	}
	else
	{
		ChangeState(FSM_State_Vergil::Vergil_Damage_Fly);
	}
}

void PlayerActor_Vergil::AddDTGauge(float _Value)
{
	if (true == DTValue && 0 < _Value) { return; }
	if (false == DTValue && 0 >= _Value) { return; }

	DTGauge = std::clamp(DTGauge + _Value, 0.0f, 10.0f);
	HUD->GetDtUI()->ActivateDtUI(DTGauge);
	if (DTGauge <= 0.0f)
	{
		HUD->GetDtUI()->SetTransValue(false);
		Sound.Play("DT_Off");
		DTValue = false;
		DTOffEffect->Play();
		SetHuman();
	}
}

void PlayerActor_Vergil::ShopOn()
{
	HUD->Off();
}

void PlayerActor_Vergil::ShopOff()
{
	HUD->On();
	IsRapidSlash = Shop_VergilYamatoUI::IsRapidSlash;
	IsUpperSlash = Shop_VergilYamatoUI::IsUpperSlash;
	IsAerialRave = Shop_VergilYamatoUI::IsAerialRave;
	IsYamatoCombo = Shop_VergilYamatoUI::IsYamatoCombo;
	IsJudgmentCutEnd = Shop_VergilYamatoUI::IsJudgmentCutEnd;
	IsSpiralBlade = Shop_VergilMirgeUI::IsSpiralBlade;
	IsStormBlade = Shop_VergilMirgeUI::IsStormBlade;
	IsLesteringBlade = Shop_VergilMirgeUI::IsLesteringBlade;
	IsHeavyRainBlade = Shop_VergilMirgeUI::IsHeavyRainBlade;
}

void PlayerActor_Vergil::ChangeState(int _StateValue)
{
	FSM.ChangeState(_StateValue);
	FSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
}

bool PlayerActor_Vergil::Input_SwordCheck(int AddState)
{
	if (Controller->GetIsSwordChargeUp())
	{
		if (FSM_State_Vergil::Vergil_yamato_JudgementCut_3 == FSMValue)
		{
			ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCut_2);
			return true;
		}
		ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCut_1);
		return true;
	}
	if (false == Controller->GetIsLockOn())
	{
		if (Controller->GetIsAnySword())
		{
			ChangeState(AddState + FSM_State_Vergil::Vergil_yamato_Combo_1);
			return true;
		}
		return false;
	}
	if (Controller->GetIsFrontSword() && true == IsRapidSlash)
	{
		ChangeState(FSM_State_Vergil::pl0300_yamato_Sissonal_1);
		return true;
	}
	if (Controller->GetIsBackSword() && true == IsUpperSlash)
	{
		ChangeState(FSM_State_Vergil::pl0300_yamato_Upper_1);
		return true;
	}
	if (Controller->GetIsSword())
	{
		ChangeState(AddState + FSM_State_Vergil::Vergil_yamato_Combo_1);
		return true;
	}
	return false;
}

bool PlayerActor_Vergil::Input_SwordCheckFly(int AddState)
{
	if (Controller->GetIsSwordChargeUp())
	{
		if (FSM_State_Vergil::Vergil_yamato_JudgementCutAir_3 == FSMValue)
		{
			ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCutAir_2);
			return true;
		}
		ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCutAir_1);
		return true;
	}
	if (false == Controller->GetIsLockOn())
	{
		if (Controller->GetIsAnySword())
		{
			ChangeState(AddState + FSM_State_Vergil::Vergil_yamato_Air_Combo_1);
			return true;
		}
		return false;
	}
	if (Controller->GetIsBackSword() && true == IsAerialRave)
	{
		ChangeState(FSM_State_Vergil::Vergil_yamato_Raid1);
		return true;
	}
	if (Controller->GetIsSword())
	{
		ChangeState(AddState + FSM_State_Vergil::Vergil_yamato_Air_Combo_1);
		return true;
	}
	return false;
}

bool PlayerActor_Vergil::Input_JumpCheck()
{
	if (Controller->GetIsProvocation())
	{
		ChangeState(FSM_State_Vergil::Vergil_Provocation);
		return true;
	}

	if (false == Controller->GetIsLockOn())
	{
		if (Controller->GetIsAnyJump())
		{
			ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
			return true;
		}
		return false;
	}


	if (Controller->GetIsLeftJump())
	{
		ChangeState(FSM_State_Vergil::Vergil_Warp_Left_1);
		return true;
	}
	if (Controller->GetIsRightJump())
	{
		ChangeState(FSM_State_Vergil::Vergil_Warp_Right_1);
		return true;
	}
	if (Controller->GetIsJump())
	{
		ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
		return true;
	}
	return false;
}

bool PlayerActor_Vergil::Input_JumpCheckFly()
{
	return false;
}

bool PlayerActor_Vergil::Input_WarpCheck()
{
	if (true == Controller->GetIsSkill())
	{
		char Dir = Controller->MoveVectorToChar4(Controller->GetMoveVector());
		if (Dir == '4')
		{
			ChangeState(FSM_State_Vergil::Vergil_Warp_Left_1);
		}
		else if (Dir == '6')
		{
			ChangeState(FSM_State_Vergil::Vergil_Warp_Right_1);
		}
		else if (Dir == '2')
		{
			ChangeState(FSM_State_Vergil::Vergil_Warp_Back_1);
		}
		else
		{
			ChangeState(FSM_State_Vergil::Vergil_Warp_Front_1);
		}
	}
	return false;
}

bool PlayerActor_Vergil::Input_WarpCheckFly()
{
	return false;
}

bool PlayerActor_Vergil::Input_SpecialCheck()
{
	if (Controller->GetIsDevilTrigger())
	{
		if (false == DTValue)
		{
			if (3 <= DTGauge)
			{
				ChangeState(FSM_State_Vergil::Vergil_DT_Start);
				return true;
			}
		}
		else
		{
			HUD->GetDtUI()->SetTransValue(false);
			DTOffEffect->Play();
			Sound.Play("DT_Off");
			ChangeState(FSM_State_Vergil::Vergil_DT_End);
			return true;
		}
	}
	if (Controller->GetIsSpecialMove() && true == IsJudgmentCutEnd && 3.0f < DTGauge)
	{
		if (false == IsGod)
		{
			DTGauge -= 3.0f;
			HUD->GetDtUI()->ActivateDtUI(DTGauge);
		}
		ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_1);
		return true;
	}
	if (Controller->GetIsBackFrontSkill())
	{
		ChangeState(FSM_State_Vergil::Vergil_Warp_Back_1);
		return true;
	}
	if (Controller->GetIsLockOnSkill())
	{
		ChangeState(FSM_State_Vergil::Vergil_Warp_AirTrick);
		return true;
	}
	return false;
}

bool PlayerActor_Vergil::Input_SpecialCheckFly()
{
	if (Controller->GetIsDevilTrigger())
	{
		if (true == DTValue)
		{
			HUD->GetDtUI()->SetTransValue(false);
			DTOffEffect->Play();
			Sound.Play("DT_Off");
			SetHuman();
			return true;
		}
	}

	if (Controller->GetIsAnyJump())
	{
		std::vector<std::shared_ptr<GameEngineCollision>> Cols;
		if (true == Col_EnemyStepCheck->CollisionAll(CollisionOrder::Enemy, Cols))
		{
			Sound.Play("EnemyStep");
			ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
			return true;
		}
	}
	if (Controller->GetIsBackFrontSkill())
	{
		ChangeState(FSM_State_Vergil::Vergil_Warp_TrickDown);
		return true;
	}
	if (Controller->GetIsLockOnSkill())
	{
		ChangeState(FSM_State_Vergil::Vergil_Warp_AirTrick);
		return true;
	}
	return false;
}

void PlayerActor_Vergil::SetHuman()
{
	DTValue = 0;
	for (int i = 9; i <= 21; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->On();
	}
	for (int i = 2; i <= 4; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->Off();
	}
	Renderer->GetAllRenderUnit()[0][22]->Off();
}

void PlayerActor_Vergil::SetDemon()
{
	DTValue = 1;
	for (int i = 9; i <= 21; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->Off();
	}
	for (int i = 2; i <= 4; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->On();
	}
	Renderer->GetAllRenderUnit()[0][22]->On();

}

void PlayerActor_Vergil::WeaponIdle()
{
	Renderer->GetAllRenderUnit()[0][0]->Off();
	Renderer->GetAllRenderUnit()[0][1]->Off();
	Renderer->GetAllRenderUnit()[0][6]->On();
	Renderer->GetAllRenderUnit()[0][7]->On();
	Renderer->GetAllRenderUnit()[0][8]->On();

	if (nullptr != Col_Attack)
	{
		Col_Attack->Off();
	}
	if (nullptr != EffectSystem)
	{
		EffectSystem->Off();
	}
}

void PlayerActor_Vergil::YamatoOff()
{
	Renderer->GetAllRenderUnit()[0][0]->Off();
	Renderer->GetAllRenderUnit()[0][1]->Off();
	Renderer->GetAllRenderUnit()[0][6]->On();
	Renderer->GetAllRenderUnit()[0][7]->On();
	Renderer->GetAllRenderUnit()[0][8]->On();
}

void PlayerActor_Vergil::YamatoOn()
{
	Renderer->GetAllRenderUnit()[0][0]->On();
	Renderer->GetAllRenderUnit()[0][1]->On();

	Renderer->GetAllRenderUnit()[0][6]->Off();
	Renderer->GetAllRenderUnit()[0][7]->Off();
	Renderer->GetAllRenderUnit()[0][8]->Off();
}

int PlayerActor_Vergil::DamageCalculate(int _Damage)
{
	if (true == DTValue)
	{
		return static_cast<int>(_Damage * 1.3f);
	}
	else
	{
		return _Damage;
	}
}

void PlayerActor_Vergil::DrawEditor()
{
	static float4 WarpPos = { 3645, 130, -11805 };
	ImGui::InputFloat4("Warp Pos", WarpPos.Arr1D);

	if (ImGui::Button("Warp"))
	{
		SetWorldPosition(WarpPos);
	}

	if (ImGui::Button("Add DTGauge"))
	{
		AddDTGauge(10.0f);
	}
}


/*
버질 렌더 유닛
0 오른손 야마토
1 오른손 야마토
2 DT 뿔
3 DT 몸
4 DT 뿔
5 왼손 야마토 검집
6 왼손 야마토
7 왼손 야마토
8 왼손 야마토

9 ~ 21 인간
22 날개
*/