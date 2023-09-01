#include "PrecompileHeader.h"
#include "PlayerActor_Nero.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include "AnimationEvent.h"
#include "PlayerController.h"
PlayerActor_Nero::PlayerActor_Nero()
{
}

PlayerActor_Nero::~PlayerActor_Nero()
{
}

void PlayerActor_Nero::Start()
{
	BasePlayerActor::Start();

	if (false == GameEngineInput::IsKey("Escape"))
	{
		GameEngineInput::CreateKey("Escape", VK_F10);
	}

#ifdef _DEBUG
	TestLoad();
#else
	NeroLoad();
#endif

}

void PlayerActor_Nero::TestLoad()
{
	Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("House1.fbx", "FBX");

	{
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Idle,
			.Start = [=] {
			},
			.Update = [=](float _DeltaTime) {
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Run);
					return;
				}
				if (true == IsLockOn)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
					return;
				}
			},
			.End = [=] {

			}
			});

		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Run,
			.Start = [=] {
			},
			.Update = [=](float _DeltaTime) {
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Idle);
					return;
				}

				if (true == IsLockOn)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
					return;
				}

				LookDir(Controller->GetMoveVector());
				float4 MoveDir = Controller->GetMoveVector() * RunSpeed;
				PhysXCapsule->SetMove(MoveDir);
			},
			.End = [=] {

			}
			});

		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon,
			.Start = [=] {
			},
			.Update = [=](float _DeltaTime) {
				if (false == IsLockOn)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Idle);
					return;
				}
				LookTarget();
				float4 MoveDir = Controller->GetMoveVector() * RunSpeed;
				PhysXCapsule->SetMove(MoveDir);
			},
			.End = [=] {

			}
			});
	}

	FSM.ChangeState(FSM_State_Nero::Nero_Idle);
}

