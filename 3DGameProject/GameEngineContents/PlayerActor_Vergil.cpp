#include "PrecompileHeader.h"
#include "PlayerActor_Vergil.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include "AnimationEvent.h"
#include "PlayerController.h"


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
					FSM.ChangeState(FSM_State_Vergil::Vergil_Run);
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
			InputCheck = false;
			Renderer->ChangeAnimation("pl0300_Jump_Vertical");
		},
		.Update = [=](float _DeltaTime) {
			if (Renderer->IsAnimationEnd())
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Fly);
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
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (InputCheck == false) {
					if (Controller->GetIsSpecialMove())
					{
						FSM.ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_1);
					}
					return;
				}

				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_yamato_Combo_2);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Right);
					return;
				}

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_RunStart);
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
				if (InputCheck == false) { return; }

				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_yamato_Combo_3);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Right);
					return;
				}
				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_RunStart);
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
				if (InputCheck == false) { return; }

				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
					return;
				}
				if (Controller->GetIsSword())
				{
					if (true == DelayCheck)
					{
						FSM.ChangeState(FSM_State_Vergil::Vergil_yamato_ComboC_1);
						return;
					}
					FSM.ChangeState(FSM_State_Vergil::Vergil_yamato_Combo_4);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Right);
					return;
				}
				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_RunStart);
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
				if (InputCheck == false) { return; }

				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Right);
					return;
				}
				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_RunStart);
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
				if (Renderer->IsAnimationEnd())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_yamato_ComboC_2);
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
					FSM.ChangeState(FSM_State_Vergil::Vergil_yamato_ComboC_3);
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
				if (InputCheck == false) { return; }

				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Right);
					return;
				}
				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
			},
			.End = [=] {
				YamatoOff();
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
					FSM.ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_2);
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
					FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Right);
					return;
				}

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_RunStart);
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
		// Warp Left
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Left,
		.Start = [=] {
			Renderer->ChangeAnimation("pl0300_Warp_Left", true);
			InputCheck = false;
			MoveCheck = false;
		},
		.Update = [=](float _DeltaTime) {
			if (InputCheck == false) { return; }

			if (Controller->GetIsJump())
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
				return;
			}
			if (Controller->GetIsLeftJump())
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Left);
				return;
			}
			if (Controller->GetIsRightJump())
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Right);
				return;
			}
			if (MoveCheck == false) { return; }
			if (Controller->GetMoveVector() != float4::ZERO)
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_RunStart);
				return;
			}
		},
		.End = [=] {

		}
			});
		// Warp Right
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Right,
		.Start = [=] {
			Renderer->ChangeAnimation("pl0300_Warp_Right", true);
			InputCheck = false;
			MoveCheck = false;
			},
		.Update = [=](float _DeltaTime) {
			if (InputCheck == false) { return; }

			if (Controller->GetIsJump())
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
				return;
			}
			if (Controller->GetIsLeftJump())
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Left);
				return;
			}
			if (Controller->GetIsRightJump())
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Right);
				return;
			}
			if (MoveCheck == false) { return; }
			if (Controller->GetMoveVector() != float4::ZERO)
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_RunStart);
				return;
			}
		},
		.End = [=] {

		}
			});
	}
	/* 데빌 트리거 */
	{
		// Demon Start
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Demon_Start,
		.Start = [=] {
			IsDevilTrigger = true;
			Renderer->ChangeAnimation("pl0300_Demon_Start", true);
			InputCheck = false;
			MoveCheck = false;
			},
		.Update = [=](float _DeltaTime) {
			if (InputCheck == false) { return; }

			if (Controller->GetIsJump())
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
				return;
			}
			if (Controller->GetIsLeftJump())
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Left);
				return;
			}
			if (Controller->GetIsRightJump())
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Right);
				return;
			}
			if (MoveCheck == false) { return; }
			if (Controller->GetMoveVector() != float4::ZERO)
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_RunStart);
				return;
			}
		},
		.End = [=] {

		}
			});
		// Demon End
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Demon_End,
		.Start = [=] {
			IsDevilTrigger = false;
			Renderer->ChangeAnimation("pl0300_Demon_End", true);
			InputCheck = false;
			MoveCheck = false;
			},
		.Update = [=](float _DeltaTime) {
			if (InputCheck == false) { return; }

			if (Controller->GetIsJump())
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
				return;
			}
			if (Controller->GetIsLeftJump())
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Left);
				return;
			}
			if (Controller->GetIsRightJump())
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_Warp_Right);
				return;
			}
			if (MoveCheck == false) { return; }
			if (Controller->GetMoveVector() != float4::ZERO)
			{
				FSM.ChangeState(FSM_State_Vergil::Vergil_RunStart);
				return;
			}
		},
		.End = [=] {

		}
			});
	}
	FSM.ChangeState(FSM_State_Vergil::Vergil_Idle);
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
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Left,
		.Start = [=] {
			Renderer->ChangeAnimation("pl0300_Warp_Left", true);
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
			});
		// Warp Right
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Warp_Right,
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
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Demon_Start,
		.Start = [=] {
			Renderer->ChangeAnimation("pl0300_Demon_Start", true);
			},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
			});
		// Demon End
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Demon_End,
		.Start = [=] {
			Renderer->ChangeAnimation("pl0300_Demon_End", true);
			},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
			});
	}
	FSM.ChangeState(FSM_State_Vergil::Vergil_Idle);

}

void PlayerActor_Vergil::Update_Character(float _DeltaTime)
{
	FSM.Update(_DeltaTime);
}

void PlayerActor_Vergil::ChangeState(FSM_State_Vergil _StateValue)
{
	FSM.ChangeState(_StateValue);
	FSMValue = _StateValue;
}

bool PlayerActor_Vergil::Input_SwordCheck(int AddState)
{
	return false;
}

bool PlayerActor_Vergil::Input_SwordCheckFly(int AddState)
{
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
		ChangeState(FSM_State_Vergil::Vergil_Warp_Left);
		return true;
	}
	if (Controller->GetIsRightJump())
	{
		ChangeState(FSM_State_Vergil::Vergil_Warp_Right);
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
	return false;
}

bool PlayerActor_Vergil::Input_WarpCheckFly()
{
	return false;
}

bool PlayerActor_Vergil::Input_SpecialCheck()
{
	return false;
}

bool PlayerActor_Vergil::Input_SpecialCheckFly()
{
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