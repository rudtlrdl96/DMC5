#include "PrecompileHeader.h"
#include "PlayerActor_Nero.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "AnimationEvent.h"
#include "PlayerController.h"
#include "NetworkManager.h"
PlayerActor_Nero::~PlayerActor_Nero()
{
}

void PlayerActor_Nero::SinglePlayLoad()
{
	UserControllLoad();
	PlayerLoad();
	LoadCheck = true;
}

void PlayerActor_Nero::Start()
{
	BasePlayerActor::Start();
	BreakerStack.push(DevilBreaker::None);
	BreakerStack.push(DevilBreaker::Overture);

	SetNetObjectType(Net_ActorType::Nero);

	//NetControllType::NetControll���� ����� �� �Ʒ� �ݹ��� ����˴ϴ�. 
	SetControllCallBack(NetControllType::NetControll, [=]()
		{
			NetControllLoad();
			NetLoad();
			LoadCheck = true;
		});

	//NetControllType::UserControll���� ����� �� �Ʒ� �ݹ��� ����˴ϴ�.
	SetControllCallBack(NetControllType::UserControll, [=]()
		{
			UserControllLoad();
			PlayerLoad();
			LoadCheck = true;
		});
}

void PlayerActor_Nero::PlayerLoad()
{
	// Effect ����
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Effect");
		NewDir.Move("Mesh");
		if (nullptr == GameEngineFBXMesh::Find("Effect_Mesh_01.FBX"))
		{
			std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
			for (GameEngineFile File : Files)
			{
				GameEngineFBXMesh::Load(File.GetFullPath());
			}
		}
		NewDir.MoveParent();
		NewDir.Move("Texture");
		if (nullptr == GameEngineTexture::Find("Effect_Texture_01.png"))
		{
			std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".png" });
			for (GameEngineFile File : Files)
			{
				GameEngineTexture::Load(File.GetFullPath());
			}
		}
		Renderer_EffectMesh = CreateComponent<GameEngineFBXRenderer>();
		Renderer_EffectMesh->SetFBXMesh("Effect_Mesh_01.FBX", "FBX");
		Renderer_EffectMesh->SetTexture("DiffuseTexture", "Effect_Texture_01.png");
		Renderer_EffectMesh->Off();
	}

	// Renderer ����
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

		Renderer = CreateComponent<GameEngineFBXRenderer>();
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

		Renderer->SetSpecularTexture("pl0000_03_albm.texout.png", "pl0000_03_atos.texout.png");
		Renderer->SetSpecularTexture("pl0010_03_albm.texout.png", "pl0010_03_atos.texout.png");

		AnimationEvent::LoadAll({ .Dir = NewDir.GetFullPath().c_str(), .Renderer = Renderer,
			.Objects = { (GameEngineObject*)AttackCollision.get() },
			.CallBacks_void = {
				std::bind([=] {InputCheck = true; }),
				std::bind(&PlayerActor_Nero::RedQueenOn, this),
				std::bind(&PlayerActor_Nero::RedQueenOff, this),
				std::bind(&PlayerActor_Nero::BlueRoseOn, this),
				std::bind(&PlayerActor_Nero::WeaponIdle, this),
				std::bind(&PhysXCapsuleComponent::SetLinearVelocityZero, PhysXCapsule),
				std::bind([=] {MoveCheck = true; }),
				std::bind([=] {DelayCheck = true; }),
				std::bind(&PhysXCapsuleComponent::TurnOnGravity, PhysXCapsule),
				std::bind(&PlayerActor_Nero::SetOverture, this),
				std::bind(&PlayerActor_Nero::SetHuman, this),
				std::bind(&PlayerActor_Nero::SetDemon, this),
				std::bind(&PlayerActor_Nero::DestroyBreaker, this),
			},
			.CallBacks_int = {
				std::bind(&PlayerActor_Nero::ChangeState, this, std::placeholders::_1)
			},
			.CallBacks_float = {
				std::bind(&BasePlayerActor::RotationToTarget, this, std::placeholders::_1),
				std::bind(&BasePlayerActor::RotationToMoveVector, this, std::placeholders::_1)
			},
			.CallBacks_float4 = {
				std::bind(&BasePlayerActor::SetForce, this, std::placeholders::_1),
				std::bind(&BasePlayerActor::SetPush, this, std::placeholders::_1),
				std::bind(&BasePlayerActor::SetMove, this, std::placeholders::_1)
			}
			});

		//Object = 0 : ���� �浹ü
		//
		//�ݹ�void = 0 : �Է�üũ����
		//�ݹ�void = 1 : �տ� ������ �����
		//�ݹ�void = 2 : �տ� ������ ��
		//�ݹ�void = 3 : �տ� ������
		//�ݹ�void = 4 : WeaponIdle (���, Į �)
		//�ݹ�void = 5 : SetLinearVelocityZero
		//�ݹ�void = 6 : �̵�üũ ����
		//�ݹ�void = 7 : ������üũ ����k
		//�ݹ�void = 8 : �߷� ����
		//�ݹ�void = 9 : SetOverture
		//�ݹ�void = 10 : SetHuman
		//�ݹ�void = 11 : SetDemon
		//�ݹ�void = 12 : �����극��Ŀ �ı�
		//
		//�ݹ� int = 0 : FSM����
		// 
		//�ݹ� float4 = 0 : SetForce
		//�ݹ� float4 = 0 : SetPush
		//�ݹ� float4 = 0 : SetMove

	}

	// OvertureRenderer ����
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
		NewDir.MoveParent();
		NewDir.Move("Animation");
		if (nullptr == GameEngineFBXAnimation::Find("wp00_010_Shoot.fbx"))
		{
			std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
			for (GameEngineFile File : Files)
			{
				GameEngineFBXAnimation::Load(File.GetFullPath());
				Renderer_Overture->CreateFBXAnimation(File.GetFileName(), { .Inter = 0.0166f, .Loop = false });
			}
		}
		Renderer_Overture->ChangeAnimation("wp00_010_Shoot.fbx");
		Renderer_Overture->Off();
	}

	SetHuman();
	SetOverture();
	WeaponIdle();

	// �⺻ ������
	{
		// Idle
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Idle,
			.Start = [=] {
				WeaponIdle();
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0000_Idle_Normal");
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
				if (true == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
					return;
				}
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
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }
				if (true == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
					return;
				}
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStop);
					return;
				}

				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Run);
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
		// Run
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Run,
			.Start = [=] {
				DashTimer = 0;
				Renderer->ChangeAnimation("pl0000_Run_Loop");
			},
			.Update = [=](float _DeltaTime) {
				DashTimer += _DeltaTime;

				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }
				if (true == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
					return;
				}

				if (Controller->GetMoveVector() == float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStop);
					return;
				}
				if (1 < DashTimer)
				{
					ChangeState(FSM_State_Nero::Nero_Dash);
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
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RunStop,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Run_Stop");
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }
				if (true == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
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
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }
				if (true == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
					return;
				}
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_DashStop);
					return;
				}

				LookDir(Controller->GetMoveVector());
				float4 MoveDir = Controller->GetMoveVector() * DashSpeed;
				PhysXCapsule->SetMove(MoveDir);
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
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }
				if (true == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				PhysXCapsule->SetLinearVelocityZero();
			}
			});
		// Jump_Vertical
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Jump_Vertical,
			.Start = [=] {
				RotationToMoveVector();
				PhysXCapsule->TurnOnGravity();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->SetMove(Controller->GetMoveVector() * 500);
				InputCheck = false;
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_Vertical");
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
				if (true == Input_SpecialCheckFly()) { return; }
				if (false == InputCheck) { return; }
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }

				PhysXCapsule->SetForce(Controller->GetMoveVector() * 3500);
			},
			.End = [=] {
			}
			});
		// Jump_Back
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Jump_Back,
			.Start = [=] {
				PhysXCapsule->TurnOnGravity();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->SetMove(Controller->GetMoveVector() * 500);
				InputCheck = false;
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_Back");
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
				if (true == Input_SpecialCheckFly()) { return; }
				if (false == InputCheck) { return; }
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }

				PhysXCapsule->SetForce(Controller->GetMoveVector() * 3500);
			},
			.End = [=] {
			}
			});
		// Jump Fly
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Jump_Fly,
			.Start = [=] {
				PhysXCapsule->SetPush(float4::DOWN * 1700);
				PhysXCapsule->TurnOnGravity();
				Renderer->ChangeAnimation("pl0000_Jump_Fly_loop");
			},
			.Update = [=](float _DeltaTime) {
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
				PhysXCapsule->SetForce(Controller->GetMoveVector() * 3500);
			},
			.End = [=] {
				PhysXCapsule->TurnOffGravity();
			}
			});
		// Landing
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Landing,
			.Start = [=] {
				UseDoubleJump = false;
				PhysXCapsule->SetLinearVelocityZero();
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_Landing");
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Idle);
				}
			},
			.End = [=] {

			}
			});
		// 2nd_Jump
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_2nd_Jump,
			.Start = [=] {
				InputCheck = false;
				RotationToMoveVector();
				PhysXCapsule->TurnOnGravity();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->SetMove(Controller->GetMoveVector() * 250);
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_2ndJump", true);
			},
			.Update = [=](float _DeltaTime) {
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }

				if (true == InputCheck)
				{
					if (true == Input_SpecialCheckFly()) { return; }
				}
				PhysXCapsule->SetForce(Controller->GetMoveVector() * 3500);
			},
			.End = [=] {
			}
			});
		// 2nd_Jump_Back
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_2nd_Jump_Back,
			.Start = [=] {
				InputCheck = false;
				PhysXCapsule->TurnOnGravity();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->SetMove(Controller->GetMoveVector() * 250);
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_Back_2ndJump", true);
			},
			.Update = [=](float _DeltaTime) {
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }

				if (true == InputCheck)
				{
					if (true == Input_SpecialCheckFly()) { return; }
				}
				PhysXCapsule->SetForce(Controller->GetMoveVector() * 3500);
			},
			.End = [=] {
			}
			});
	}
	{}
	// ���� ��
	{
		// RedQueen ComboA1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboA_1,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0000_RQ_ComboA_1");
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck(1)) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen ComboA2
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboA_2,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboA_2");
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
				DelayCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (true == DelayCheck)
				{
					if (true == Input_SwordCheck(4)) { return; }
				}
				else if (true == Input_SwordCheck(2)) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen ComboA3
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboA_3,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboA_3");
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck(3)) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen ComboA4
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboA_4,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboA_4");
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen ComboD1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboD_1,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboD_1");
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck(5)) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen ComboD2
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboD_2,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboD_2");
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck(6)) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen ComboD3
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboD_3,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboD_3");
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck(7)) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen ComboD4
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboD_4,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboD_4");
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen HR
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_HR,
			.Start = [=] {
				PhysXCapsule->TurnOnGravity();
				PhysXCapsule->SetLinearVelocityZero();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_HR-EX");
				RotationToTarget();
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
				if (true == Input_SpecialCheckFly()) { return; }
				if (false == InputCheck) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
				RedQueenOff();
				WeaponIdle();
			}
			});
		// RedQueen Shuffle
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Shuffle,
			.Start = [=] {
				InputCheck = false;
				DelayCheck = false;
				PhysXCapsule->SetLinearVelocityZero();
				RotationToTarget();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Shuffle", true);
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Idle);
				}
				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (DelayCheck == false) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen Stleak1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Stleak1,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				RotationToTarget();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Stleak_1");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheck()) { return; }
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_Stleak2);
				}
			},
			.End = [=] {
				RedQueenOff();
			}
			});
		static float StreakTimer = 0;
		// RedQueen Stleak2
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Stleak2,
			.Start = [=] {
				RedQueenOn();
				StreakTimer = 0;
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Stleak_2_Loop");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheck()) { return; }
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				std::vector<std::shared_ptr<GameEngineCollision>> Cols;
				if (true == PlayerCollision->CollisionAll(CollisionOrder::Enemy, Cols))
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_Stleak3);
					return;
				}
				StreakTimer += _DeltaTime;
				if (0.5f < StreakTimer)
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_Stleak3);
					return;
				}
				PhysXCapsule->SetMove(GetTransform()->GetWorldForwardVector() * 1300);
			},
			.End = [=] {
				RedQueenOff();
				PhysXCapsule->SetLinearVelocityZero();
			}
			});
		// RedQueen Stleak3
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Stleak3,
			.Start = [=] {
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				InputCheck = false;
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Stleak_3");
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Idle);
				}
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				RedQueenOff();
			}
			});
		// RedQueen AirComboA1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_AirComboA_1,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_RQ_AT-Jump_1");
				RotationToTarget();
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }
				if (false == InputCheck) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly(1)) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }

			},
			.End = [=] {
				WeaponIdle();
				PhysXCapsule->TurnOnGravity();
			}
			});
		// RedQueen AirComboA2
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_AirComboA_2,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_AT-Jump_2");
				RotationToTarget();
				InputCheck = false;
				DelayCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }
				if (false == InputCheck) { return; }
				if (DelayCheck == true)
				{
					if (true == Input_SwordCheckFly(3)) { return; }
				}
				else
				{
					if (true == Input_SwordCheckFly(2)) { return; }
				}
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
				WeaponIdle();
				PhysXCapsule->TurnOnGravity();
			}
			});
		// RedQueen AirComboA3
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_AirComboA_3,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_AT-Jump_3");
				RotationToTarget();
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }
				if (false == InputCheck) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
				WeaponIdle();
				PhysXCapsule->TurnOnGravity();
			}
			});
		// RedQueen AirComboB
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_AirComboB,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_AirComboB");
				RotationToTarget(30.0f);
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }
				if (false == InputCheck) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
				WeaponIdle();
				PhysXCapsule->TurnOnGravity();
			}
			});
		// RedQueen Split_1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Split_1,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				RotationToTarget();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Split_1");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheckFly()) { return; }
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_Split_2);
				}
			},
			.End = [=] {
			}
			});
		// RedQueen Split_2
		static float4 SplitTargetPos;
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Split_2,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Split_2_Loop");
				GetLevel()->RayCast(GetTransform()->GetWorldPosition(), float4::DOWN, SplitTargetPos, 9999.0f);
				SplitTargetPos += float4::UP * 100;
			},
			.Update = [=](float _DeltaTime) {
				PhysXCapsule->SetMove(float4::DOWN * 500);
				if (GetTransform()->GetWorldPosition().y < SplitTargetPos.y)
				{
					PhysXCapsule->SetWorldPosition(SplitTargetPos);
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_Split_3);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }

				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_Split_3);
					return;
				}
			},
			.End = [=] {
			}
			});
		// RedQueen Split_3
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Split_3,
			.Start = [=] {
				InputCheck = false;
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Split_3");
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
			}
			});
		// RedQueen Caliber_1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Caliber_1,
			.Start = [=] {
				RedQueenOn();
				RotationToTarget();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Caliber_1");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SpecialCheckFly()) { return; }
				std::vector<std::shared_ptr<GameEngineCollision>> Cols;
				if (true == PlayerCollision->CollisionAll(CollisionOrder::Enemy, Cols))
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_Caliber_2);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_Caliber_2);
				}
			},
			.End = [=] {
			}
			});
		// RedQueen Caliber_2
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Caliber_2,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Caliber_2");
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }
				if (false == InputCheck) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
				PhysXCapsule->TurnOnGravity();
				RedQueenOff();
			}
			});


	}
	{}
	// ��� ���� (����)
	{
		// Idle To LockOn
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0000_BR_Switch_Idle_to_Lockon");
			},
			.Update = [=](float _DeltaTime) {

				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }
				if (false == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Lockon_to_Idle);
					return;

				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_BR_Strafe);
					return;
				}
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Lockon_Front);
					return;
				}
				LookTarget();
			},
			.End = [=] {

			}
			});
		// LockOnFront
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Lockon_Front,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0000_BR_Lockon_Front");
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }
				if (false == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Lockon_to_Idle);
					return;

				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_BR_Strafe);
					return;
				}

				LookTarget();
			},
			.End = [=] {

			}
			});
		// Strafe
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Strafe,
			.Start = [=] {
				BlueRoseOn();
			},
			.Update = [=](float _DeltaTime) {

				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }
				if (false == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Lockon_to_Idle);
					return;

				}
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_BR_Lockon_Front);
					return;
				}

				LookTarget();

				float4 MoveDir = Controller->GetMoveVector() * WalkSpeed;
				PhysXCapsule->SetMove(MoveDir);

				switch (Controller->MoveVectorToChar(Controller->GetMoveVector()))
				{
				case '8':
					Renderer->ChangeAnimation("pl0000_Strafe_F_Loop");
					FSMValue = FSM_State_Nero::Nero_BR_Strafe_F;
					break;
				case '7':
					Renderer->ChangeAnimation("pl0000_Strafe_FL_Loop");
					FSMValue = FSM_State_Nero::Nero_BR_Strafe_FL;
					break;
				case '4':
					Renderer->ChangeAnimation("pl0000_Strafe_L_Loop");
					FSMValue = FSM_State_Nero::Nero_BR_Strafe_L;
					break;
				case '1':
					Renderer->ChangeAnimation("pl0000_Strafe_BL_Loop");
					FSMValue = FSM_State_Nero::Nero_BR_Strafe_BL;
					break;
				case '2':
					Renderer->ChangeAnimation("pl0000_Strafe_B_Loop");
					FSMValue = FSM_State_Nero::Nero_BR_Strafe_B;
					break;
				case '3':
					Renderer->ChangeAnimation("pl0000_Strafe_BR_Loop");
					FSMValue = FSM_State_Nero::Nero_BR_Strafe_BR;
					break;
				case '6':
				case '9':
					Renderer->ChangeAnimation("pl0000_Strafe_R_Loop");
					FSMValue = FSM_State_Nero::Nero_BR_Strafe_R;
					break;
				}
				NetworkManager::SendFsmChangePacket(this, FSMValue);

			},
			.End = [=] {
				PhysXCapsule->SetLinearVelocityZero();
			} });
		// LockOn To Idle
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Switch_Lockon_to_Idle,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0000_BR_Switch_Lockon_to_Idle");
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }
				if (true == Controller->GetIsLockOn())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Idle);
					return;
				}
			},
			.End = [=] {

			}
			});
		// Evade
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Evade_Left,
			.Start = [=] {
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				BlueRoseOn();
				InputCheck = false;
				MoveCheck = false;
				Renderer->ChangeAnimation("pl0000_Evade_Left", true);
			},
			.Update = [=](float _DeltaTime) {
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Lockon_Front);
					return;
				}
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_BR_Strafe);
					return;
				}

			},
			.End = [=] {
			}
			});
		// Evade
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Evade_Right,
			.Start = [=] {
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				BlueRoseOn();
				InputCheck = false;
				MoveCheck = false;
				Renderer->ChangeAnimation("pl0000_Evade_Right", true);
			},
			.Update = [=](float _DeltaTime) {
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Lockon_Front);
					return;
				}
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_BR_Strafe);
					return;
				}
			},
			.End = [=] {
			}
			});
		// Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Shoot,
			.Start = [=] {
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				RotationToTarget();
				BlueRoseOn();
				InputCheck = false;
				MoveCheck = false;
				Renderer->ChangeAnimation("pl0000_BR_Shoot", true);
			},
			.Update = [=](float _DeltaTime) {
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Lockon_Front);
					return;
				}
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }
				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }

				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_BR_Strafe);
					return;
				}			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// Air Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_AirShoot,
			.Start = [=] {
				BlueRoseOn();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				RotationToTarget();
				InputCheck = false;
				MoveCheck = false;
				Renderer->ChangeAnimation("pl0000_BR_Air_Shoot", true);
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }
				if (false == InputCheck) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
				WeaponIdle();
			}
			});
	}
	// �����߾�
	{}
	{
		// Nero_Overture_Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Overture_Shoot,
			.Start = [=] {
				WeaponIdle();
				SetOvertureAnimation();
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0000_Overture_Shoot", true);
				Renderer_Overture->ChangeAnimation("wp00_010_Shoot.fbx", true);
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					LookDir(Controller->GetMoveVector());
				}
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				SetOverture();
				WeaponIdle();
			}
			});
		// Nero_Overture_AirShoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Overture_AirShoot,
			.Start = [=] {
				WeaponIdle();
				SetOvertureAnimation();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_Overture_Air_Shoot", true);
				Renderer_Overture->ChangeAnimation("wp00_010_Air_Shoot.fbx", true);
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					LookDir(Controller->GetMoveVector());
				}
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (true == Input_SpecialCheckFly()) { return; }
				if (false == InputCheck) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
				SetOverture();
				WeaponIdle();
			}
			});
	}
	// �ź���
	{}
	{
		// Nero_Gerbera_Back
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Gerbera_Back,
			.Start = [=] {
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				InputCheck = false;
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Gerbera_Jump_Back", true);
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
				if (true == Input_SpecialCheckFly()) { return; }
				if (false == InputCheck) { return; }
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
			}
			});

			// Nero_Gerbera_Front
			FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Gerbera_Front,
				.Start = [=] {
					PhysXCapsule->TurnOffGravity();
					PhysXCapsule->SetLinearVelocityZero();
					InputCheck = false;
					WeaponIdle();
					Renderer->ChangeAnimation("pl0000_Gerbera_Jump_Front", true);
				},
				.Update = [=](float _DeltaTime) {
					if (Renderer->IsAnimationEnd())
					{
						ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					}
					if (true == Input_SpecialCheckFly()) { return; }
					if (false == InputCheck) { return; }
					if (true == FloorCheck())
					{
						ChangeState(FSM_State_Nero::Nero_Landing);
						return;
					}
					if (true == Input_JumpCheckFly()) { return; }
					if (true == Input_SwordCheckFly()) { return; }
					if (true == Input_GunCheckFly()) { return; }
					if (true == Input_DevilBreakerCheckFly()) { return; }
				},
				.End = [=] {
				}
				});

			// Nero_Gerbera_Left
			FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Gerbera_Left,
				.Start = [=] {
					PhysXCapsule->TurnOffGravity();
					PhysXCapsule->SetLinearVelocityZero();
					InputCheck = false;
					WeaponIdle();
					Renderer->ChangeAnimation("pl0000_Gerbera_Jump_Left", true);
				},
				.Update = [=](float _DeltaTime) {
					if (Renderer->IsAnimationEnd())
					{
						ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					}
					if (true == Input_SpecialCheckFly()) { return; }
					if (false == InputCheck) { return; }
					if (true == FloorCheck())
					{
						ChangeState(FSM_State_Nero::Nero_Landing);
						return;
					}
					if (true == Input_JumpCheckFly()) { return; }
					if (true == Input_SwordCheckFly()) { return; }
					if (true == Input_GunCheckFly()) { return; }
					if (true == Input_DevilBreakerCheckFly()) { return; }
				},
				.End = [=] {
				}
				});

			// Nero_Gerbera_Right
			FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Gerbera_Right,
				.Start = [=] {
					PhysXCapsule->TurnOffGravity();
					PhysXCapsule->SetLinearVelocityZero();
					InputCheck = false;
					WeaponIdle();
					Renderer->ChangeAnimation("pl0000_Gerbera_Jump_Right", true);
				},
				.Update = [=](float _DeltaTime) {
					if (Renderer->IsAnimationEnd())
					{
						ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					}
					if (true == Input_SpecialCheckFly()) { return; }
					if (false == InputCheck) { return; }
					if (true == FloorCheck())
					{
						ChangeState(FSM_State_Nero::Nero_Landing);
						return;
					}
					if (true == Input_JumpCheckFly()) { return; }
					if (true == Input_SwordCheckFly()) { return; }
					if (true == Input_GunCheckFly()) { return; }
					if (true == Input_DevilBreakerCheckFly()) { return; }
				},
				.End = [=] {
				}
				});
	}
	// �����;�
	{}
	{
		// BusterArm Catch
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Catch,
			.Start = [=] {
				WeaponIdle();
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0000_Buster_Catch", true);
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					LookDir(Controller->GetMoveVector());
				}
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Input_SpecialCheck()) { return; }
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheck()) { return; }
				if (true == Input_SwordCheck()) { return; }
				if (true == Input_GunCheck()) { return; }
				if (true == Input_DevilBreakerCheck()) { return; }

				if (MoveCheck == false) { return; }

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
			}
			});
	}

	// Ư��
	{}
	{
		// DT Start
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_DT_Start,
		.Start = [=] {
			WeaponIdle();
			PhysXCapsule->SetLinearVelocityZero();
			Renderer->ChangeAnimation("pl0000_DT_Start", true);
			InputCheck = false;
			MoveCheck = false;
		},
		.Update = [=](float _DeltaTime) {
			if (true == Input_SpecialCheck()) { return; }
			if (InputCheck == false) { return; }
			if (false == FloorCheck())
			{
				ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				return;
			}


			if (true == Input_JumpCheck()) { return; }
			if (true == Input_SwordCheck()) { return; }
			if (true == Input_GunCheck()) { return; }
			if (true == Input_DevilBreakerCheck()) { return; }

			if (MoveCheck == false) { return; }

			if (Controller->GetMoveVector() != float4::ZERO)
			{
				ChangeState(FSM_State_Nero::Nero_RunStart);
				return;
			}
		},
		.End = [=] {
		}
			});
		// DT Air Start
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_DT_AirStart,
			.Start = [=] {
			WeaponIdle();
				InputCheck = false;
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_DT_AirStart", true);
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}

				if (true == Input_SpecialCheckFly()) { return; }
				if (InputCheck == false) { return; }
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
			}
			});

		// GT Bomb
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_GT_Bomb,
		.Start = [=] {
			WeaponIdle();
			PhysXCapsule->SetLinearVelocityZero();
			Renderer->ChangeAnimation("pl0000_GT_Bomb", true);
			InputCheck = false;
			MoveCheck = false;
		},
		.Update = [=](float _DeltaTime) {
			if (true == Input_SpecialCheck()) { return; }
			if (InputCheck == false) { return; }
			if (false == FloorCheck())
			{
				ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				return;
			}


			if (true == Input_JumpCheck()) { return; }
			if (true == Input_SwordCheck()) { return; }
			if (true == Input_GunCheck()) { return; }
			if (true == Input_DevilBreakerCheck()) { return; }

			if (MoveCheck == false) { return; }

			if (Controller->GetMoveVector() != float4::ZERO)
			{
				ChangeState(FSM_State_Nero::Nero_RunStart);
				return;
			}
		},
		.End = [=] {
		}
			});
		// GT Bomb Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_GT_AirBomb,
			.Start = [=] {
				WeaponIdle();
				InputCheck = false;
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_GT_AirBomb", true);
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}

				if (true == Input_SpecialCheckFly()) { return; }
				if (InputCheck == false) { return; }
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
			}
			});

		// EnemyStep
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_EnemyStep,
			.Start = [=] {
				WeaponIdle();
				UseDoubleJump = false;
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_EnemyStep", true);
			},
			.Update = [=](float _DeltaTime) {
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
				if (Renderer->IsAnimationEnd())
				{
					if ('2' == Controller->MoveVectorToChar4(Controller->GetMoveVector()))
					{
						ChangeState(FSM_State_Nero::Nero_2nd_Jump_Back);
						return;
					}
					ChangeState(FSM_State_Nero::Nero_2nd_Jump);
					return;
				}
			},
			.End = [=] {
			}
			});


		// Provocation 1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Provocation,
		.Start = [=] {
			PhysXCapsule->SetLinearVelocityZero();
			Renderer->ChangeAnimation("pl0000_Provocation", true);
			InputCheck = false;
			MoveCheck = false;
		},
		.Update = [=](float _DeltaTime) {
			if (InputCheck == false) { return; }
			if (false == FloorCheck())
			{
				ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				return;
			}


			if (true == Input_SpecialCheck()) { return; }
			if (true == Input_JumpCheck()) { return; }
			if (true == Input_SwordCheck()) { return; }
			if (true == Input_GunCheck()) { return; }
			if (true == Input_DevilBreakerCheck()) { return; }

			if (MoveCheck == false) { return; }

			if (Controller->GetMoveVector() != float4::ZERO)
			{
				ChangeState(FSM_State_Nero::Nero_RunStart);
				return;
			}
		},
		.End = [=] {
		}});

		// Provocation Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Provocation_Air,
			.Start = [=] {
				InputCheck = false;
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				RotationToMoveVector();
				PhysXCapsule->SetMove(Controller->GetMoveVector() * 350);
				Renderer->ChangeAnimation("pl0000_Air_Provocation", true);
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}

				if (true == Input_SpecialCheckFly()) { return; }
				if (InputCheck == false) { return; }
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
				WeaponIdle();
			}
			});

	}
	ChangeState(FSM_State_Nero::Nero_Idle);
}