void PlayerActor_Nero::NeroLoad()
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
		Renderer->SetFBXMesh("Nero.FBX", "MeshAniTexture");
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


		//Object = 0 : ���� �浹ü
		//
		//�ݹ�void = 0 : �Է�üũ����
		//�ݹ�void = 1 : �տ� ������ �����
		//�ݹ�void = 2 : �տ� ������ ��
		//�ݹ�void = 3 : �տ� ������
		//�ݹ�void = 4 : WeaponIdle (���, Į �)
		//�ݹ�void = 5 : SetLinearVelocityZero
		//�ݹ�void = 6 : �̵�üũ ����
		//
		//�ݹ� int = 0 : FSM����
		// 
		//�ݹ� float4 = 0 : SetForce
		//�ݹ� float4 = 0 : SetPush
		//�ݹ� float4 = 0 : SetMove



		Renderer->GetAllRenderUnit()[0][12]->Off();	// ������ ��
		Renderer->GetAllRenderUnit()[0][13]->Off();	// ������ ��
		Renderer->GetAllRenderUnit()[0][14]->On();	// �����߾�
		Renderer->GetAllRenderUnit()[0][15]->Off();	// �ź���
		Renderer->GetAllRenderUnit()[0][16]->Off();	// �ź���
		Renderer->GetAllRenderUnit()[0][17]->Off();	// �� ������
		Renderer->GetAllRenderUnit()[0][18]->On();	// �� ������
		Renderer->GetAllRenderUnit()[0][19]->Off();	// ������
	}
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
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}

				if (true == IsLockOn)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
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
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RunStop);
					return;
				}
				if (true == IsLockOn)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
					return;
				}
				if (true == Renderer->IsAnimationEnd())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Run);
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
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RunStop);
					return;
				}

				if (true == IsLockOn)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
					return;
				}

				if (1 < DashTimer)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Dash);
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
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
				if (true == IsLockOn)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
					return;
				}
			},
			.End = [=] {

			}
			});

		// Dash
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Dash,
			.Start = [=] {
				DashTimer = 0;
				Renderer->ChangeAnimation("pl0000_Dash_Loop");
			},
			.Update = [=](float _DeltaTime) {
				DashTimer += _DeltaTime;
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_DashStop);
					return;
				}
				if (true == IsLockOn)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
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
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
				if (true == IsLockOn)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Switch_Idle_to_Lockon);
					return;
				}
			},
			.End = [=] {

			}
			});

		// Jump_Vertical
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Jump_Vertical,
			.Start = [=] {
				InputCheck = false;
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Jump_Vertical");
			},
			.Update = [=](float _DeltaTime) {
				if (Renderer->IsAnimationEnd())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
				float4 MoveDir = Controller->GetMoveVector() * RunSpeed;
				PhysXCapsule->SetMove(MoveDir);

				if (false == InputCheck) { return; }
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_AirShoot);
					return;
				}
			},
			.End = [=] {
			}
			});

		static float Timer = 0;
		// Jump Fly
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_Jump_Fly,
			.Start = [=] {
				Timer = 0;
				Renderer->ChangeAnimation("pl0000_Jump_Fly_loop");
			},
			.Update = [=](float _DeltaTime) {
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_AirShoot);
					return;
				}

				Timer += _DeltaTime;
				if (0.5f < Timer)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Landing);
				}
				float4 MoveDir = Controller->GetMoveVector() * RunSpeed;
				PhysXCapsule->SetMove(MoveDir);

			},
			.End = [=] {

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
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
				if (Renderer->IsAnimationEnd())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Idle);
				}
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
				if (InputCheck == false) { return; }
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_2);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RunStart);
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
			},
			.Update = [=](float _DeltaTime) {
				if (InputCheck == false) { return; }
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_3);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RunStart);
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
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_4);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RunStart);
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
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}

				if (MoveCheck == false) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RunStart);
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
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Fly);
				}
				if (InputCheck == false) { return; }
				if (Controller->GetIsLeftJump())
				{
					//FSM.ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					//FSM.ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					//FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_AirShoot);
					return;
				}
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
				PhysXCapsule->SetLinearVelocityZero();
				LookTarget();
				Renderer->ChangeAnimation("pl0000_BR_Switch_Idle_to_Lockon");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Controller->GetLockOnFree())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Switch_Lockon_to_Idle);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Strafe);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}
				if (true == Renderer->IsAnimationEnd())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Lockon_Front);
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
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Switch_Lockon_to_Idle);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Strafe);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
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
				CurDir = 'n';
			},
			.Update = [=](float _DeltaTime) {
				if (true == Controller->GetLockOnFree())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Switch_Lockon_to_Idle);
					return;
				}
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Lockon_Front);
					return;
				}
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}

				LookTarget();

				float4 MoveDir = Controller->GetMoveVector() * WalkSpeed;
				PhysXCapsule->SetMove(MoveDir);

				char NewDir = Controller->MoveVectorToChar(Controller->GetMoveVector());
				if (CurDir == NewDir) { return; }
				CurDir = NewDir;
				switch (CurDir)
				{
				case '8':
					Renderer->ChangeAnimation("pl0000_Strafe_F_Loop");
					break;
				case '7':
					Renderer->ChangeAnimation("pl0000_Strafe_FL_Loop");
					break;
				case '4':
					Renderer->ChangeAnimation("pl0000_Strafe_L_Loop");
					break;
				case '1':
					Renderer->ChangeAnimation("pl0000_Strafe_BL_Loop");
					break;
				case '2':
					Renderer->ChangeAnimation("pl0000_Strafe_B_Loop");
					break;
				case '3':
					Renderer->ChangeAnimation("pl0000_Strafe_BR_Loop");
					break;
				case '6':
					Renderer->ChangeAnimation("pl0000_Strafe_R_Loop");
					break;
				case '9':
					Renderer->ChangeAnimation("pl0000_Strafe_R_Loop");
					break;
				}

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
					FSM.ChangeState(FSM_State_Nero::Nero_Idle);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}


				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
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
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Lockon_Front);
					return;
				}

				if (false == InputCheck) { return; }
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}

				if (false == MoveCheck) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {

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
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Lockon_Front);
					return;
				}

				if (false == InputCheck) { return; }
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}

				if (false == MoveCheck) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {

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
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Lockon_Front);
					return;
				}

				if (false == InputCheck) { return; }
				if (Controller->GetIsLeftJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_Shoot);
					return;
				}
				if (Controller->GetIsBackSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_Skill_HR);
					return;
				}
				if (Controller->GetIsSword())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RQ_ComboA_1);
					return;
				}

				if (false == MoveCheck) { return; }
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Nero_RunStart);
					return;
				}
			},
			.End = [=] {

			}
			});

		// Air Shoot
		FSM.CreateState({ .StateValue = FSM_State_Nero::Nero_BR_AirShoot,
			.Start = [=] {
				BlueRoseOn();
				InputCheck = false;
				MoveCheck = false;
				Renderer->ChangeAnimation("pl0000_BR_Air_Shoot", true);
			},
			.Update = [=](float _DeltaTime) {
				if (true == Renderer->IsAnimationEnd())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_Jump_Fly);
					return;
				}

				if (false == InputCheck) { return; }
				if (Controller->GetIsLeftJump())
				{
					//FSM.ChangeState(FSM_State_Nero::Nero_Evade_Left);
					return;
				}
				if (Controller->GetIsRightJump())
				{
					//FSM.ChangeState(FSM_State_Nero::Nero_Evade_Right);
					return;
				}
				if (Controller->GetIsJump())
				{
					//FSM.ChangeState(FSM_State_Nero::Nero_Jump_Vertical);
					return;
				}
				if (Controller->GetGunUp())
				{
					FSM.ChangeState(FSM_State_Nero::Nero_BR_AirShoot);
					return;
				}
			},
			.End = [=] {
				BlueRoseOff();
			}
			});
	}
	FSM.ChangeState(FSM_State_Nero::Nero_Idle);
}

void PlayerActor_Nero::Update_Character(float _DeltaTime)
{
	if (GameEngineInput::IsDown("Escape"))
	{
		SetWorldPosition({ 0, 100, 0 });
	}
	FSM.Update(_DeltaTime);
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
	Renderer->GetAllRenderUnit()[0][17]->On();	// �� ������
	Renderer->GetAllRenderUnit()[0][18]->Off();	// �� ������
	Renderer->GetAllRenderUnit()[0][19]->Off();	// ������
}

void PlayerActor_Nero::RedQueenOff()
{
	Renderer->GetAllRenderUnit()[0][17]->Off();	// �� ������
	Renderer->GetAllRenderUnit()[0][18]->On();	// �� ������
}

void PlayerActor_Nero::BlueRoseOn()
{
	Renderer->GetAllRenderUnit()[0][17]->Off();	// �� ������
	Renderer->GetAllRenderUnit()[0][18]->On();	// �� ������
	Renderer->GetAllRenderUnit()[0][19]->On();	// ������
}

void PlayerActor_Nero::WeaponIdle()
{
	Renderer->GetAllRenderUnit()[0][17]->Off();	// �� ������
	Renderer->GetAllRenderUnit()[0][18]->On();	// �� ������
	Renderer->GetAllRenderUnit()[0][19]->Off();	// ������
}

//LookDir((LockOnEnemyTransform->GetWorldPosition() - GetTransform()->GetWorldPosition()).NormalizeReturn());
