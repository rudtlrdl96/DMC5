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

void PlayerActor_Vergil::NetLoad()
{
	if (true == LoadCheck) { return; }

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
		Renderer->GetTransform()->SetLocalRotation({ 0, 0, 0 });
		Renderer->GetTransform()->SetLocalPosition({ 0, -75, 0 });

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
			Renderer->SetSpecularTexture("pl0300_03_albm.texout.png", "pl0300_03_atos.texout.png");
			Renderer->SetSpecularTexture("pl0310_trans_00_alba.texout.png", "pl0310_trans_atos.texout.png");
			Renderer->SetSpecularTexture("pl0310_trans_01_alba.texout.png", "pl0310_trans_atos.texout.png");
		}
		break;
		default:
			break;
		}

		Renderer->ShadowOn();
		Renderer->SetDynamic();

		Renderer->SetMaterial("pl0310_trans_00_alba.texout.png", "AniFBX_2DAlpha", nullptr);
		Renderer->SetMaterial("pl0310_trans_01_alba.texout.png", "AniFBX_2DAlpha", nullptr);

		AnimationEvent::LoadAll({ .Dir = NewDir.GetFullPath().c_str(), .Renderer = Renderer, .RendererLocalPos = {0, -75, 0},
			//.Objects = { (GameEngineObject*)Col_Attack.get() },
			.CallBacks_void = {
				nullptr,			// 0
				std::bind(&PlayerActor_Vergil::YamatoOn, this),
				std::bind(&PlayerActor_Vergil::YamatoOff, this),
				std::bind(&PlayerActor_Vergil::SetHuman, this),
				std::bind(&PlayerActor_Vergil::SetDemon, this)
			},
			.CallBacks_int = {
				std::bind(&GameEngineFSM::ChangeState, &FSM, std::placeholders::_1),
				std::bind(&SoundController::Play, &Sound, "Yamato_", std::placeholders::_1),
			},
			});
		SetHuman();
		WeaponIdle();
	}
	Col_Attack->Off();

	/* 기본 움직임 */
	{
		// Idle
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Idle,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_Idle_Normal");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Idle LockOn
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_IdleLockOn,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_Idle_Normal");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Walk
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Walk,
			.Start = [=] {
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Walk Front
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_WalkFront,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_Walk_Front");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Walk Left
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_WalkLeft,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_Walk_Left");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Walk Back
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_WalkBack,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_Walk_Back");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Walk Right
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_WalkRight,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_Walk_Right");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Run Start
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_RunStart,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_Run_Start");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Run
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Run,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_Run_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// RunStop
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_RunStop,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_Run_Stop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Jump Vertical
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Jump_Vertical,
		.Start = [=] {
			Sound.Play("Jump");
			Renderer->ChangeAnimation("pl0300_Jump_Vertical");
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
			});
		// Jump Fly
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Jump_Fly,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_Jump_Vertical_Fly");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Landing
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Landing,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_Jump_Vertical_Landing");
			},
			.Update = [=](float _DeltaTime) {

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
				EffectSystem->PlayFX("Yamato_Combo_1.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Combo_1");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Combo2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Combo_2,
			.Start = [=] {
				Sound.Play("Yamato_", 1);
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(93));
				EffectSystem->PlayFX("Yamato_Combo_2.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Combo_2");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Combo3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Combo_3,
			.Start = [=] {
				Sound.PlayVoiceRandom(3, 5, DTValue);
				Col_Attack->SetAttackData(DamageType::VergilLight, DamageCalculate(103));
				EffectSystem->PlayFX("Yamato_Combo_3.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Combo_3");
			},
			.Update = [=](float _DeltaTime) {
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
				EffectSystem->PlayFX("Yamato_Combo_4.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Combo_4");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// ComboC1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_ComboC_1,
			.Start = [=] {
				Sound.PlayVoiceRandom(2, 3, DTValue);
				YamatoOn();
				Renderer->ChangeAnimation("pl0300_yamato_ComboC_1");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// ComboC2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_ComboC_2,
			.Start = [=] {
				Col_Attack->SetAttackData(DamageType::VergilLight, DamageCalculate(28));
				EffectSystem->PlayFX("Yamato_Combo_C_2.effect");
				Renderer->ChangeAnimation("pl0300_yamato_ComboC_2_Loop", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// ComboC3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_ComboC_3,
			.Start = [=] {
				Sound.PlayVoiceRandom(7, 9, DTValue);
				Col_Attack->SetAttackData(DamageType::Heavy, DamageCalculate(300));
				EffectSystem->PlayFX("Yamato_Combo_C_3.effect");
				Renderer->ChangeAnimation("pl0300_yamato_ComboC_3");
			},
			.Update = [=](float _DeltaTime) {
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
				EffectSystem->PlayFX("Yamato_Sissonal_1.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Sissonal_1");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Sissonal2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Sissonal_2,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_yamato_Sissonal_2_loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// Sissonal3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Sissonal_3,
			.Start = [=] {
				Sound.Play("Yamato_", 7);
				Sound.PlayVoiceRandom(16, 18, DTValue);
				Sound.NoSkipOn();
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(600));
				EffectSystem->PlayFX("Yamato_Sissonal_3.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Sissonal_3");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Sissonal Up
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Sissonal_Up,
			.Start = [=] {
				Sound.PlayVoiceRandom(18, 19, DTValue);
				Sound.NoSkipOn();
				Col_Attack->SetAttackData(DamageType::Air, DamageCalculate(150));
				TimeEvent.AddEvent(0.5f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(131));
				});
				EffectSystem->PlayFX("Yamato_Sissonal_Up.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Sissonal_Up");
			},
			.Update = [=](float _DeltaTime) {
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
				Renderer->ChangeAnimation("pl0300_yamato_AttackUp_1");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// Upper 2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Upper_2,
			.Start = [=] {
				Sound.Play("Yamato_", 6);
				YamatoOn();
				Col_Attack->SetAttackData(DamageType::Air, DamageCalculate(168));
				EffectSystem->PlayFX("Yamato_AttackUp_2.effect");
				Renderer->ChangeAnimation("pl0300_yamato_AttackUp_2");
			},
			.Update = [=](float _DeltaTime) {
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
				EffectSystem->PlayFX("Yamato_AttackUp_3.effect");
				Renderer->ChangeAnimation("pl0300_yamato_AttackUp_3");
			},
			.Update = [=](float _DeltaTime) {
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
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(93));
				EffectSystem->PlayFX("Yamato_Air_Combo_1.effect");
				Renderer->ChangeAnimation("pl0300_yamato_AirCombo_1");
			},
			.Update = [=](float _DeltaTime) {
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
				EffectSystem->PlayFX("Yamato_Air_Combo_2.effect");
				Renderer->ChangeAnimation("pl0300_yamato_AirCombo_2");
			},
			.Update = [=](float _DeltaTime) {
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
				EffectSystem->PlayFX("Yamato_Air_Combo_3.effect");
				Renderer->ChangeAnimation("pl0300_yamato_AirCombo_3");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Air Combo B 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Air_ComboB_1,
			.Start = [=] {
				Sound.PlayVoiceRandom(5, 6, DTValue);
				YamatoOn();
				Col_Attack->SetAttackData(DamageType::Air, DamageCalculate(375));
				EffectSystem->PlayFX("Yamato_Air_ComboB_1.effect");
				Renderer->ChangeAnimation("pl0300_yamato_AirComboB_1");
			},
			.Update = [=](float _DeltaTime) {
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
				EffectSystem->PlayFX("Yamato_Air_Combo_3.effect");
				Renderer->ChangeAnimation("pl0300_yamato_AirComboB_2");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Raid1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Raid1,
			.Start = [=] {
				Sound.PlayVoiceRandom(12, 13, DTValue);
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_yamato_Raid1");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Raid2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Raid2,
			.Start = [=] {
				Col_Attack->SetAttackData(DamageType::Slam, DamageCalculate(525));
				YamatoOn();
				Renderer->ChangeAnimation("pl0300_yamato_Raid2_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Raid3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Raid3,
			.Start = [=] {
				Sound.Play("Yamato_", 8);
				YamatoOn();
				EffectSystem->PlayFX("Yamato_Raid_3.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Raid3");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});

		// JudgementCut 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCut_1,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_yamato_JudgementCut_1");
			},
			.Update = [=](float _DeltaTime) {
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
				EffectSystem_Target->GetTransform()->SetWorldPosition(LockOnPosition);
				EffectSystem_Target->PlayFX("Yamato_JC_Effect.effect");
				Renderer->ChangeAnimation("pl0300_yamato_JudgementCut_2_Loop");
			},
			.Update = [=](float _DeltaTime) {
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
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// Vergil_yamato_JudgementCutAir_1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCutAir_1,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_yamato_Air_JudgementCut_1");
			},
			.Update = [=](float _DeltaTime) {
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
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(450));
				EffectSystem_Target->GetTransform()->SetWorldPosition(LockOnPosition);
				EffectSystem_Target->PlayFX("Yamato_JC_Effect.effect");
				EffectSystem->PlayFX("Yamato_JudgementCut_Air.effect");
				Renderer->ChangeAnimation("pl0300_yamato_Air_JudgementCut_2_Loop");
			},
			.Update = [=](float _DeltaTime) {
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
			},
			.End = [=] {
			}
			});
		// JudgementCutEnd 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_1,
			.Start = [=] {
				Col_Attack->SetAttackData(DamageType::Stun, 0);
				Sound.Play("Yamato_", 15);
				Sound.Play("Yamato_", 17);
				Sound.PlayVoice(21, DTValue);
				EffectSystem->PlayFX("Yamato_JudgementCut_End_1.effect");
				Renderer->ChangeAnimation("pl0300_yamato_JudgementCutEnd_1");
				TimeEvent.AddEvent(1.58f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					BGMPlayer::SetBGMVolume(0.3f);
					Sound.Play("Yamato_", 16);
					Renderer->Off();
					Effect_Color->SetStartColor(float4::ZERO);
					Effect_Color->SetEndColor({-0.15f, -0.15f, -0.11f});
					Effect_Color->SetSpeed({ 10.0f, 10.0f, 10.0f });
					Effect_Color->ColorEffectOn();
				});
				TimeEvent.AddEvent(1.83f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Effect_JC->JudgementCutOn();
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
			},
			.End = [=] {

			}
			});
		// Vergil_yamato_JudgementCutEnd_2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_2,
			.Start = [=] {
				Col_Attack->SetAttackData(DamageType::Heavy, 3000);
				Renderer->On();
				EffectSystem->PlayFX("Yamato_JudgementCut_End_2.effect");
				Renderer->ChangeAnimation("pl0300_yamato_JudgementCutEnd_2");
				TimeEvent.AddEvent(1.82f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					BGMPlayer::SetBGMVolume(1.0f);
					Sound.Play("Yamato_", 18);
					Sound.Play("Yamato_", 20);
					Effect_JC->JudgementCutOff();
					Effect_Color->SetStartColor(float4::ONE * 0.3f);
					Effect_Color->SetEndColor(float4::ZERO);
					Effect_Color->SetSpeed({ 10.0f, 10.0f, 10.0f });
					Effect_Color->ColorEffectOn();
				});
			},
			.Update = [=](float _DeltaTime) {
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
			EffectSystem->PlayFX("Vergil_Warp_2.effect");
			Renderer->ChangeAnimation("pl0300_Warp_Front_2");
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		} });
		// Warp Back 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Back_1,
		.Start = [=] {
			Sound.Play("Vergil_Warp");
			WeaponIdle();
			Renderer->ChangeAnimation("pl0300_Warp_Back_1");
			EffectSystem->PlayFX("Vergil_Warp_1.effect");
			TimeEvent.AddEvent(0.116f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Renderer->Off();
					EffectSystem->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
				});
			TimeEvent.AddEvent(0.3f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					EffectSystem->GetTransform()->SetLocalPosition(float4::ZERO);
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
			Renderer->ChangeAnimation("pl0300_Warp_Back_2");
			EffectSystem->PlayFX("Vergil_Warp_2.effect");
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
			});
		// Warp Left 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Left_1,
		.Start = [=] {
			Sound.Play("Vergil_Warp");
			WeaponIdle();
			Renderer->ChangeAnimation("pl0300_Warp_Left_1");
			EffectSystem->PlayFX("Vergil_Warp_1.effect");
			TimeEvent.AddEvent(0.116f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Renderer->Off();
					EffectSystem->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
				});
			TimeEvent.AddEvent(0.3f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					EffectSystem->GetTransform()->SetLocalPosition(float4::ZERO);
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
			Renderer->ChangeAnimation("pl0300_Warp_Left_2");
			EffectSystem->PlayFX("Vergil_Warp_2.effect");
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
			});
		// Warp Right 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Right_1,
		.Start = [=] {
			Sound.Play("Vergil_Warp");
			WeaponIdle();
			Renderer->ChangeAnimation("pl0300_Warp_Right_1", true);
			EffectSystem->PlayFX("Vergil_Warp_1.effect");
			TimeEvent.AddEvent(0.116f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Renderer->Off();
					EffectSystem->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
				});
			TimeEvent.AddEvent(0.3f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					EffectSystem->GetTransform()->SetLocalPosition(float4::ZERO);
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
			Renderer->ChangeAnimation("pl0300_Warp_Right_2");
			EffectSystem->PlayFX("Vergil_Warp_2.effect");
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		} });

		// Warp Up
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_AirTrick,
		.Start = [=] {
			Sound.Play("Vergil_Warp");
			WeaponIdle();
			Renderer->ChangeAnimation("pl0300_Warp_Up", true);
			EffectSystem->PlayFX("Vergil_Warp_1.effect");
			TimeEvent.AddEvent(0.116f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
			{
				Renderer->Off();
				EffectSystem->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
			});
			TimeEvent.AddEvent(0.3f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
			{
				EffectSystem->GetTransform()->SetLocalPosition(float4::ZERO);
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

		// Warp Down
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_TrickDown,
		.Start = [=] {
			Sound.Play("Vergil_Warp");
			WeaponIdle();
			Renderer->ChangeAnimation("pl0300_Warp_Down", true);
			EffectSystem->PlayFX("Vergil_Warp_1.effect");
			TimeEvent.AddEvent(0.116f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Renderer->Off();
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
	}
	/* 데빌 트리거 */
	{}
	{
		// Demon Start
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_DT_Start,
		.Start = [=] {
			Sound.Play("DT_On");
			Sound.PlayVoice(31, true);
			Sound.NoSkipOn();
			EffectSystem->PlayFX("Vergil_DT_On.effect");
			Renderer->ChangeAnimation("pl0300_Demon_Start", true);
			},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
			});
		// Demon End
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_DT_End,
		.Start = [=] {
			Renderer->ChangeAnimation("pl0300_Demon_End", true);
			},
		.Update = [=](float _DeltaTime) {
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
				WeaponIdle();
				EffectSystem->PlayFX("Damage_Light.effect");
				Renderer->ChangeAnimation("pl0300_Damage_Common", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage Heavy
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Damage_Heavy,
			.Start = [=] {
				Sound.PlayVoiceRandom(24, 25, DTValue);
				WeaponIdle();
				EffectSystem->PlayFX("Damage_Heavy.effect");
				Renderer->ChangeAnimation("pl0300_damage_Away", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage Fly
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Damage_Fly,
			.Start = [=] {
				Sound.PlayVoiceRandom(24, 25, DTValue);
				WeaponIdle();
				EffectSystem->PlayFX("Damage_Heavy.effect");
				Renderer->ChangeAnimation("pl0300_Damage_Air", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage Fall
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Damage_Fall,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_Damage_Away_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage Ground
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Damage_Ground,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_Damage_Away_Bound");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage Death
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Damage_Death,
			.Start = [=] {
				Sound.PlayVoice(28, DTValue);
				WeaponIdle();
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
				Sound.PlayVoice(28, DTValue);
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
			Renderer->ChangeAnimation("pl0300_Provocation", true);
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {
		}
		});

	FSM.ChangeState(FSM_State_Vergil::Vergil_Idle);
}
