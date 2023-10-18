#include "PrecompileHeader.h"
#include "PlayerActor_Nero.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "AnimationEvent.h"
#include "EffectRenderer.h"
#include "PlayerController.h"
#include "NetworkManager.h"
#include "AttackCollision.h"
#include "BaseEnemyActor.h"
#include "NeroItemGlass.h"
#include "NeroHPUI.h"
#include "FXSystem.h"
#include "PlayerCamera.h"
#include "Player_Snatch.h"
#include "RankUI.h"
#include "Item_DevilBreaker.h"
#include "CavaliereAngelo.h"

void PlayerActor_Nero::NetLoad()
{
	// Effect 생성
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Effect");
		NewDir.Move("Nero");

		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".effect" });
		for (GameEngineFile File : Files)
		{
			if (nullptr == FXData::Find(File.GetFileName()))
			{
				FXData::Load(File.GetFullPath());
			}
			EffectSystem->CreateFX(FXData::Find(File.GetFileName()));
		}

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
		DTOffEffect->CreateFX(FXData::Find("Nero_DT_Off.effect"));
		DTOffEffect->ChangeFX("Nero_DT_Off.effect");
		DTOffEffect->Off();
	}

	// Renderer 생성
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Character");
		NewDir.Move("Player");
		NewDir.Move("Nero");
		NewDir.Move("Mesh");
		if (nullptr == GameEngineFBXMesh::Find("Nero.FBX"))
		{
			GameEngineFBXMesh::Load(NewDir.GetPlusFileName("Nero.fbx").GetFullPath());
			GameEngineTexture::Load(NewDir.GetPlusFileName("pl0010_03_atos.texout.png").GetFullPath());
			GameEngineTexture::Load(NewDir.GetPlusFileName("pl0000_03_atos.texout.png").GetFullPath());
		}
		NewDir.MoveParent();
		NewDir.Move("Animation");

		Renderer->GetTransform()->SetLocalRotation({ 0, 0, 0 });
		Renderer->GetTransform()->SetLocalPosition({ 0, -75, 0 });

		switch (GameEngineOption::GetOption("Shader"))
		{
		case GameEngineOptionValue::Low:
		{
			Renderer->SetFBXMesh("Nero.FBX", "AniFBX_Low");
		}
		break;
		case GameEngineOptionValue::High:
		{
			Renderer->SetFBXMesh("Nero.FBX", "AniFBX");
		}
		break;
		default:
			break;
		}

		Renderer->SetMaterial("pl0000_03_albm.texout.png", "AniFBX_Alpha", nullptr);

		Renderer->ShadowOn();
		Renderer->SetDynamic();

		Renderer->SetSpecularTexture("pl0000_03_albm.texout.png", "pl0000_03_atos.texout.png");
		Renderer->SetSpecularTexture("pl0010_03_albm.texout.png", "pl0010_03_atos.texout.png");

		AnimationEvent::LoadAll({ .Dir = NewDir.GetFullPath().c_str(), .Renderer = Renderer,
			//.Objects = { (GameEngineObject*)Col_Attack.get() },
			.CallBacks_void = {
				nullptr,
				std::bind(&PlayerActor_Nero::RedQueenOn, this),
				std::bind(&PlayerActor_Nero::RedQueenOff, this),
				std::bind(&PlayerActor_Nero::BlueRoseOn, this),
				std::bind(&PlayerActor_Nero::WeaponIdle, this),
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				std::bind(&PlayerActor_Nero::SetOverture, this),
				std::bind(&PlayerActor_Nero::SetHuman, this),
				std::bind(&PlayerActor_Nero::SetDemon, this),
				nullptr,
			},
			.CallBacks_int = {
				std::bind(&GameEngineFSM::ChangeState, &FSM, std::placeholders::_1),
				std::bind(&SoundController::Play, &Sound, "RQ_", std::placeholders::_1),
				std::bind(&SoundController::Play, &Sound, "BR_", std::placeholders::_1),
			},
			});

		//Object = 0 : 공격 충돌체
		//
		//콜백void = 0 : 입력체크시작
		//콜백void = 1 : 손에 레드퀸 들려줌
		//콜백void = 2 : 손에 레드퀸 뺌
		//콜백void = 3 : 손에 블루로즈
		//콜백void = 4 : WeaponIdle (빈손, 칼 등에)
		//콜백void = 5 : SetLinearVelocityZero
		//콜백void = 6 : 이동체크 시작
		//콜백void = 7 : 딜레이체크 시작k
		//콜백void = 8 : 중력 적용
		//콜백void = 9 : SetOverture
		//콜백void = 10 : SetHuman
		//콜백void = 11 : SetDemon
		//콜백void = 12 : 데빌브레이커 파괴
		//
		//콜백 int = 0 : FSM변경
		// 
		//콜백 float4 = 0 : SetForce
		//콜백 float4 = 0 : SetPush
		//콜백 float4 = 0 : SetMove

	}

	// OvertureRenderer 생성
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Character");
		NewDir.Move("Player");
		NewDir.Move("Nero");
		NewDir.Move("Overture");
		NewDir.Move("Mesh");
		if (nullptr == GameEngineFBXMesh::Find("Overture.FBX"))
		{
			GameEngineFBXMesh::Load(NewDir.GetPlusFileName("Overture.fbx").GetFullPath());
		}
		Renderer_Overture = CreateComponent<GameEngineFBXRenderer>();
		Renderer_Overture->GetTransform()->SetLocalPosition({ 0, -75, 0 });
		switch (GameEngineOption::GetOption("Shader"))
		{
		case GameEngineOptionValue::Low:
		{
			Renderer_Overture->SetFBXMesh("Overture.FBX", "AniFBX_Low");
		}
		break;
		case GameEngineOptionValue::High:
		{
			Renderer_Overture->SetFBXMesh("Overture.FBX", "AniFBX");
		}
		break;
		default:
			break;
		}

		Renderer_Overture->ShadowOn();
		Renderer_Overture->SetDynamic();

		NewDir.MoveParent();
		NewDir.Move("Animation");

		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
		for (GameEngineFile File : Files)
		{
			if (nullptr == GameEngineFBXAnimation::Find(File.GetFileName()))
			{
				GameEngineFBXAnimation::Load(File.GetFullPath());
			}
			Renderer_Overture->CreateFBXAnimation(File.GetFileName(), { .Inter = 0.0166f, .Loop = false });
		}
		Renderer_Overture->ChangeAnimation("wp00_010_Shoot.fbx");
		Renderer_Overture->Off();
	}
	// Snatch 생성
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Character", "Player", "Nero", "Wire"
			});
		if (nullptr == GameEngineFBXMesh::Find("Nero_WireArm.FBX"))
		{
			GameEngineFBXMesh::Load(Path + "\\Nero_WireArm.fbx");
		}

		Snatch = GetLevel()->CreateActor<Player_Snatch>();
		Snatch->GetTransform()->SetParent(GetTransform());
	}
	SetHuman();
	WeaponIdle();
	Col_Attack->Off();
	// 기본 움직임
	{
		// Idle
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Idle,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Idle_Normal");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Run Start
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RunStart,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Run_Start");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// Run
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Run,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Run_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// RunStop
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RunStop,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Run_Stop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Dash
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Dash,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Dash_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// DashStop
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_DashStop,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Dash_Stop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// Jump_Vertical
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Jump_Vertical,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_Vertical");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// Jump_Back
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Jump_Back,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_Back");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// Jump Fly
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Jump_Fly,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Jump_Fly_loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// Landing
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Landing,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_Landing");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// 2nd_Jump
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_2nd_Jump,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_2ndJump", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// 2nd_Jump_Back
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_2nd_Jump_Back,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_Back_2ndJump", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// EnemyStep Jump
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_EnemyStep_Jump,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_EnemyStep", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
	}
	{}
	// 레드 퀸
	{
		// RedQueen ComboA1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboA_1,
			.Start = [=] {
				Sound.Play("RQ_", 0);
				Sound.PlayVoiceRandom(0, 1, DTValue);
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(120));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 12);
					EffectSystem->PlayFX("RQ_ComboA1_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_ComboA1.effect");
				}
				Renderer->ChangeAnimation("pl0000_RQ_ComboA_1");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen ComboA2
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboA_2,
			.Start = [=] {
				Sound.Play("RQ_", 1);
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(90));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 14);
					EffectSystem->PlayFX("RQ_ComboA2_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_ComboA2.effect");
				}
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboA_2");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen ComboA3
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboA_3,
			.Start = [=] {
				Sound.Play("RQ_", 3);
				Sound.PlayVoiceRandom(2, 3, DTValue);
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(60));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 12);
					EffectSystem->PlayFX("RQ_ComboA3_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_ComboA3.effect");
				}
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboA_3");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen ComboA4
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboA_4,
			.Start = [=] {
				Sound.PlayVoiceRandom(4, 5, DTValue);
				Col_Attack->SetAttackData(DamageType::Heavy, DamageCalculate(150));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 12);
					EffectSystem->PlayFX("RQ_ComboA4_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_ComboA4.effect");
				}
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboA_4");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen ComboD1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboD_1,
			.Start = [=] {
				Sound.PlaySetVolume("RQ_", 2, 0.6f);
				Sound.PlayVoiceRandom(3, 4, DTValue);
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(60));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 12);
					EffectSystem->PlayFX("RQ_ComboD1_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_ComboD1.effect");
				}
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboD_1");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen ComboD2
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboD_2,
			.Start = [=] {
				Sound.Play("RQ_", 6);
				Sound.PlayVoice(5, DTValue);
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(90));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 14);
					EffectSystem->PlayFX("RQ_ComboD2_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_ComboD2.effect");
				}
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboD_2");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen ComboD3
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboD_3,
			.Start = [=] {
				Sound.PlaySetVolume("RQ_", 8, 0.6f);
				Sound.PlayVoice(9, DTValue);
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(60));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 12);
					EffectSystem->PlayFX("RQ_ComboD3_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_ComboD3.effect");
				}
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboD_3");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen ComboD4
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboD_4,
			.Start = [=] {
				Sound.Play("RQ_", 4);
				Sound.PlayVoice(7, DTValue);
				Col_Attack->SetAttackData(DamageType::Heavy, DamageCalculate(150));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 14);
					EffectSystem->PlayFX("RQ_ComboD4_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_ComboD4.effect");
				}
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboD_4");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen HR
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_HR,
			.Start = [=] {
				Sound.PlayVoiceRandom(15, 16, DTValue);
				Col_Attack->SetAttackData(DamageType::Air, DamageCalculate(135, true));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 12);
					EffectSystem->PlayFX("RQ_HR_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_HR.effect");
				}
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_HR-EX");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen Shuffle
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Shuffle,
			.Start = [=] {
				Sound.Play("RQ_", 22);
				TimeEvent.AddEvent(0.4f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
					{
						Sound.PlayVoiceRandom(18, 19, DTValue);
					});
				Col_Attack->SetAttackData(DamageType::Heavy, DamageCalculate(180, true));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 12);
					EffectSystem->PlayFX("RQ_Shuffle_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_Shuffle.effect");
				}
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Shuffle", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen Stleak1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Stleak1,
			.Start = [=] {
				Sound.Play("RQ_", 22);
				EffectSystem->PlayFX("RQ_Stleak_1.effect");
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Stleak_1");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen Stleak2
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Stleak2,
			.Start = [=] {
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Stleak_2_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen Stleak3
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Stleak3,
			.Start = [=] {
				Sound.Play("RQ_", 8);
				Sound.PlayVoiceRandom(15, 17, DTValue);
				Col_Attack->SetAttackData(DamageType::Heavy, DamageCalculate(180, true));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 12);
					EffectSystem->PlayFX("RQ_Stleak_3_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_Stleak_3.effect");
				}
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Stleak_3");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen AirComboA1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_AirComboA_1,
			.Start = [=] {
				Sound.Play("RQ_", 0);
				Sound.PlayVoiceRandom(4, 5, DTValue);
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(150));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 12);
					EffectSystem->PlayFX("RQ_AirComboA1_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_AirComboA1.effect");
				}
				Renderer->ChangeAnimation("pl0000_RQ_AT-Jump_1");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen AirComboA2
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_AirComboA_2,
			.Start = [=] {
				Sound.Play("RQ_", 1);
				Col_Attack->SetAttackData(DamageType::Light, DamageCalculate(120));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 12);
					EffectSystem->PlayFX("RQ_AirComboA2_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_AirComboA2.effect");
				}
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_AT-Jump_2");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen AirComboA3
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_AirComboA_3,
			.Start = [=] {
				Sound.Play("RQ_", 7);
				Sound.PlayVoiceRandom(6, 7, DTValue);
				Col_Attack->SetAttackData(DamageType::Slam, DamageCalculate(180));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 14);
					EffectSystem->PlayFX("RQ_AirComboA3_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_AirComboA3.effect");
				}
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_AT-Jump_3");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen AirComboB
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_AirComboB,
			.Start = [=] {
				Sound.Play("RQ_", 2);
				Sound.PlayVoiceRandom(7, 8, DTValue);
				Col_Attack->SetAttackData(DamageType::Air, DamageCalculate(405));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 14);
					EffectSystem->PlayFX("RQ_AirComboB_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_AirComboB.effect");
				}
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_AirComboB");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen Split_1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Split_1,
			.Start = [=] {
				Sound.Play("RQ_", 3);
				Sound.PlayVoiceRandom(13, 14, DTValue);
				Col_Attack->SetAttackData(DamageType::Slam, DamageCalculate(22, true));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 14);
					EffectSystem->PlayFX("RQ_Split_1_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_Split_1.effect");
				}
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Split_1");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// RedQueen Split_2
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Split_2,
			.Start = [=] {
				if (0 < ExceedLevel)
				{
					EffectSystem->PlayFX("RQ_Split_2_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_Split_2.effect");
				}
				EffectSystem->Loop = true;
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Split_2_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				EffectSystem->Loop = false;
			}
			});
		// RedQueen Split_3
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Split_3,
			.Start = [=] {
				Sound.Play("RQ_", 11);
				Col_Attack->SetAttackData(DamageType::Heavy, DamageCalculate(90, true));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 13);
					EffectSystem->PlayFX("RQ_Split_3_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_Split_3.effect");
				}
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Split_3");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen Caliber_1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Caliber_1,
			.Start = [=] {
				Sound.Play("RQ_", 22);
				EffectSystem->PlayFX("RQ_Calibur_1.effect");
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Caliber_1");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// RedQueen Caliber_2
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Caliber_2,
			.Start = [=] {
				Sound.Play("RQ_", 8);
				Sound.PlayVoiceRandom(19, 20, DTValue);
				Col_Attack->SetAttackData(DamageType::Heavy, DamageCalculate(180, true));
				if (0 < ExceedLevel)
				{
					Sound.Play("RQ_", 12);
					EffectSystem->PlayFX("RQ_Calibur_2_EX.effect");
				}
				else
				{
					EffectSystem->PlayFX("RQ_Calibur_2.effect");
				}
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Caliber_2");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});


	}
	{}
	// 블루 로즈 (락온)
	{
		// Idle To LockOn
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_BR_Switch_Idle_to_Lockon");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// LockOnFront
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Lockon_Front,
			.Start = [=] {
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_BR_Lockon_Front");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Strafe
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Strafe,
			.Start = [=] {
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_Strafe_F_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			} });
		// StrafeF
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Strafe_F,
			.Start = [=] {
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_Strafe_F_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			} });
		// StrafeFL
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Strafe_FL,
			.Start = [=] {
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_Strafe_FL_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			} });
		// StrafeL
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Strafe_L,
			.Start = [=] {
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_Strafe_L_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			} });
		// StrafeBL
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Strafe_BL,
			.Start = [=] {
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_Strafe_BL_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			} });
		// StrafeB
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Strafe_B,
			.Start = [=] {
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_Strafe_B_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			} });
		// StrafeBR
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Strafe_BR,
			.Start = [=] {
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_Strafe_BR_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			} });
		// StrafeR
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Strafe_R,
			.Start = [=] {
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_Strafe_R_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			} });
		// LockOn To Idle
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Switch_Lockon_to_Idle,
			.Start = [=] {
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_BR_Switch_Lockon_to_Idle");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Evade
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Evade_Left,
			.Start = [=] {
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_Evade_Left", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Evade
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Evade_Right,
			.Start = [=] {
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_Evade_Right", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Shoot,
			.Start = [=] {
				Sound.Play("BR_", 0);
				EffectSystem->PlayFX("BR_Shoot.effect");
				Col_Attack->SetAttackData(DamageType::Light, 54, std::bind(&GameEngineObjectBase::Off, Col_Attack));
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_BR_Shoot", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Air Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_AirShoot,
			.Start = [=] {
				Sound.Play("BR_", 0);
				EffectSystem->PlayFX("BR_Shoot.effect");
				Col_Attack->SetAttackData(DamageType::Light, 54, std::bind(&GameEngineObjectBase::Off, Col_Attack));
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_BR_Air_Shoot", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
	}
	// 오버추어
	{}
	{
		// Nero_Overture_Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Overture_Shoot,
			.Start = [=] {
				TimeEvent.AddEvent(0.15f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Sound.PlayVoiceRandom(6, 7, DTValue);
					Sound.Play("Overture_Shoot");
				});
				WeaponIdle();
				SetOvertureAnimation();
				Col_Attack->SetAttackData(DamageType::Heavy, 420);
				EffectSystem->PlayFX("Overture_Shoot.effect");
				Renderer->ChangeAnimation("pl0000_Overture_Shoot", true);
				Renderer_Overture->ChangeAnimation("wp00_010_Shoot.fbx", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				SetOverture();
				WeaponIdle();
			}
			});
		// Nero_Overture_AirShoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Overture_AirShoot,
			.Start = [=] {
				TimeEvent.AddEvent(0.15f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Sound.PlayVoiceRandom(6, 7, DTValue);
					Sound.Play("Overture_Shoot");
				});
				WeaponIdle();
				SetOvertureAnimation();
				Col_Attack->SetAttackData(DamageType::Heavy, 420);
				EffectSystem->PlayFX("Overture_Shoot.effect");
				Renderer->ChangeAnimation("pl0000_Overture_Air_Shoot", true);
				Renderer_Overture->ChangeAnimation("wp00_010_Air_Shoot.fbx", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				SetOverture();
				WeaponIdle();
			}
			});
	}
	// 거베라
	{}
	{
		// Nero_Gerbera_Back
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Gerbera_Back,
			.Start = [=] {
				Sound.Play("Gerbera_Shoot");
				WeaponIdle();
				Col_Attack->SetAttackData(DamageType::Heavy, 75);
				EffectSystem->PlayFX("Gerbera_Jump_Back.effect");
				Renderer->ChangeAnimation("pl0000_Gerbera_Jump_Back", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Nero_Gerbera_Front
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Gerbera_Front,
			.Start = [=] {
				Sound.Play("Gerbera_Shoot");
				WeaponIdle();
				Col_Attack->SetAttackData(DamageType::Heavy, 75);
				EffectSystem->PlayFX("Gerbera_Jump_Back.effect");
				EffectSystem->GetTransform()->SetLocalRotation({ 0, 180, 0 });
				Renderer->ChangeAnimation("pl0000_Gerbera_Jump_Front", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				EffectSystem->GetTransform()->SetLocalRotation(float4::ZERO);
			}
			});

		// Nero_Gerbera_Left
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Gerbera_Left,
			.Start = [=] {
				Sound.Play("Gerbera_Shoot");
				WeaponIdle();
				Col_Attack->SetAttackData(DamageType::Heavy, 75);
				EffectSystem->PlayFX("Gerbera_Jump_Back.effect");
				EffectSystem->GetTransform()->SetLocalRotation({ 0, 90, 0 });
				Renderer->ChangeAnimation("pl0000_Gerbera_Jump_Left", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				EffectSystem->GetTransform()->SetLocalRotation(float4::ZERO);
			}
			});

		// Nero_Gerbera_Right
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Gerbera_Right,
			.Start = [=] {
				Sound.Play("Gerbera_Shoot");
				WeaponIdle();
				Col_Attack->SetAttackData(DamageType::Heavy, 75);
				EffectSystem->PlayFX("Gerbera_Jump_Back.effect");
				EffectSystem->GetTransform()->SetLocalRotation({ 0, -90, 0 });
				Renderer->ChangeAnimation("pl0000_Gerbera_Jump_Right", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				EffectSystem->GetTransform()->SetLocalRotation(float4::ZERO);
			}
			});
	}
	// 버스터암
	{}
	{
		// BusterArm Catch
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Catch,
			.Start = [=] {
				Sound.PlayVoiceRandom(6, 7, DTValue);
				Sound.Play("BusterArm_Catch");
				Col_Attack->SetAttackData(DamageType::Buster, 0);
				WeaponIdle();
				EffectSystem->PlayFX("Buster_Catch.effect");
				Renderer->ChangeAnimation("pl0000_Buster_Catch", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				Col_Attack->Off();
			}
			});

		// BusterArm Strike
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Strike,
			.Start = [=] {
				Sound.Play("BusterArm_Strike");
				EffectSystem->PlayFX("Buster_Strike.effect");
				Renderer->ChangeAnimation("pl0000_Buster_Strike_Common");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// BusterArm em5501
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_em5501,
			.Start = [=] {
				Sound.Play("BusterArm_Strike");
				EffectSystem->PlayFX("Buster_Strike_em5501.effect");
				Renderer->ChangeAnimation("pl0000_Buster_em5501");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// BusterArm Repelled
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Repelled,
			.Start = [=] {
				Sound.Play("BusterArm_Repelled");
				Renderer->ChangeAnimation("pl0000_Buster_Catch_Repelled");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// BusterArm Catch Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Catch_Air,
			.Start = [=] {
				Sound.PlayVoiceRandom(6, 7, DTValue);
				Sound.Play("BusterArm_Catch");
				Col_Attack->SetAttackData(DamageType::Buster, 50);
				WeaponIdle();
				EffectSystem->PlayFX("Buster_Catch.effect");
				Renderer->ChangeAnimation("pl0000_Buster_Air_Catch", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				Col_Attack->Off();
			}
			});

		// BusterArm Strike Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Strike_Air,
			.Start = [=] {
				Sound.Play("BusterArm_Strike");
				EffectSystem->PlayFX("Buster_Strike.effect");
				Renderer->ChangeAnimation("pl0000_Buster_Air_Strike_Common");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// BusterArm Repelled Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Repelled_Air,
			.Start = [=] {
				Sound.Play("BusterArm_Repelled");
				Renderer->ChangeAnimation("pl0000_Buster_Air_Catch_Repelled");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
	}
	// 스내치
	{}
	{
		static float4 SnatchDir;
		const static float SnatchSpeed = 4000.0f;
		// Snatch Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Shoot,
			.Start = [=] {
				Sound.Play("Snatch_Shoot");
				Sound.PlayVoiceRandom(0, 3, DTValue);
				WeaponIdle();
				Col_Attack->SetAttackData(DamageType::Snatch, 0);
				Renderer->ChangeAnimation("pl0000_Snatch", true);
				Snatch->SnatchOn(LockOnPosition);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				Col_Attack->Off();
			}
			});

		// Snatch Pull
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Pull,
			.Start = [=] {
				Snatch->SnatchOff();
				Renderer->ChangeAnimation("pl0000_Snatch_Pull");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Snatch Repel
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Repel,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Snatch_Repel");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Snatch Shoot Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Shoot_Air,
			.Start = [=] {
				Sound.Play("Snatch_Shoot");
				Sound.PlayVoiceRandom(0, 3, DTValue);
				WeaponIdle();
				Col_Attack->SetAttackData(DamageType::Snatch, 0);
				Renderer->ChangeAnimation("pl0000_Air_Snatch", true);
				Snatch->SnatchOn(LockOnPosition);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Snatch Pull Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Pull_Air,
			.Start = [=] {
				Snatch->SnatchOff();
				Renderer->ChangeAnimation("pl0000_Air_Snatch_Pull", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Snatch Repel Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Repel_Air,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Air_Snatch_Repel", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
	}
	// 특수
	{}
	{
		// DT Start
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_DT_Start,
		.Start = [=] {
			Sound.Play("DT_On");
			Sound.PlayVoiceRandom(34, 36, true);
			WeaponIdle();
			Col_Attack->SetAttackData(DamageType::Air, 150);
			EffectSystem->PlayFX("Nero_DT_On.effect");
			Renderer->ChangeAnimation("pl0000_DT_Start", true);
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {
		}
			});
		// DT Air Start
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_DT_AirStart,
			.Start = [=] {
				Sound.Play("DT_On");
				Sound.PlayVoiceRandom(34, 36, true);
				WeaponIdle();
				Col_Attack->SetAttackData(DamageType::Air, 150);
				EffectSystem->PlayFX("Nero_DT_On.effect");
				Renderer->ChangeAnimation("pl0000_DT_AirStart", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// GT Bomb
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_GT_Bomb,
		.Start = [=] {
			Sound.Play("GT_Bomb");
			Sound.PlayVoice(33, DTValue);
			WeaponIdle();
			Col_Attack->SetAttackData(DamageType::Heavy, 150);
			EffectSystem->PlayFX("GT_Bomb.effect");
			Renderer->ChangeAnimation("pl0000_GT_Bomb", true);
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {
		}
			});
		// GT Bomb Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_GT_AirBomb,
			.Start = [=] {
				Sound.Play("GT_Bomb");
				Sound.PlayVoice(33, DTValue);
				WeaponIdle();
				Col_Attack->SetAttackData(DamageType::Heavy, 150);
				EffectSystem->PlayFX("GT_Bomb.effect");
				Renderer->ChangeAnimation("pl0000_GT_AirBomb", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// EnemyStep
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_EnemyStep,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_EnemyStep", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});


		// Provocation 1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Provocation,
		.Start = [=] {
			Sound.PlayVoice(30, DTValue);
			WeaponIdle();
			Renderer->ChangeAnimation("pl0000_Provocation", true);
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {
		} });

		// Provocation Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Provocation_Air,
			.Start = [=] {
				Sound.Play("RQ_", 19);
				Sound.PlayVoiceRandom(31, 32, DTValue);
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Air_Provocation", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});

	}
	// 대미지
	{}
	{
		// Damage Light
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Light,
			.Start = [=] {
				Sound.PlayVoiceRandom(22, 24, DTValue);
				WeaponIdle();
				EffectSystem->PlayFX("Damage_Light.effect");
				Renderer->ChangeAnimation("pl0000_Damage_Common", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage Heavy
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Heavy,
			.Start = [=] {
				Sound.PlayVoiceRandom(24, 26, DTValue);
				WeaponIdle();
				EffectSystem->PlayFX("Damage_Heavy.effect");
				Renderer->ChangeAnimation("pl0000_Damage_Away_Fly", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage Fly
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Fly,
			.Start = [=] {
				Sound.PlayVoiceRandom(24, 26, DTValue);
				WeaponIdle();
				EffectSystem->PlayFX("Damage_Heavy.effect");
				Renderer->ChangeAnimation("pl0000_Damage_AirCombo_Fly", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage Fall
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Fall,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Damage_Supine_Fall_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage Ground
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Ground,
			.Start = [=] {
				Sound.PlayVoiceRandom(27, 28, DTValue);
				Renderer->ChangeAnimation("pl0000_Damage_Away_Ground");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage GetUp
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_GetUp,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Damage_Away_GetUp");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage Death1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Death_1,
			.Start = [=] {
				Sound.PlayVoice(29, DTValue);
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Damage_Kneel_Down_1", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage Death2
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Death_2,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Damage_Kneel_Down_GameOver", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage Fly Death
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Fly_Death,
			.Start = [=] {
				Sound.PlayVoice(29, DTValue);
				Renderer->ChangeAnimation("pl0000_Damage_Away_Death_Ground", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
	}

	FSM.ChangeState(FSM_State_Nero::Nero_Idle);
}