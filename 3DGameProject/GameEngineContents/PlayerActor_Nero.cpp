#include "PrecompileHeader.h"
#include "PlayerActor_Nero.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "AnimationEvent.h"
#include "EffectFBXRenderer.h"
#include "PlayerController.h"
#include "NetworkManager.h"
#include "AttackCollision.h"
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

	SetNetObjectType(Net_ActorType::Nero);

	//NetControllType::NetControll으로 변경될 때 아래 콜백이 실행됩니다. 
	SetControllCallBack(NetControllType::NetControll, [=]()
		{
			NetControllLoad();
			NetLoad();
			LoadCheck = true;
		});

	//NetControllType::UserControll으로 변경될 때 아래 콜백이 실행됩니다.
	SetControllCallBack(NetControllType::UserControll, [=]()
		{
			UserControllLoad();
			PlayerLoad();
			LoadCheck = true;
		});
}

void PlayerActor_Nero::PlayerLoad()
{
	// Effect 생성
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
		Renderer_EffectMesh = CreateComponent<EffectFBXRenderer>();
		Renderer_EffectMesh->SetFBXMesh("Effect_Mesh_01.FBX", "ClipEffect");
		Renderer_EffectMesh->SetTexture("DiffuseTexture", "Effect_Texture_02.tga");
		Renderer_EffectMesh->Off();
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

		Renderer = CreateComponent<GameEngineFBXRenderer>();
		Renderer->GetTransform()->SetLocalRotation({ 0, 0, 0 });
		Renderer->GetTransform()->SetLocalPosition({ 0, -75, 0 });
		
		//Renderer->SetBaseColor(float4(2, 0, 0));

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
			.Objects = { (GameEngineObject*)Col_Attack.get() },
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

	SetHuman();
	AddBreaker(DevilBreaker::BusterArm);
	AddBreaker(DevilBreaker::Gerbera);
	AddBreaker(DevilBreaker::Overture);
	WeaponIdle();

	// 기본 움직임
	{
		// Idle
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Idle,
			.Start = [=] {
				PhysXCapsule->TurnOffGravity();
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
				PhysXCapsule->TurnOnGravity();
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
			}
			});
		// RunStop
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RunStop,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
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
				MoveCheck = false;
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
				
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }

				PhysXCapsule->SetForce(Controller->GetMoveVector() * 3500);

				if (false == MoveCheck) { return; }
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
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
				MoveCheck = false;
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
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }

				PhysXCapsule->SetForce(Controller->GetMoveVector() * 3500);

				if (false == MoveCheck) { return; }
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
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
				PhysXCapsule->TurnOnGravity();
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
				MoveCheck = false;
				RotationToMoveVector();
				PhysXCapsule->TurnOnGravity();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->SetMove(Controller->GetMoveVector() * 250);
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_2ndJump", true);
			},
			.Update = [=](float _DeltaTime) {
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

				if (false == MoveCheck) { return; }
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
			},
			.End = [=] {
			}
			});
		// 2nd_Jump_Back
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_2nd_Jump_Back,
			.Start = [=] {
				InputCheck = false;
				MoveCheck = false;
				PhysXCapsule->TurnOnGravity();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->SetMove(Controller->GetMoveVector() * 250);
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_Back_2ndJump", true);
			},
			.Update = [=](float _DeltaTime) {
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

				if (false == MoveCheck) { return; }
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
			},
			.End = [=] {
			}
			});

		// EnemyStep Jump
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_EnemyStep_Jump,
			.Start = [=] {
				InputCheck = false;
				MoveCheck = false;
				PhysXCapsule->TurnOnGravity();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->SetMove(Controller->GetMoveVector() * 250);
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_EnemyStep", true);
			},
			.Update = [=](float _DeltaTime) {
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

				if (false == MoveCheck) { return; }
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
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
				Col_Attack->SetAttackData(DamageType::Light, 50);
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
				PhysXCapsule->SetLinearVelocityZero();
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
				if (true == Col_Player->CollisionAll(CollisionOrder::Enemy, Cols))
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
				if (true == Col_Player->CollisionAll(CollisionOrder::Enemy, Cols))
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
	// 블루 로즈 (락온)
	{
		// Idle To LockOn
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon,
			.Start = [=] {
				PhysXCapsule->TurnOffGravity();
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
				PhysXCapsule->TurnOffGravity();
				BlueRoseOn();
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
				WeaponIdle();
			}
			});
		// Strafe
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Strafe,
			.Start = [=] {
				PhysXCapsule->TurnOnGravity();
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
				WeaponIdle();
				PhysXCapsule->SetLinearVelocityZero();
			} });
		// LockOn To Idle
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Switch_Lockon_to_Idle,
			.Start = [=] {
				PhysXCapsule->TurnOffGravity();
				BlueRoseOn();
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
				WeaponIdle();
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
				WeaponIdle();
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
				WeaponIdle();
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
	// 오버추어
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
	// 거베라
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
	// 버스터암
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

				if (true == Controller->GetIsSkill())
				{
					// 임시용
					ChangeState(FSM_State_Nero::Nero_Buster_Repelled);
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

		// BusterArm Strike
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Strike,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Buster_Strike_Common");
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				
				if (InputCheck == false) { return; }

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
			}
			});

		// BusterArm Repelled
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Repelled,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Buster_Catch_Repelled");
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}


				if (InputCheck == false) { return; }

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
			}
			});

		// BusterArm Catch Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Catch_Air,
			.Start = [=] {
				WeaponIdle();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity	();
				Renderer->ChangeAnimation("pl0000_Buster_Air_Catch", true);
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

				if (true == Controller->GetIsSkill())
				{
					// 임시용
					ChangeState(FSM_State_Nero::Nero_Buster_Strike_Air);
					return;
				}

				if (true == Input_SpecialCheckFly()) { return; }
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
			}
			});

		// BusterArm Strike Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Strike_Air,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Buster_Air_Strike_Common");
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
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
			}
			});

		// BusterArm Repelled Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Repelled_Air,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Buster_Air_Catch_Repelled");
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
				if (InputCheck == false) { return; }

				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
			}
			});
	}
	// 스내치
	{}
	{
		// Snatch Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Shoot,
			.Start = [=] {
				WeaponIdle();
				PhysXCapsule->SetLinearVelocityZero();
				RotationToTarget();
				Renderer->ChangeAnimation("pl0000_Snatch", true);
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (true == Controller->GetIsLockOnSkill())
				{
					// 임시용
					ChangeState(FSM_State_Nero::Nero_Snatch_Pull);
					return;
				}

				if (InputCheck == false) { return; }

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
			}
			});

		// Snatch Pull
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Pull,
			.Start = [=] {
				RotationToTarget();
				Renderer->ChangeAnimation("pl0000_Snatch_Pull");
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (InputCheck == false) { return; }

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
			}
			});

		// Snatch Repel
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Repel,
			.Start = [=] {
				RotationToTarget();
				Renderer->ChangeAnimation("pl0000_Snatch_Repel");
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (false == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (InputCheck == false) { return; }

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
			}
			});

		// Snatch Shoot Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Shoot_Air,
			.Start = [=] {
				WeaponIdle();
				PhysXCapsule->TurnOffGravity();
				PhysXCapsule->SetLinearVelocityZero();
				RotationToTarget();
				Renderer->ChangeAnimation("pl0000_Air_Snatch", true);
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

				if (true == Controller->GetIsLockOnSkill())
				{
					// 임시용
					ChangeState(FSM_State_Nero::Nero_Snatch_Repel_Air);
					return;
				}

				if (InputCheck == false) { return; }

				if (true == Input_SpecialCheckFly()) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
			}
			});

		// Snatch Pull Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Pull_Air,
			.Start = [=] {
				RotationToTarget();
				Renderer->ChangeAnimation("pl0000_Air_Snatch_Pull", true);
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

				if (InputCheck == false) { return; }

				if (true == Input_SpecialCheckFly()) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
			},
			.End = [=] {
			}
			});

		// Snatch Repel Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Repel_Air,
			.Start = [=] {
				RotationToTarget();
				Renderer->ChangeAnimation("pl0000_Air_Snatch_Repel", true);
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

				if (InputCheck == false) { return; }

				if (true == Input_SpecialCheckFly()) { return; }
				if (true == Input_JumpCheckFly()) { return; }
				if (true == Input_SwordCheckFly()) { return; }
				if (true == Input_GunCheckFly()) { return; }
				if (true == Input_DevilBreakerCheckFly()) { return; }
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
			if (InputCheck == false) { return; }
			if (true == Input_SpecialCheck()) { return; }
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

				if (InputCheck == false) { return; }
				if (true == Input_SpecialCheckFly()) { return; }
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
					ChangeState(FSM_State_Nero::Nero_EnemyStep_Jump);
					return;
				}
			},
			.End = [=] {
			}
			});


		// Provocation 1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Provocation,
		.Start = [=] {
			WeaponIdle();
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
				WeaponIdle();
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
	// 대미지
	{}
	{
		// Damage Common
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Common,
			.Start = [=] {
				WeaponIdle();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_Damage_Common", true);
				InputCheck = false;
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
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
			}
			});

		// Damage Combo
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Combo,
			.Start = [=] {
				WeaponIdle();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_Damage_Combo", true);
				InputCheck = false;
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
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
			}
			});

		// Damage Fly
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Fly,
			.Start = [=] {
				WeaponIdle();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_Damage_AirCombo_Fly", true);
			},
			.Update = [=](float _DeltaTime) {
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Damage_Fall);
					return;
				}
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Damage_Ground);
					return;
				}
			},
			.End = [=] {
			}
			});

		// Damage Fall
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Fall,
			.Start = [=] {
				PhysXCapsule->SetPush({ 0, -1000, 0 });
				PhysXCapsule->TurnOnGravity();
				Renderer->ChangeAnimation("pl0000_Damage_Supine_Fall_Loop");
			},
			.Update = [=](float _DeltaTime) {
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Damage_Ground);
					return;
				}
			},
			.End = [=] {
			}
			});

		// Damage Ground
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Ground,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_Damage_Away_Ground");
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Damage_GetUp);
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
			},
			.End = [=] {
			}
			});

	}

	ChangeState(FSM_State_Nero::Nero_Idle);
}

