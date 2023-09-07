#include "PrecompileHeader.h"
#include "PlayerActor_Nero.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include "AnimationEvent.h"
#include "PlayerController.h"
#include "NetworkManager.h"
PlayerActor_Nero::~PlayerActor_Nero()
{
}

void PlayerActor_Nero::SinglePlayLoad()
{
	if (false == GameEngineInput::IsKey("Escape"))
	{
		GameEngineInput::CreateKey("Escape", VK_F10);
	}
	UserControllLoad();
	PlayerLoad();
	LoadCheck = true;
}

void PlayerActor_Nero::Start()
{
	BasePlayerActor::Start();

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
			if (false == GameEngineInput::IsKey("Escape"))
			{
				GameEngineInput::CreateKey("Escape", VK_F10);
			}
			PlayerLoad();
			LoadCheck = true;
		});


	/*if (NetControllType::NetControll == GameEngineNetObject::GetControllType())
	{
		NetLoad();
	}
	else if (NetControllType::UserControll == GameEngineNetObject::GetControllType())
	{
		if (false == GameEngineInput::IsKey("Escape"))
		{
			GameEngineInput::CreateKey("Escape", VK_F10);
		}
		PlayerLoad();
	}*/
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
		Renderer_EffectMesh = CreateComponent<GameEngineFBXRenderer>();
		Renderer_EffectMesh->SetFBXMesh("Effect_Mesh_01.FBX", "FBX");
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
			},
			.CallBacks_int = {
				std::bind(&GameEngineFSM::ChangeState, &FSM, std::placeholders::_1)
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
		//콜백void = 7 : 딜레이체크 시작
		//콜백void = 8 : 중력 적용
		//콜백void = 9 : SetOverture
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
		Renderer_Overture->SetFBXMesh("Overture.FBX", "MeshAniTexture");
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

	// 기본 움직임
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
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetIsSkill())
				{
					ChangeState(FSM_State_Nero::Nero_Overture_Shoot);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}

				if (true == IsLockOn)
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
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStop);
					return;
				}
				if (true == IsLockOn)
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
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
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStop);
					return;
				}
				if (true == IsLockOn)
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
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
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
				if (true == IsLockOn)
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
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
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_DashStop);
					return;
				}
				if (true == IsLockOn)
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
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
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
				if (true == IsLockOn)
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
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
				PhysXCapsule->TurnOnGravity();
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
				if (false == InputCheck) { return; }
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_AirShoot);
					return;
				}
				if (Controller->GetIsBackFrontSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_Caliber_1);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_Split_1);
					return;
				}
				if (Controller->GetIsSkill())
				{
					ChangeState(FSM_State_Nero::Nero_Overture_AirShoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_AirComboA_1);
					return;
				}

				PhysXCapsule->SetForce(Controller->GetMoveVector() * 5000);
			},
			.End = [=] {
			}
			});
		// Jump Fly
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Jump_Fly,
			.Start = [=] {
				PhysXCapsule->SetPush(float4::DOWN * 1000);
				PhysXCapsule->TurnOnGravity();
				Renderer->ChangeAnimation("pl0000_Jump_Fly_loop");
			},
			.Update = [=](float _DeltaTime) {
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_AirShoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_Split_1);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_AirComboA_1);
					return;
				}

				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
			},
			.End = [=] {
				PhysXCapsule->TurnOffGravity();
			}
			});
		// Landing
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Landing,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_Landing");
			},
			.Update = [=](float _DeltaTime) {
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
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
	}
	{}
	// 레드 퀸
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
				if (InputCheck == false) { return; }
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_2);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

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
				if (InputCheck == false) { return; }
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					if (true == DelayCheck)
					{
						ChangeState(FSM_State_Nero::Nero_RQ_ComboD_1);
						return;
					}
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_3);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

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
				if (InputCheck == false) { return; }
				if (true == PhysXCapsule->GetIsPlayerGroundTouch())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_4);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

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
				if (InputCheck == false) { return; }
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

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
				if (InputCheck == false) { return; }
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboD_2);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

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
				if (InputCheck == false) { return; }
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboD_3);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

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
				if (InputCheck == false) { return; }
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboD_4);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

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
				if (InputCheck == false) { return; }
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

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
				PhysXCapsule->SetLinearVelocityZero();
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_HR-EX");
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
				if (InputCheck == false) { return; }
				if (Controller->GetIsLeftJump())
				{
					//ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					//ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					//ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_AirShoot);
					return;
				}
			},
			.End = [=] {
				WeaponIdle();
			}
			});
		// RedQueen AirComboA1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_AirComboA_1,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_RQ_AT-Jump_1");
				RotationToTarget(30.0f);
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (InputCheck == false) { return; }
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_AirShoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_AirComboA_2);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
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
				RotationToTarget(30.0f);
				InputCheck = false;
				DelayCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (InputCheck == false) { return; }
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_AirShoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					if (DelayCheck == true) {
						ChangeState(FSM_State_Nero::Nero_RQ_AirComboB);
						return;
					}

					ChangeState(FSM_State_Nero::Nero_RQ_AirComboA_3);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
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
				RotationToTarget(30.0f);
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (InputCheck == false) { return; }
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_AirShoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_AirComboA_1);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}
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
				if (true == FloorCheck())
				{
					ChangeState(FSM_State_Nero::Nero_Landing);
					return;
				}
				if (InputCheck == false) { return; }
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_AirShoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_AirComboA_1);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
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
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Split_1");
			},
			.Update = [=](float _DeltaTime) {
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
				PhysXCapsule->SetMove(float4::DOWN * 1500);
				if (GetTransform()->GetWorldPosition().y < SplitTargetPos.y)
				{
					PhysXCapsule->SetWorldPosition(SplitTargetPos);
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_Split_3);
					return;
				}

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
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Split_3");
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Idle);
				}
			},
			.End = [=] {
			}
			});
		// RedQueen Caliber_1
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_RQ_Skill_Caliber_1,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_RQ_Skill_Caliber_1");
			},
			.Update = [=](float _DeltaTime) {
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
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
			},
			.End = [=] {
				PhysXCapsule->TurnOnGravity();
			}
			});
	}
	{}
	// 블루 로즈 (락온)
	{
		// Idle To LockOn
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				LookTarget();
				Renderer->ChangeAnimation("pl0000_BR_Switch_Idle_to_Lockon");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Controller->GetLockOnFree())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Lockon_to_Idle);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_BR_Strafe);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
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
				if (true == Controller->GetLockOnFree())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Lockon_to_Idle);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_BR_Strafe);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
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
				if (true == Controller->GetLockOnFree())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Switch_Lockon_to_Idle);
					return;
				}
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_BR_Lockon_Front);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
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
				if (true == Renderer->IsAnimationEnd())
				{
					ChangeState(FSM_State_Nero::Nero_Idle);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}


				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
			},
			.End = [=] {

			}
			});
		// Evade
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Evade_Left,
			.Start = [=] {
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

				if (false == InputCheck) { return; }
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					FSMForce = true;
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}

				if (false == MoveCheck) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				FSMForce = false;

			}
			});
		// Evade
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Evade_Right,
			.Start = [=] {
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

				if (false == InputCheck) { return; }
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					FSMForce = true;
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}

				if (false == MoveCheck) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				FSMForce = false;
			}
			});
		// Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Shoot,
			.Start = [=] {
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

				if (false == InputCheck) { return; }
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					FSMForce = true;
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}

				if (false == MoveCheck) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {
				FSMForce = false;
			}
			});
			// Air Shoot
			FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_AirShoot,
				.Start = [=] {
					BlueRoseOn();
					PhysXCapsule->SetLinearVelocityZero();
					PhysXCapsule->TurnOffGravity();
					InputCheck = false;
					MoveCheck = false;
					Renderer->ChangeAnimation("pl0000_BR_Air_Shoot", true);
				},
				.Update = [=](float _DeltaTime) {
					if (true == FloorCheck())
					{
						ChangeState(FSM_State_Nero::Nero_Landing);
						return;
					}
					if (true == Renderer->IsAnimationEnd())
					{
						ChangeState(FSM_State_Nero::Nero_Jump_Fly);
						return;
					}

					if (false == InputCheck) { return; }
					if (Controller->GetIsLeftJump())
					{
						//ChangeState(FSM_State_Nero::Nero_Evade_Left);
						return;
					}
					if (Controller->GetIsRightJump())
					{
						//ChangeState(FSM_State_Nero::Nero_Evade_Right);
						return;
					}
					if (Controller->GetIsJump())
					{
						//ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
						return;
					}
					if (Controller->GetGunUp())
					{
						ChangeState(FSM_State_Nero::Nero_BR_AirShoot);
						FSMForce = true;
						return;
					}
					if (Controller->GetIsSword())
					{
						ChangeState(FSM_State_Nero::Nero_RQ_AirComboA_1);
						return;
					}
				},
				.End = [=] {
					WeaponIdle();
					FSMForce = false;
				}
				});
	}
	// 오버추어
	{}
	{
		// Nero_Overture_Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Overture_Shoot,
			.Start = [=] {
				OffDevilBreaker();
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0000_Overture_Shoot");
				Renderer_Overture->On();
				Renderer_Overture->ChangeAnimation("wp00_010_Shoot.fbx", true);
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (InputCheck == false) { return; }
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_2);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

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
				OffDevilBreaker();
				PhysXCapsule->SetLinearVelocityZero();
				PhysXCapsule->TurnOffGravity();
				Renderer->ChangeAnimation("pl0000_Overture_Air_Shoot");
				Renderer_Overture->On();
				Renderer_Overture->ChangeAnimation("wp00_010_Air_Shoot.fbx", true);
				RotationToTarget(30.0f);
				InputCheck = false;
				MoveCheck = false;
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
				}
				if (InputCheck == false) { return; }
				if (Controller->GetGunUp())
				{
					ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					ChangeState(FSM_State_Nero::Nero_RQ_ComboA_2);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

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
	}
	ChangeState(FSM_State_Nero::Nero_Idle);
}

