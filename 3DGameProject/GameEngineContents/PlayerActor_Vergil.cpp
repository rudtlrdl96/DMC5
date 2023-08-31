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
			.Objects = { (GameEngineObject*)AttackCollision.get() },
			.CallBacks_void = {
				std::bind([=] {InputCheck = true; }),
				std::bind(&PlayerActor_Vergil::YamatoOn, this),
				std::bind(&PlayerActor_Vergil::YamatoOff, this),
				std::bind(&PlayerActor_Vergil::SetHuman, this),
				std::bind(&PlayerActor_Vergil::SetMajin, this),
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

		SetHuman();
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
				if (Controller->GetIsSpecialMove())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_1);
					return;
				}
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
				CurDir = 'n';
			},
			.Update = [=](float _DeltaTime) {
				if (Controller->GetMoveVector() == float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_IdleLockOn);
					return;
				}
				if (true == Controller->GetLockOnFree())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_RunStart);
					return;
				}
				if (Controller->GetIsSpecialMove())
				{
					FSM.ChangeState(FSM_State_Vergil::Vergil_yamato_JudgementCutEnd_1);
					return;
				}

				if (nullptr != LockOnEnemyTransform)
				{
					LookTarget(LockOnEnemyTransform->GetWorldPosition());
				}
				float4 MoveDir = Controller->GetMoveVector() * WalkSpeed;
				PhysXCapsule->SetMove(MoveDir);

				char NewDir = Controller->MoveVectorToChar4(Controller->GetMoveVector());
				if (CurDir == NewDir) { return; }
				CurDir = NewDir;

				switch (CurDir)
				{
				case '8':
					Renderer->ChangeAnimation("pl0300_Walk_Front");
					break;
				case '4':
					Renderer->ChangeAnimation("pl0300_Walk_Left");
					break;
				case '2':
					Renderer->ChangeAnimation("pl0300_Walk_Back");
					break;
				case '6':
					Renderer->ChangeAnimation("pl0300_Walk_Right");
					break;
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

	/* 야마토 */
	{
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

void PlayerActor_Vergil::SetHuman()
{
	for (int i = 0; i <= 17; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->On();
	}
	for (int i = 20; i <= 22; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->Off();
	}
}

void PlayerActor_Vergil::SetMajin()
{
	for (int i = 0; i <= 17; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->Off();
	}
	for (int i = 20; i <= 22; i++)
	{
		Renderer->GetAllRenderUnit()[0][i]->On();
	}
}

void PlayerActor_Vergil::YamatoOff()
{
	Renderer->GetAllRenderUnit()[0][18]->Off();
	Renderer->GetAllRenderUnit()[0][19]->Off();
	Renderer->GetAllRenderUnit()[0][24]->On();
	Renderer->GetAllRenderUnit()[0][25]->On();
	Renderer->GetAllRenderUnit()[0][26]->On();
}

void PlayerActor_Vergil::YamatoOn()
{
	Renderer->GetAllRenderUnit()[0][18]->On();
	Renderer->GetAllRenderUnit()[0][19]->On();

	Renderer->GetAllRenderUnit()[0][24]->Off();
	Renderer->GetAllRenderUnit()[0][25]->Off();
	Renderer->GetAllRenderUnit()[0][26]->Off();
}


/*
버질 렌더 유닛
0 옷
1 옷
2 옷
3 머리카락
4 옷
5 옷
6 팔
7 장갑
8 옷
9 안면
10 ~ 17 얼굴
18 오른손  야마토
19 오른손 야마토 2
20 뿔
21 마인
22 마인 뿔

23 왼손 야마토 검집
24 왼손 야마토 손잡이 악세사리
25 왼손 야마토 검
26 왼손 야마토 손잡이
*/