void PlayerActor_Nero::NetLoad()
{
	// Effect 생성
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
		Renderer_EffectMesh = CreateComponent<EffectFBXRenderer>();
		Renderer_EffectMesh->SetFBXMesh("Effect_Mesh_01.FBX", "ClipEffect");
		Renderer_EffectMesh->SetTexture("DiffuseTexture", "Effect_Texture_01.png");
		Renderer_EffectMesh->Off();
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
			.Objects = { (GameEngineObject*)Col_Attack.get() },
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
				std::bind(&GameEngineFSM::ChangeState, &FSM, std::placeholders::_1)
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

	SetHuman();
	AddBreaker(DevilBreaker::BusterArm);
	AddBreaker(DevilBreaker::Gerbera);
	AddBreaker(DevilBreaker::Overture);
	WeaponIdle();

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
				RedQueenOff();
				WeaponIdle();
			}
			});
		// RedQueen Shuffle
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Shuffle,
			.Start = [=] {
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
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Stleak_1");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				RedQueenOff();
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
				RedQueenOff();
			}
			});
		// RedQueen Stleak3
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Stleak3,
			.Start = [=] {
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Stleak_3");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				RedQueenOff();
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
		static float4 SplitTargetPos;
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
			}
			});
		// RedQueen Caliber_1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Caliber_1,
			.Start = [=] {
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
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Caliber_2");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
				RedQueenOff();
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
				LookTarget();
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
				WeaponIdle();
				SetOvertureAnimation();
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
				WeaponIdle();
				SetOvertureAnimation();
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
				WeaponIdle();
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
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Gerbera_Jump_Front", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Nero_Gerbera_Left
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Gerbera_Left,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Gerbera_Jump_Left", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Nero_Gerbera_Right
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Gerbera_Right,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Gerbera_Jump_Right", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});
	}
	// 버스터암
	{}
	{
		// BusterArm Catch
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Catch,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Buster_Catch", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// BusterArm Strike
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Strike,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_Buster_Strike_Common");
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// BusterArm Repelled
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Repelled,
			.Start = [=] {
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
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Buster_Air_Catch", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// BusterArm Strike Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Buster_Strike_Air,
			.Start = [=] {
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
		// Snatch Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Shoot,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Snatch", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Snatch Pull
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Pull,
			.Start = [=] {
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
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Air_Snatch", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Snatch Pull Air
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Snatch_Pull_Air,
			.Start = [=] {
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
			WeaponIdle();
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
				WeaponIdle();
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
			WeaponIdle();
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
				WeaponIdle();
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
		// Damage Common
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Common,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Damage_Common", true);
			},
			.Update = [=](float _DeltaTime) {
			},
			.End = [=] {
			}
			});

		// Damage Combo
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Damage_Combo,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Damage_Combo", true);
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
			ChangeState(FSM_State_Nero::Nero_Damage_Fly);
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
	if (true == Controller->GetIsSkill())
	{
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

	if (true == Controller->GetIsLockOnSkill())
	{
		ChangeState(FSM_State_Nero::Nero_Snatch_Shoot);
		return true;
	}

	return false;
}

bool PlayerActor_Nero::Input_DevilBreakerCheckFly()
{
	if (true == Controller->GetIsSkill())
	{

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
			ChangeState(FSM_State_Nero::Nero_Buster_Catch_Air);
			break;
		}
		return true;
	}

	if (true == Controller->GetIsLockOnSkill())
	{
		ChangeState(FSM_State_Nero::Nero_Snatch_Shoot_Air);
		return true;
	}

	return false;
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
		if (true == Col_EnemyStepCheck->CollisionAll(CollisionOrder::EnemyAttack, Cols))
		{
			ChangeState(FSM_State_Nero::Nero_EnemyStep);
			return true;
		}
		if (false == UseDoubleJump && PhysXCapsule->GetIsPlayerWallTouch())
		{
			ChangeState(FSM_State_Nero::Nero_EnemyStep);
			UseDoubleJump = true;
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
0 12~13 버스트 암
0 14 오버추어
0 15~16 거베라
0 17 손 레드퀸
0 18 등 레드퀸
0 19 손 블루 로즈
*/
void PlayerActor_Nero::RedQueenOn()
{
	Renderer->GetAllRenderUnit()[0][15]->On();	// 손 레드퀸
	Renderer->GetAllRenderUnit()[0][24]->Off();	// 등 레드퀸
	Renderer->GetAllRenderUnit()[0][16]->Off();	// 블루로즈
}

void PlayerActor_Nero::RedQueenOff()
{
	Renderer->GetAllRenderUnit()[0][15]->Off();	// 손 레드퀸
	Renderer->GetAllRenderUnit()[0][24]->On();	// 등 레드퀸
}

void PlayerActor_Nero::BlueRoseOn()
{
	Renderer->GetAllRenderUnit()[0][15]->Off();	// 손 레드퀸
	Renderer->GetAllRenderUnit()[0][24]->On();	// 등 레드퀸
	Renderer->GetAllRenderUnit()[0][16]->On();	// 블루로즈
}

void PlayerActor_Nero::WeaponIdle()
{
	Renderer->GetAllRenderUnit()[0][15]->Off();	// 손 레드퀸
	Renderer->GetAllRenderUnit()[0][24]->On();	// 등 레드퀸
	Renderer->GetAllRenderUnit()[0][16]->Off();	// 블루로즈
	Renderer_Overture->Off();
	if (true == IsDevilTrigger)
	{
		return;
	}
	OnDevilBraeker();
}

void PlayerActor_Nero::SetHuman()
{
	DTValue = 0;
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
	DTValue = 1;
	ArmValue = 0;
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
	ArmValue = 1;
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
	ArmValue = 2;
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
	ArmValue = 3;
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
	ArmValue = 0;
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
네로 렌더 유닛

0 인간팔
1 인간옷
2 인간옷
3 인간옷
4 인간 눈썹
5 인간 눈
6 인간 속눈썹
7 인간머리
8 인간헤어
9 버스터암
10 버스터암
11 거베라
12 거베라
13 오버추어
14 인간 기계팔
15 손 레드퀸
16 손 블루로즈

17 DT 얼굴
18 DT 몸
19 DT 손발
20 DT 눈
21 DT 헤어
22 DT 헤어
23 DT 날개
24 등 레드퀸
*/