void PlayerActor_Nero::NetLoad()
{

	// Renderer ����
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
		AnimationEvent::LoadAll({ .Dir = NewDir.GetFullPath().c_str(), .Renderer = Renderer,
			.Objects = { (GameEngineObject*)AttackCollision.get() },
			.CallBacks_void = {
				nullptr,
				std::bind(&PlayerActor_Nero::RedQueenOn, this),
				std::bind(&PlayerActor_Nero::RedQueenOff, this),
				std::bind(&PlayerActor_Nero::BlueRoseOn, this),
				std::bind(&PlayerActor_Nero::WeaponIdle, this),
			}
			});


		//Object = 0 : ���� �浹ü
		//
		//�ݹ�void = 0 : nullptr
		//�ݹ�void = 1 : �տ� ������ �����
		//�ݹ�void = 2 : �տ� ������ ��
		//�ݹ�void = 3 : �տ� ������
		//
		//�ݹ� int = 0 : FSM����
		// 

		SetDemon();
		WeaponIdle();
	}


	// �⺻ ������
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
	}
	{}
	// ���� ��
	{
		// RedQueen ComboA1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_ComboA_1,
			.Start = [=] {
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
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_HR-EX");
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
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Split_2_Loop");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
		// RedQueen Split_3
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Split_3,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Split_3");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				WeaponIdle();
			}
			});
	}
	{}
	// ��� ���� (����)
	{
		// Idle To LockOn
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_BR_Switch_Idle_to_Lockon");
			},
			.Update = [=](float _DeltaTime) {
				LookTarget();
			},
			.End = [=] {

			}
			});
		// LockOnFront
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Lockon_Front,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_BR_Lockon_Front");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Strafe
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Strafe,
			.Start = [=] {
				BlueRoseOn();
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
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
			} });
		// LockOn To Idle
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Switch_Lockon_to_Idle,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_BR_Switch_Lockon_to_Idle");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
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

			}
			});
		// Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Shoot,
			.Start = [=] {
				BlueRoseOn();
				Renderer->ChangeAnimation("pl0000_BR_Shoot", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {

			}
			});
		// Air Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_AirShoot,
			.Start = [=] {
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
	// �����߾�
	{}
	{
		// Nero_Overture_Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Overture_Shoot,
			.Start = [=] {
				WeaponIdle();
				SetOvertureAnimation();
				Renderer->ChangeAnimation("pl0000_Overture_Shoot");
				Renderer_Overture->On();
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
				WeaponIdle();
				SetOvertureAnimation();
				Renderer->ChangeAnimation("pl0000_Overture_Air_Shoot");
				Renderer_Overture->On();
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
	// �ź���
	{}
	{
		// Nero_Gerbera_Back
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Gerbera_Back,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Gerbera_Jump_Back");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
	}
	FSM.ChangeState(FSM_State_Nero::Nero_Idle);
}

void PlayerActor_Nero::Update_Character(float _DeltaTime)
{
	if (LoadCheck == false) { return; }
	FSM.Update(_DeltaTime);

	if (NetControllType::UserControll == GameEngineNetObject::GetControllType())
	{
		if (GameEngineInput::IsDown("Escape"))
		{
			SetWorldPosition({ 0, 100, 0 });
			PhysXCapsule->SetWorldRotation({0, 0, 0});
			ChangeState(Nero_Idle);
		}
		if (GameEngineInput::IsDown("SelectLevel_01"))
		{
			AddBreaker(DevilBreaker::Overture);
		}
		if (GameEngineInput::IsDown("SelectLevel_02"))
		{
			AddBreaker(DevilBreaker::Gerbera);
		}
		if (GameEngineInput::IsDown("SelectLevel_03"))
		{
			AddBreaker(DevilBreaker::BusterArm);
		}
	}
}

bool PlayerActor_Nero::Input_SwordCheck(int AddState /*= 0*/)
{
	if (false == Controller->GetIsLockOn())
	{
		if (Controller->GetIsAnySword())
		{
			ChangeState(AddState + Nero_RQ_ComboA_1);
			return true;
		}
		return false;
	}

	if (Controller->GetIsBackFrontSword())
	{
		ChangeState(FSM_State_Nero::Nero_RQ_Skill_Shuffle);
		return true;
	}
	if (Controller->GetIsFrontSword())
	{
		ChangeState(FSM_State_Nero::Nero_RQ_Skill_Stleak1);
		return true;
	}
	if (Controller->GetIsBackSword())
	{
		ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
		return true;
	}
	if (Controller->GetIsSword())
	{
		ChangeState(AddState + Nero_RQ_ComboA_1);
		return true;
	}
	return false;
}

bool PlayerActor_Nero::Input_SwordCheckFly(int AddState /*= 0*/)
{
	if (false == Controller->GetIsLockOn())
	{
		if (Controller->GetIsAnySword())
		{
			ChangeState(AddState + Nero_RQ_AirComboA_1);
			return true;
		}
		return false;
	}

	if (Controller->GetIsBackFrontSword())
	{
		ChangeState(FSM_State_Nero::Nero_RQ_Skill_Caliber_1);
		return true;
	}
	if (Controller->GetIsBackSword())
	{
		ChangeState(FSM_State_Nero::Nero_RQ_Skill_Split_1);
		return true;
	}
	if (Controller->GetIsSword())
	{
		ChangeState(AddState + Nero_RQ_AirComboA_1);
		return true;
	}
	return false;
}

bool PlayerActor_Nero::Input_GunCheck()
{
	if (Controller->GetGunUp())
	{
		ChangeState(FSM_State_Nero::Nero_BR_Shoot);
		return true;
	}
	return false;
}

bool PlayerActor_Nero::Input_GunCheckFly()
{
	if (Controller->GetGunUp())
	{
		ChangeState(FSM_State_Nero::Nero_BR_AirShoot);
		return true;
	}
	return false;
}

bool PlayerActor_Nero::Input_JumpCheck()
{
	if (Controller->GetIsProvocation())
	{
		ChangeState(FSM_State_Nero::Nero_Provocation);
		return true;
	}

	if (false == Controller->GetIsLockOn())
	{
		if (Controller->GetIsAnyJump())
		{
			if ('2' == Controller->MoveVectorToChar4(Controller->GetMoveVector()))
			{
				ChangeState(FSM_State_Nero::Nero_Jump_Back);
				return true;
			}
			ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
			return true;
		}
		return false;
	}


	if (Controller->GetIsLeftJump())
	{
		ChangeState(FSM_State_Nero::Nero_Evade_Left);
		return true;
	}
	if (Controller->GetIsRightJump())
	{
		ChangeState(FSM_State_Nero::Nero_Evade_Right);
		return true;
	}
	if (Controller->GetIsJump())
	{
		if ('2' == Controller->MoveVectorToChar4(Controller->GetMoveVector()))
		{
			ChangeState(FSM_State_Nero::Nero_Jump_Back);
			return true;
		}
		ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
		return true;
	}
	return false;
}

bool PlayerActor_Nero::Input_JumpCheckFly()
{
	if (Controller->GetIsProvocation())
	{
		ChangeState(FSM_State_Nero::Nero_Provocation_Air);
		return true;
	}

	if (true == UseDoubleJump)
	{
		return false;
	}
	if (Controller->GetIsAnyJump())
	{
		UseDoubleJump = true;
		if ('2' == Controller->MoveVectorToChar4(Controller->GetMoveVector()))
		{
			ChangeState(FSM_State_Nero::Nero_2nd_Jump_Back);
			return true;
		}
		ChangeState(FSM_State_Nero::Nero_2nd_Jump);
		return true;
	}
	return false;
}

bool PlayerActor_Nero::Input_DevilBreakerCheck()
{
	if (false == Controller->GetIsSkill())
	{
		return false;
	}
	if (CurDevilBreaker == DevilBreaker::None)
	{
		return false;
	}

	switch (CurDevilBreaker)
	{
	case DevilBreaker::Overture:
		ChangeState(FSM_State_Nero::Nero_Overture_Shoot);
		break;
	case DevilBreaker::Gerbera:
	{
		char Dir = Controller->MoveVectorToChar4(Controller->GetMoveVector());
		if (Dir == '4')
		{
			ChangeState(FSM_State_Nero::Nero_Gerbera_Right);
		}
		else if (Dir == '6')
		{
			ChangeState(FSM_State_Nero::Nero_Gerbera_Left);
		}
		else if (Dir == '2')
		{
			ChangeState(FSM_State_Nero::Nero_Gerbera_Front);
		}
		else
		{
			ChangeState(FSM_State_Nero::Nero_Gerbera_Back);
		}
		break;
	}
	case DevilBreaker::BusterArm:
		ChangeState(FSM_State_Nero::Nero_Buster_Catch);
		break;
	}
	return true;
}

bool PlayerActor_Nero::Input_DevilBreakerCheckFly()
{
	if (false == Controller->GetIsSkill())
	{
		return false;
	}
	if (CurDevilBreaker == DevilBreaker::None)
	{
		return false;
	}

	switch (CurDevilBreaker)
	{
	case DevilBreaker::Overture:
		ChangeState(FSM_State_Nero::Nero_Overture_AirShoot);
		break;
	case DevilBreaker::Gerbera:
	{
		char Dir = Controller->MoveVectorToChar4(Controller->GetMoveVector());
		if (Dir == '4')
		{
			ChangeState(FSM_State_Nero::Nero_Gerbera_Right);
		}
		else if (Dir == '6')
		{
			ChangeState(FSM_State_Nero::Nero_Gerbera_Left);
		}
		else if (Dir == '2')
		{
			ChangeState(FSM_State_Nero::Nero_Gerbera_Front);
		}
		else
		{
			ChangeState(FSM_State_Nero::Nero_Gerbera_Back);
		}
		break;
	}
	case DevilBreaker::BusterArm:
		ChangeState(FSM_State_Nero::Nero_Buster_Catch);
		break;
	}
	return true;
}

bool PlayerActor_Nero::Input_SpecialCheck()
{
	if (Controller->GetIsDevilTrigger())
	{
		if (false == IsDevilTrigger)
		{
			IsDevilTrigger = true;
			ChangeState(FSM_State_Nero::Nero_DT_Start);
			return true;
		}
		IsDevilTrigger = false;
		SetHuman();
		SetOverture();
		return true;
	}
	if (Controller->GetIsGTBomb() && CurDevilBreaker != DevilBreaker::None)
	{
		ChangeState(FSM_State_Nero::Nero_GT_Bomb);
		return true;
	}

	return false;
}

bool PlayerActor_Nero::Input_SpecialCheckFly()
{
	if (Controller->GetIsAnyJump())
	{
		std::vector<std::shared_ptr<GameEngineCollision>> Cols;
		if (true == EnemyStepCheckCollision->CollisionAll(CollisionOrder::Enemy, Cols))
		{
			ChangeState(FSM_State_Nero::Nero_EnemyStep);
			return true;
		}
	}
	if (Controller->GetIsDevilTrigger())
	{
		if (false == IsDevilTrigger)
		{
			IsDevilTrigger = true;
			ChangeState(FSM_State_Nero::Nero_DT_AirStart);
			return true;
		}
		IsDevilTrigger = false;
		SetHuman();
		SetOverture();
		return true;
	}
	if (Controller->GetIsGTBomb() && CurDevilBreaker != DevilBreaker::None)
	{
		ChangeState(FSM_State_Nero::Nero_GT_AirBomb);
		return true;
	}

	return false;
}

//void PlayerActor_Nero::ChangeState(FSM_State_Nero _StateValue)
//{
//	FSM.ChangeState(_StateValue);
//	FSMValue = _StateValue;
//	NetworkManager::SendFsmChangePacket(this, _StateValue);
//}

void PlayerActor_Nero::ChangeState(int _StateValue)
{
	FSM.ChangeState(_StateValue);
	FSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
}

/*
0 12~13 ����Ʈ ��
0 14 �����߾�
0 15~16 �ź���
0 17 �� ������
0 18 �� ������
0 19 �� ��� ����
*/
void PlayerActor_Nero::RedQueenOn()
{
	Renderer->GetAllRenderUnit()[0][15]->On();	// �� ������
	Renderer->GetAllRenderUnit()[0][24]->Off();	// �� ������
	Renderer->GetAllRenderUnit()[0][16]->Off();	// ������
}

void PlayerActor_Nero::RedQueenOff()
{
	Renderer->GetAllRenderUnit()[0][15]->Off();	// �� ������
	Renderer->GetAllRenderUnit()[0][24]->On();	// �� ������
}

void PlayerActor_Nero::BlueRoseOn()
{
	Renderer->GetAllRenderUnit()[0][15]->Off();	// �� ������
	Renderer->GetAllRenderUnit()[0][24]->On();	// �� ������
	Renderer->GetAllRenderUnit()[0][16]->On();	// ������
}

void PlayerActor_Nero::WeaponIdle()
{
	Renderer->GetAllRenderUnit()[0][15]->Off();	// �� ������
	Renderer->GetAllRenderUnit()[0][24]->On();	// �� ������
	Renderer->GetAllRenderUnit()[0][16]->Off();	// ������
	Renderer_Overture->Off();
	if (true == IsDevilTrigger)
	{
		return;
	}
	OnDevilBraeker();
}

void PlayerActor_Nero::SetHuman()
{
	for (int i = 0; i <= 14; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->On();
	}

	for (int i = 17; i <= 23; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->Off();
	}
}

void PlayerActor_Nero::SetDemon()
{
	for (int i = 0; i <= 8; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->Off();
	}
	Renderer->GetAllRenderUnit()[0][14]->Off();

	for (int i = 17; i <= 23; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->On();
	}
	Renderer->GetAllRenderUnit()[0][13]->Off();
	Renderer->GetAllRenderUnit()[0][12]->Off();
	Renderer->GetAllRenderUnit()[0][11]->Off();
	Renderer->GetAllRenderUnit()[0][10]->Off();
	Renderer->GetAllRenderUnit()[0][9]->Off();
}

void PlayerActor_Nero::SetOverture()
{
	if (true == IsDevilTrigger)
	{
		return;
	}
	Renderer->GetAllRenderUnit()[0][13]->On();
	Renderer->GetAllRenderUnit()[0][12]->Off();
	Renderer->GetAllRenderUnit()[0][11]->Off();
	Renderer->GetAllRenderUnit()[0][10]->Off();
	Renderer->GetAllRenderUnit()[0][9]->Off();
	Renderer_Overture->Off();
}

void PlayerActor_Nero::SetGerbera()
{
	if (true == IsDevilTrigger)
	{
		return;
	}
	Renderer->GetAllRenderUnit()[0][13]->Off();
	Renderer->GetAllRenderUnit()[0][12]->On();
	Renderer->GetAllRenderUnit()[0][11]->On();
	Renderer->GetAllRenderUnit()[0][10]->Off();
	Renderer->GetAllRenderUnit()[0][9]->Off();
	Renderer_Overture->Off();
}

void PlayerActor_Nero::SetBusterArm()
{
	if (true == IsDevilTrigger)
	{
		return;
	}
	Renderer->GetAllRenderUnit()[0][13]->Off();
	Renderer->GetAllRenderUnit()[0][12]->Off();
	Renderer->GetAllRenderUnit()[0][11]->Off();
	Renderer->GetAllRenderUnit()[0][10]->On();
	Renderer->GetAllRenderUnit()[0][9]->On();
	Renderer_Overture->Off();
}

void PlayerActor_Nero::SetOvertureAnimation()
{
	if (true == IsDevilTrigger)
	{
		return;
	}
	Renderer->GetAllRenderUnit()[0][13]->Off();
	Renderer->GetAllRenderUnit()[0][12]->Off();
	Renderer->GetAllRenderUnit()[0][11]->Off();
	Renderer->GetAllRenderUnit()[0][10]->Off();
	Renderer->GetAllRenderUnit()[0][9]->Off();
	Renderer_Overture->On();
}

void PlayerActor_Nero::OnDevilBraeker()
{
	switch (CurDevilBreaker)
	{
	case DevilBreaker::None:
		break;
	case DevilBreaker::Overture:
		SetOverture();
		break;
	case DevilBreaker::Gerbera:
		SetGerbera();
		break;
	case DevilBreaker::BusterArm:
		SetBusterArm();
		break;
	default:
		break;
	}
}

void PlayerActor_Nero::OffDevilBraeker()
{
	Renderer->GetAllRenderUnit()[0][13]->Off();
	Renderer->GetAllRenderUnit()[0][12]->Off();
	Renderer->GetAllRenderUnit()[0][11]->Off();
	Renderer->GetAllRenderUnit()[0][10]->Off();
	Renderer->GetAllRenderUnit()[0][9]->Off();
}

void PlayerActor_Nero::AddBreaker(DevilBreaker _Breaker)
{
	CurDevilBreaker = _Breaker;
	switch (CurDevilBreaker)
	{
	case DevilBreaker::None:
		break;
	case DevilBreaker::Overture:
		BreakerStack.push(DevilBreaker::Overture);
		SetOverture();
		break;
	case DevilBreaker::Gerbera:
		BreakerStack.push(DevilBreaker::Gerbera);
		SetGerbera();
		break;
	case DevilBreaker::BusterArm:
		BreakerStack.push(DevilBreaker::BusterArm);
		SetBusterArm();
		break;
	default:
		break;
	}
}

void PlayerActor_Nero::DestroyBreaker()
{
	if (BreakerStack.top() == DevilBreaker::None) { return; }
	BreakerStack.pop();
	CurDevilBreaker = BreakerStack.top();
	switch (CurDevilBreaker)
	{
	case DevilBreaker::None:
		OffDevilBraeker();
		break;
	case DevilBreaker::Overture:
		SetOverture();
		break;
	case DevilBreaker::Gerbera:
		SetGerbera();
		break;
	case DevilBreaker::BusterArm:
		SetBusterArm();
		break;
	default:
		break;
	}
}

/*
�׷� ���� ����

0 �ΰ���
1 �ΰ���
2 �ΰ���
3 �ΰ���
4 �ΰ� ����
5 �ΰ� ��
6 �ΰ� �Ӵ���
7 �ΰ��Ӹ�
8 �ΰ����
9 �����;�
10 �����;�
11 �ź���
12 �ź���
13 �����߾�
14 �ΰ� �����
15 �� ������
16 �� ������

17 DT ��
18 DT ��
19 DT �չ�
20 DT ��
21 DT ���
22 DT ���
23 DT ����
24 �� ������
*/