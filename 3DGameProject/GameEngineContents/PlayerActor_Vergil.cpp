#include "PrecompileHeader.h"
#include "PlayerActor_Vergil.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include "AnimationEvent.h"
#include "PlayerController.h"

#include "PlayerWindow.h"
PlayerActor_Vergil::PlayerActor_Vergil() 
{
}

PlayerActor_Vergil::~PlayerActor_Vergil() 
{
}

void PlayerActor_Vergil::Start()
{
	BasePlayerActor::Start();
	VergilLoad();
}

void PlayerActor_Vergil::VergilLoad()
{
	// Renderer ����
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
			.Objects = { (GameEngineObject*)AttackCollision.get() },
			.CallBacks_void = {
				std::bind([=] {InputCheck = true; }),
				std::bind(&PhysXCapsuleComponent::SetLinearVelocityZero, PhysXCapsule)
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

		PlayerWindow::Renderer = Renderer.get();
	}

	/* �⺻ ������ */
	{
		// Idle
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Idle,
			.Start = [=] {
				PhysXCapsule->SetLinearVelocityZero();
				Renderer->ChangeAnimation("pl0300_Idle_Normal");
			},
			.Update = [=](float _DeltaTime) {

				if (Controller->GetSwordDown())
				{
					//FSM.ChangeState(FSM_State_Vergil::Vergil_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					//FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}

				if (true == IsLockOn)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_IdleLockOn);
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

				if (Controller->GetSwordDown())
				{
					//FSM.ChangeState(FSM_State_Vergil::Vergil_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					//FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
					return;
				}
				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Walk);
					return;
				}

				if (false == IsLockOn)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Idle);
					return;
				}
			},
			.End = [=] {

			}
			});

		// Walk
		FSM.CreateState({ .StateValue = FSM_State_Vergil::Vergil_Walk,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0300_Walk_Front");
			},
			.Update = [=](float _DeltaTime) {
				if (Controller->GetSwordDown())
				{
					//FSM.ChangeState(FSM_State_Vergil::Vergil_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					//FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
					return;
				}

				if (Controller->GetMoveVector() == float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_IdleLockOn);
					return;
				}
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
				if (Controller->GetSwordDown())
				{
					//FSM.ChangeState(FSM_State_Vergil::Vergil_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					//FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
					return;
				}
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_RunStop);
					return;
				}
				if (true == IsLockOn)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Walk);
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
				if (Controller->GetSwordDown())
				{
					//FSM.ChangeState(FSM_State_Vergil::Vergil_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					//FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
					return;
				}
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_RunStop);
					return;
				}

				if (true == IsLockOn)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_Walk);
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
				InputCheck = false;
				Renderer->ChangeAnimation("pl0300_Run_Stop");
			},
			.Update = [=](float _DeltaTime) {
				if (Controller->GetSwordDown())
				{
					//FSM.ChangeState(FSM_State_Vergil::Vergil_RQ_ComboA_1);
					return;
				}
				if (Controller->GetIsJump())
				{
					//FSM.ChangeState(FSM_State_Vergil::Vergil_Jump_Vertical);
					return;
				}

				if (false == InputCheck) { return; }

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

void PlayerActor_Vergil::Update_Character(float _DeltaTime)
{
	FSM.Update(_DeltaTime);
}


/*
���� ���� ����
0 ��
1 ��
2 ��
3 �Ӹ�ī��
4 ��
5 ��
6 ��
7 �尩
8 ��
9 �ȸ�
10 ~ 17 ��
18 ������  �߸���
19 ������ �߸��� 2
20 ��
21 ����
22 ���� ��

23 �޼� �߸��� ����
24 �޼� �߸��� ������ �Ǽ��縮
25 �޼� �߸��� ��
26 �޼� �߸��� ������
*/