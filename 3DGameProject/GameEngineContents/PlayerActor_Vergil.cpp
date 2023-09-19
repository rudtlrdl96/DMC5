#include "PrecompileHeader.h"
#include "PlayerActor_Vergil.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include "AnimationEvent.h"
#include "PlayerController.h"
#include "AttackCollision.h"
PlayerActor_Vergil::~PlayerActor_Vergil()
{
}

void PlayerActor_Vergil::Start()
{
	BasePlayerActor::Start();
	SetNetObjectType(Net_ActorType::Vergil);
	UserControllLoad();
	VergilLoad();
}

void PlayerActor_Vergil::VergilLoad()
{
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
			GameEngineTexture::Load(NewDir.GetPlusFileName("pl0300_03_atos.texout.png").GetFullPath());
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
		}
		break;
		default:
			break;
		}
		Renderer->SetSpecularTexture("pl0300_03_albm.texout.png", "pl0300_03_atos.texout.png");

		AnimationEvent::LoadAll({ .Dir = NewDir.GetFullPath().c_str(), .Renderer = Renderer,
			.Objects = { (GameEngineObject*)Col_Attack.get() },
			.CallBacks_void = {
				std::bind([=] {InputCheck = true; }),			// 0
				std::bind(&PlayerActor_Vergil::YamatoOn, this),
				std::bind(&PlayerActor_Vergil::YamatoOff, this),
				std::bind(&PlayerActor_Vergil::SetHuman, this),
				std::bind(&PlayerActor_Vergil::SetMajin, this),
				std::bind(&PhysXCapsuleComponent::SetLinearVelocityZero, PhysXCapsule),		// 5
				std::bind([=] {MoveCheck = true; }),
				std::bind([=] {DelayCheck = true; }),
				std::bind(&PhysXCapsuleComponent::TurnOnGravity, PhysXCapsule),		// 8 
			},
			.CallBacks_int = {
				std::bind(&GameEngineFSM::ChangeState, &FSM, std::placeholders::_1)
			},
			.CallBacks_float4 = {
				std::bind(&BasePlayerActor::SetForce, this, std::placeholders::_1),
				std::bind(&BasePlayerActor::SetPush, this, std::placeholders::_1),
				std::bind(&BasePlayerActor::SetMove, this, std::placeholders::_1)
			}
			});
		SetHuman();
		YamatoOff();
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
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
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
				if (true == Input_GunCheck()) { return; }
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
				PhysXCapsule->TurnOffGravity();
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
				if (true == Input_GunCheck()) { return; }
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
				PhysXCapsule->SetMove(Controller->GetMoveVector() * WalkSpeed);

			},
			.End = [=] {

			}
			});
		// Run Start
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_RunStart,
			.Start = [=] {
				PhysXCapsule->TurnOffGravity();
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
				if (true == Input_GunCheck()) { return; }
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
				if (true == Input_GunCheck()) { return; }
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
				PhysXCapsule->SetLinearVelocityZero();
			}
			});
		// RunStop
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_RunStop,
			.Start = [=] {
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
				if (true == Input_GunCheck()) { return; }
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
			PhysXCapsule->TurnOnGravity();
			PhysXCapsule->SetLinearVelocityZero();
			PhysXCapsule->SetMove(Controller->GetMoveVector() * 500);
			InputCheck = false;
			MoveCheck = false;
			YamatoOff();
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
			if (true == Input_GunCheckFly()) { return; }
			if (true == Input_WarpCheckFly()) { return; }

			PhysXCapsule->SetForce(Controller->GetMoveVector() * 3500);

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
				PhysXCapsule->SetPush(float4::DOWN * 1700);
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
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_WarpCheckFly()) { return; }
				PhysXCapsule->SetForce(Controller->GetMoveVector() * 3500);

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
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
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
	}
	{}
	/* 야마토 */
	{
		// Combo1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Combo_1,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_yamato_Combo_1");
				RotationToTarget();
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
					if (Controller->GetIsSpecialMove())
					{
						ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_1);
					}
					return;
				}
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck(1)) { return; }
				if (true == Input_GunCheck()) { return; }
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
		// Combo2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Combo_2,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
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
				if (true == Input_GunCheck()) { return; }
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
		// Combo3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Combo_3,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
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
				if (true == DelayCheck)
				{
					if (true == Input_SwordCheck(4)) { return; }
				}
				else if (true == Input_SwordCheck(3)) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				YamatoOff();
			}
			});
		// Combo4
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Combo_4,
			.Start = [=] {
				YamatoOn();
				PhysXCapsule->SetLinearVelocityZero();
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
				if (true == Input_GunCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				YamatoOff();
			}
			});
		// ComboC1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_ComboC_1,
			.Start = [=] {
				YamatoOn();
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_yamato_ComboC_1");
				RotationToTarget(30.0f);
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
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_yamato_ComboC_2_Loop");
				RotationToTarget(30.0f);
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
					if (InputCheck == true)
					{
						Renderer->ChangeAnimation("pl0300_yamato_ComboC_2_Loop", true);
						InputCheck = false;
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
				PhysXCapsule->SetLinearVelocityZero();
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
				if (true == Input_GunCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				YamatoOff();
			}
			});
		// Sissonal1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Sissonal_1,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				RotationToTarget();
				Renderer->ChangeAnimation("pl0300_yamato_Sissonal_1");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheck()) { return; }
				if (false == FloorCheck())
				{
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
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheck()) { return; }
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
					return;
				}
				std::vector<std::shared_ptr<GameEngineCollision>> Cols;
				if (true == Col_EnemyStepCheck->CollisionAll(CollisionOrder::Enemy, Cols))
				{
					if (true == Controller->GetIsSwordPress())
					{
						ChangeState(FSM_State_Vergil::pl0300_yamato_Sissonal_Up);
						return;
					}
					ChangeState(FSM_State_Vergil::pl0300_yamato_Sissonal_3);
					return;
				}
				SissonalTimer += _DeltaTime;
				if (0.5f < SissonalTimer)
				{
					ChangeState(FSM_State_Vergil::pl0300_yamato_Sissonal_3);
					return;
				}
				PhysXCapsule->SetMove(GetTransform()->GetWorldForwardVector() * 1300);
			},
			.End = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				Controller->SwordChargeTimer = -1.0f;
			}
			});
		// Sissonal3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Sissonal_3,
			.Start = [=] {
				Col_Attack->SetAttackData(DamageType::Heavy, 50);
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_yamato_Sissonal_3");
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
				if (true == Input_GunCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				YamatoOff();
				Rot.y += 180.0f;
				PhysXCapsule->AddWorldRotation({ 0, 180, 0 });
				GetTransform()->AddWorldRotation({ 0, 180, 0 });
			}
			});
		// Sissonal Up
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Sissonal_Up,
			.Start = [=] {
				Col_Attack->SetAttackData(DamageType::Air, 50);
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
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
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_WarpCheckFly()) { return; }
			},
			.End = [=] {
				YamatoOff();
			}
			});
		// Upper 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Upper_1,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_yamato_AttackUp_1");
				RotationToTarget(30.0f);
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
				YamatoOn();
				Col_Attack->SetAttackData(DamageType::Air, 50);
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
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
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_WarpCheckFly()) { return; }
			},
			.End = [=] {
				YamatoOff();
			}
			});
		// Upper3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::pl0300_yamato_Upper_3,
			.Start = [=] {
				YamatoOn();
				Col_Attack->SetAttackData(DamageType::Air, 50);
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
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
				if (true == Input_GunCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				YamatoOff();
			}
			});

		// Air Combo 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Air_Combo_1,
			.Start = [=] {
				RotationToTarget();
				Col_Attack->SetAttackData(DamageType::Light, 50);
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
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
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_WarpCheckFly()) { return; }
			},
			.End = [=] {
				YamatoOff();
			}
			});
		// Air Combo 2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Air_Combo_2,
			.Start = [=] {
				YamatoOn();
				Col_Attack->SetAttackData(DamageType::Light, 50);
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
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
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_WarpCheckFly()) { return; }
			},
			.End = [=] {
				YamatoOff();
			}
			});
		// Air Combo 3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Air_Combo_3,
			.Start = [=] {
				YamatoOn();
				Col_Attack->SetAttackData(DamageType::Light, 50);
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
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
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_WarpCheckFly()) { return; }
			},
			.End = [=] {
				YamatoOff();
			}
			});
		// Air Combo B 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Air_ComboB_1,
			.Start = [=] {
				YamatoOn();
				Col_Attack->SetAttackData(DamageType::Air, 50);
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
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
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_WarpCheckFly()) { return; }
			},
			.End = [=] {
				YamatoOff();
			}
			});
		// Air Combo B 2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Air_ComboB_2,
			.Start = [=] {
				YamatoOn();
				Col_Attack->SetAttackData(DamageType::Slam, 50);
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
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
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_WarpCheckFly()) { return; }
			},
			.End = [=] {
				YamatoOff();
			}
			});
		// Raid1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Raid1,
			.Start = [=] {
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
				YamatoOff();
			}
			});
		// Raid2
		static float4 RaidTargetPos;
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Raid2,
			.Start = [=] {
				YamatoOn();
				Renderer->ChangeAnimation("pl0300_yamato_Raid2_Loop");
				GetLevel()->RayCast(GetTransform()->GetWorldPosition(), float4::DOWN, RaidTargetPos, 9999.0f);
				RaidTargetPos += float4::UP * 100;
			},
			.Update = [=](float _DeltaTime) {
				PhysXCapsule->SetMove(float4::DOWN * 500);
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
				YamatoOff();
			}
			});
		// Raid3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Raid3,
			.Start = [=] {
				Controller->SwordChargeTimer = 0.0f;
				YamatoOn();
				Renderer->ChangeAnimation("pl0300_yamato_Raid3");
			},
			.Update = [=](float _DeltaTime) {

				if (true == Input_SpecialCheck()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				YamatoOff();
			}
			});

		// Vergil_yamato_JudgementCut_1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCut_1,
			.Start = [=] {
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
				YamatoOff();
			}
			});
		// Vergil_yamato_JudgementCut_3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCut_3,
			.Start = [=] {
				YamatoOff();
				Renderer->ChangeAnimation("pl0300_yamato_JudgementCut_3");
			},
			.Update = [=](float _DeltaTime) {

				if (true == Input_SpecialCheck()) { return; }

				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
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
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
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
				YamatoOff();
			}
			});
		// Vergil_yamato_JudgementCutAir_3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCutAir_3,
			.Start = [=] {
				YamatoOff();
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
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_WarpCheckFly()) { return; }

			},
			.End = [=] {
			}
			});
		// Vergil_yamato_JudgementCutEnd_1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_1,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_yamato_JudgementCutEnd_1");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_2);
				}
			},
			.End = [=] {

			}
			});
		// Vergil_yamato_JudgementCutEnd_2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_2,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_yamato_JudgementCutEnd_2");
			},
			.Update = [=](float _DeltaTime) {
				if (InputCheck == false) { return; }

				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Vergil::Vergil_Warp_Left_1);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Vergil::Vergil_Warp_Right_1);
					return;
				}

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				YamatoOff();
			}
			});
	}
	{}
	/* 워프 */
	{
		// Warp Front 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Front_1,
		.Start = [=] {
			RotationToTarget();
			Renderer->ChangeAnimation("pl0300_Warp_Front_1", true);
			},
		.Update = [=](float _DeltaTime) {
			if (true == Renderer->IsAnimationEnd())
			{
				ChangeState(FSM_State_Vergil::Vergil_Warp_Front_2);
			}
		},
		.End = [=] {

		}
			});
		// Warp Front 2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Front_2,
		.Start = [=] {
			Renderer->ChangeAnimation("pl0300_Warp_Front_2");
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
				if (true == Input_GunCheck()) { return; }
				if (true == Input_WarpCheck()) { return; }
			}
			else
			{
				if (true == Input_SpecialCheckFly()) { return; }

				if (InputCheck == false) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
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

		} });
		// Warp Back 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Back_1,
		.Start = [=] {
			RotationToTarget();
			Renderer->ChangeAnimation("pl0300_Warp_Back_1");
		},
		.Update = [=](float _DeltaTime) {
			if (true == Renderer->IsAnimationEnd())
			{
				ChangeState(FSM_State_Vergil::Vergil_Warp_Back_2);
			}
		},
		.End = [=] {

		}
			});
		// Warp Back 2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Back_2,
		.Start = [=] {
			Renderer->ChangeAnimation("pl0300_Warp_Back_2");
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
			if (true == Input_GunCheck()) { return; }
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
		// Warp Left 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Left_1,
		.Start = [=] {
			RotationToTarget();
			Renderer->ChangeAnimation("pl0300_Warp_Left_1");
		},
		.Update = [=](float _DeltaTime) {
			if (true == Renderer->IsAnimationEnd())
			{
				ChangeState(FSM_State_Vergil::Vergil_Warp_Left_2);
			}
		},
		.End = [=] {

		}
		});
		// Warp Left 2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Left_2,
		.Start = [=] {
			Renderer->ChangeAnimation("pl0300_Warp_Left_2");
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
			if (true == Input_GunCheck()) { return; }
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
		// Warp Right 1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Right_1,
		.Start = [=] {
			RotationToTarget();
			Renderer->ChangeAnimation("pl0300_Warp_Right_1", true);
			},
		.Update = [=](float _DeltaTime) {
			if (true == Renderer->IsAnimationEnd())
			{
				ChangeState(FSM_State_Vergil::Vergil_Warp_Right_2);
			}
		},
		.End = [=] {

		}
		});
		// Warp Right 2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Right_2,
		.Start = [=] {
			Renderer->ChangeAnimation("pl0300_Warp_Right_2");
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
			if (true == Input_GunCheck()) { return; }
			if (true == Input_WarpCheck()) { return; }

			if (MoveCheck == false) { return; }
			if (Controller->GetMoveVector() != float4::ZERO)
			{
				ChangeState(FSM_State_Vergil::Vergil_RunStart);
				return;
			}
		},
		.End = [=] {

		} });

		// Warp Up
		static float4 WarpPos;
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_AirTrick,
		.Start = [=] {
			PhysXCapsule->TurnOffGravity();
			PhysXCapsule->SetLinearVelocityZero();
			WarpPos = float4::ZERO;
			if (nullptr != LockOnEnemyTransform)
			{
				RotationToTarget();
				WarpPos = LockOnEnemyTransform->GetWorldPosition();
				WarpPos = WarpPos + (GetTransform()->GetWorldPosition() - LockOnEnemyTransform->GetWorldPosition()).NormalizeReturn() * 150;
			}
			Renderer->ChangeAnimation("pl0300_Warp_Up", true);
			},
		.Update = [=](float _DeltaTime) {
			if (true == Renderer->IsAnimationEnd())
			{
				ChangeState(FSM_State_Vergil::Vergil_Warp_Front_2);
			}
		},
		.End = [=] {
			SetWorldPosition(WarpPos);
		}
		});

		// Warp Down
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_TrickDown,
		.Start = [=] {
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
				WarpPos.y += 100;
			}
			else
			{
				WarpPos = GetTransform()->GetWorldPosition();
			}
			Renderer->ChangeAnimation("pl0300_Warp_Down", true);
			},
		.Update = [=](float _DeltaTime) {
			if (true == Renderer->IsAnimationEnd())
			{
				ChangeState(FSM_State_Vergil::Vergil_Warp_Back_2);
			}
		},
		.End = [=] {
			SetWorldPosition(WarpPos);
		}
			});
	}
	/* 데빌 트리거 */
	{
		// Demon Start
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_DT_Start,
		.Start = [=] {
			IsDevilTrigger = true;
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
			if (true == Input_GunCheck()) { return; }
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
		// Demon End
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_DT_End,
		.Start = [=] {
			IsDevilTrigger = false;
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
			if (true == Input_GunCheck()) { return; }
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
	ChangeState(FSM_State_Vergil::Vergil_Idle);
}

void PlayerActor_Vergil::NetLoad()
{
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
		}
		NewDir.MoveParent();
		NewDir.Move("Animation");

		Renderer = CreateComponent<GameEngineFBXRenderer>();
		Renderer->GetTransform()->SetLocalRotation({ 0, 0, 0 });
		Renderer->GetTransform()->SetLocalPosition({ 0, -75, 0 });
		Renderer->SetFBXMesh("Vergil.FBX", "MeshAniTexture");
		AnimationEvent::LoadAll({ .Dir = NewDir.GetFullPath().c_str(), .Renderer = Renderer,
			.Objects = { (GameEngineObject*)Col_Attack.get() },
			.CallBacks_void = {
				nullptr,
				std::bind(&PlayerActor_Vergil::YamatoOn, this),
				std::bind(&PlayerActor_Vergil::YamatoOff, this),
				std::bind(&PlayerActor_Vergil::SetHuman, this),
				std::bind(&PlayerActor_Vergil::SetMajin, this),
			},
			.CallBacks_int = {
				std::bind(&GameEngineFSM::ChangeState, &FSM, std::placeholders::_1)
			}
			});

		SetHuman();
	}

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
				Renderer->ChangeAnimation("pl0300_yamato_Combo_1");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Combo2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Combo_2,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_yamato_Combo_2");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Combo3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Combo_3,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_yamato_Combo_3");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Combo4
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_Combo_4,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_yamato_Combo_4");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// ComboC1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_ComboC_1,
			.Start = [=] {
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
				Renderer->ChangeAnimation("pl0300_yamato_ComboC_2_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// ComboC3
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_ComboC_3,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_yamato_ComboC_3");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// Vergil_yamato_JudgementCutEnd_1
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_1,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_yamato_JudgementCutEnd_1");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Vergil_yamato_JudgementCutEnd_2
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_2,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_yamato_JudgementCutEnd_2");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
	}
	{}
	/* 워프 */
	{
		// Warp Left
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Left_1,
		.Start = [=] {
			Renderer->ChangeAnimation("pl0300_Warp_Left", true);
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
			});
		// Warp Right
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Right_1,
		.Start = [=] {
			Renderer->ChangeAnimation("pl0300_Warp_Right", true);
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
			});
	}
	/* 데빌 트리거 */
	{
		// Demon Start
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_DT_Start,
		.Start = [=] {
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
	ChangeState(FSM_State_Vergil::Vergil_Idle);

}

void PlayerActor_Vergil::Update_Character(float _DeltaTime)
{
	FSM.Update(_DeltaTime);
}

void PlayerActor_Vergil::ChangeState(int _StateValue)
{
	FSM.ChangeState(_StateValue);
	FSMValue = _StateValue;
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
	if (Controller->GetIsFrontSword())
	{
		ChangeState(FSM_State_Vergil::pl0300_yamato_Sissonal_1);
		return true;
	}
	if (Controller->GetIsBackSword())
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
	if (Controller->GetIsBackSword())
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

bool PlayerActor_Vergil::Input_GunCheck()
{
	return false;
}

bool PlayerActor_Vergil::Input_GunCheckFly()
{
	return false;
}

bool PlayerActor_Vergil::Input_JumpCheck()
{
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
		if (false == IsDevilTrigger)
		{
			IsDevilTrigger = true;
			ChangeState(FSM_State_Vergil::Vergil_DT_Start);
			return true;
		}
		IsDevilTrigger = false;
		ChangeState(FSM_State_Vergil::Vergil_DT_End);
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
	if (Controller->GetIsAnyJump())
	{
		std::vector<std::shared_ptr<GameEngineCollision>> Cols;
		if (true == Col_EnemyStepCheck->CollisionAll(CollisionOrder::Enemy, Cols))
		{
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

	for (int i = 9; i <= 21; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->On();
	}
	for (int i = 2; i <= 4; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->Off();
	}
}

void PlayerActor_Vergil::SetMajin()
{
	for (int i = 9; i <= 21; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->Off();
	}
	for (int i = 2; i <= 4; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->On();
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
*/