void PlayerActor_Nero::NetLoad()
{
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
		}
		NewDir.MoveParent();
		NewDir.Move("Animation");

		Renderer = CreateComponent<GameEngineFBXRenderer>();
		Renderer->GetTransform()->SetLocalRotation({ 0, 0, 0 });
		Renderer->GetTransform()->SetLocalPosition({ 0, -75, 0 });
		Renderer->SetFBXMesh("Nero.FBX", "MeshAniTexture");
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


		//Object = 0 : 공격 충돌체
		//
		//콜백void = 0 : nullptr
		//콜백void = 1 : 손에 레드퀸 들려줌
		//콜백void = 2 : 손에 레드퀸 뺌
		//콜백void = 3 : 손에 블루로즈
		//
		//콜백 int = 0 : FSM변경
		// 

		SetDemon();
		WeaponIdle();
	}


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
			ChangeState(Nero_Idle);
		}
	}
}

void PlayerActor_Nero::ChangeState(FSM_State_Nero _StateValue)
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
}

void PlayerActor_Nero::SetOverture()
{
	Renderer->GetAllRenderUnit()[0][13]->On();
	Renderer->GetAllRenderUnit()[0][12]->Off();
	Renderer->GetAllRenderUnit()[0][11]->Off();
	Renderer->GetAllRenderUnit()[0][10]->Off();
	Renderer->GetAllRenderUnit()[0][9]->Off();
	Renderer_Overture->Off();
}

void PlayerActor_Nero::OffDevilBreaker()
{
	Renderer->GetAllRenderUnit()[0][13]->Off();
	Renderer->GetAllRenderUnit()[0][12]->Off();
	Renderer->GetAllRenderUnit()[0][11]->Off();
	Renderer->GetAllRenderUnit()[0][10]->Off();
	Renderer->GetAllRenderUnit()[0][9]->Off();
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