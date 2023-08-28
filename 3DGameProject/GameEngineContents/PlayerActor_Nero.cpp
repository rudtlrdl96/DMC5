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
		Renderer->GetTransform()->SetLocalRotation({ 0, 90, 0 });
		Renderer->GetTransform()->SetLocalPosition({ 0, -75, 0 });
		Renderer->SetFBXMesh("Nero.FBX", "MeshAniTexture");
		AnimationEvent::LoadAll({ .Dir = NewDir.GetFullPath().c_str(), .Renderer = Renderer,
			.Objects = { (GameEngineObject*)AttackCollision.get() },
			.CallBacks_void = {
				std::bind([=] {InputCheck = true; }),
				std::bind(&PlayerActor_Nero::RedQueenOn, this),
				std::bind(&PlayerActor_Nero::RedQueenOff, this),
				std::bind(&PlayerActor_Nero::BlueRoseOn, this),
				std::bind(&PlayerActor_Nero::WeaponIdle, this)
			},
			.CallBacks_int = { 
				std::bind(&GameEngineFSM::ChangeState, &FSM, std::placeholders::_1)
			}

			});

		/*
		* Object = 0 : 공격 충돌체
		* 콜백void = 0 : 입력체크시작
		* 콜백void = 1 : 손에 레드퀸 들려줌
		* 콜백void = 2 : 손에 레드퀸 뺌
		* 콜백void = 3 : 손에 블루로즈
		* 콜백void = 4 : WeaponIdle
		* 콜백 int = 0 : FSM변경
		* 
		*/

		Renderer->GetAllRenderUnit()[0][12]->Off();	// 버스터 암
		Renderer->GetAllRenderUnit()[0][13]->Off();	// 버스터 암
		Renderer->GetAllRenderUnit()[0][14]->On();	// 오버추어
		Renderer->GetAllRenderUnit()[0][15]->Off();	// 거베라
		Renderer->GetAllRenderUnit()[0][16]->Off();	// 거베라
		Renderer->GetAllRenderUnit()[0][17]->Off();	// 손 레드퀸
		Renderer->GetAllRenderUnit()[0][18]->On();	// 등 레드퀸
		Renderer->GetAllRenderUnit()[0][19]->Off();	// 블루로즈
	}

	/* 기본 움직임 */
	{
		FSM.CreateState({ .StateValue = FSM_State_Nero::Idle,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Idle_Normal");
			},
			.Update = [=](float _DeltaTime) {

				if (Controller->GetSwordDown())
				{
					FSM.ChangeState(FSM_State_Nero::RQ_ComboA_1);
					return;
				}

				if (Controller->GetMoveVector() != float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Walk);
					return;
				}

				if (true == IsLockOn)
				{
					FSM.ChangeState(BR_Switch_Idle_to_Lockon);
					return;
				}
			},
			.End = [=] {

			}
			});

		FSM.CreateState({ .StateValue = FSM_State_Nero::Walk,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_Dash_Loop");
			},
			.Update = [=](float _DeltaTime) {

				if (Controller->GetSwordDown())
				{
					FSM.ChangeState(FSM_State_Nero::RQ_ComboA_1);
					return;
				}

				if (Controller->GetMoveVector() == float4::ZERO)
				{
					FSM.ChangeState(FSM_State_Nero::Idle);
					return;
				}

				if (true == IsLockOn)
				{
					FSM.ChangeState(BR_Switch_Idle_to_Lockon);
					return;
				}

				LookDir(Controller->GetMoveVector());
				physx::PxTransform PhyTF = PhysXCapsule->GetDynamic()->getGlobalPose();
				float4 MoveDir = Controller->GetMoveVector() * MoveSpeed * _DeltaTime;
				PhyTF.p.x += MoveDir.x;
				PhyTF.p.z += MoveDir.z;
				PhysXCapsule->GetDynamic()->setGlobalPose(PhyTF);
			},
			.End = [=] {

			}
			});
	}
	/* 레드 퀸 */
	{
		// RedQueen Combo
		FSM.CreateState({ .StateValue = FSM_State_Nero::RQ_ComboA_1,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_RQ_ComboA_1");
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (InputCheck == false) { return; }
				if (Controller->GetSwordDown())
				{
					FSM.ChangeState(FSM_State_Nero::RQ_ComboA_2);
					return;
				}
			},
			.End = [=] {

			}
			});

		// RedQueen Combo
		FSM.CreateState({ .StateValue = FSM_State_Nero::RQ_ComboA_2,
			.Start = [=] {
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboA_2");
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (InputCheck == false) { return; }
				if (Controller->GetSwordDown())
				{
					FSM.ChangeState(FSM_State_Nero::RQ_ComboA_3);
					return;
				}
			},
			.End = [=] {

			}
			});

		FSM.CreateState({ .StateValue = FSM_State_Nero::RQ_ComboA_3,
			.Start = [=] {
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboA_3");
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (InputCheck == false) { return; }
				if (Controller->GetSwordDown())
				{
					FSM.ChangeState(FSM_State_Nero::RQ_ComboA_4);
					return;
				}
			},
			.End = [=] {

			}
			});

		FSM.CreateState({ .StateValue = FSM_State_Nero::RQ_ComboA_4,
			.Start = [=] {
				RedQueenOn();
				Renderer->ChangeAnimation("pl0000_RQ_ComboA_4");
				InputCheck = false;
			},
			.Update = [=](float _DeltaTime) {
				if (InputCheck == false) { return; }
			},
			.End = [=] {

			}
			});

	}
	/* 블루 로즈 (락온) */
	{
		FSM.CreateState({ .StateValue = FSM_State_Nero::BR_Switch_Idle_to_Lockon,
			.Start = [=] {
				WeaponIdle();
				Renderer->ChangeAnimation("pl0000_BR_Switch_Idle_to_Lockon");
			},
			.Update = [=](float _DeltaTime) {


				if (Controller->GetMoveVector() != float4::ZERO)
				{
					//FSM.ChangeState(FSM_State_Nero::Walk);
					return;
				}

				if (true == Renderer->IsAnimationEnd())
				{
					FSM.ChangeState(FSM_State_Nero::BR_Lockon_Front);
					return;
				}
			},
			.End = [=] {

			}
			});

		FSM.CreateState({ .StateValue = FSM_State_Nero::BR_Lockon_Front,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_BR_Lockon_Front");
			},
			.Update = [=](float _DeltaTime) {
				if (false == IsLockOn)
				{
					FSM.ChangeState(FSM_State_Nero::BR_Switch_Lockon_to_Idle);
				}
			},
			.End = [=] {

			}
			});

		FSM.CreateState({ .StateValue = FSM_State_Nero::BR_Switch_Lockon_to_Idle,
			.Start = [=] {
				Renderer->ChangeAnimation("pl0000_BR_Switch_Lockon_to_Idle");
			},
			.Update = [=](float _DeltaTime) {
				if (true == Renderer->IsAnimationEnd())
				{
					FSM.ChangeState(FSM_State_Nero::Idle);
					return;
				}
			},
			.End = [=] {

			}
			});
	}
	FSM.ChangeState(FSM_State_Nero::Idle);
}

void PlayerActor_Nero::Update_Character(float _DeltaTime)
{
	FSM.Update(_DeltaTime);
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
	Renderer->GetAllRenderUnit()[0][17]->On();	// 손 레드퀸
	Renderer->GetAllRenderUnit()[0][18]->Off();	// 등 레드퀸
	Renderer->GetAllRenderUnit()[0][19]->Off();	// 블루로즈
}

void PlayerActor_Nero::RedQueenOff()
{
	Renderer->GetAllRenderUnit()[0][17]->Off();	// 손 레드퀸
	Renderer->GetAllRenderUnit()[0][18]->On();	// 등 레드퀸
}

void PlayerActor_Nero::BlueRoseOn()
{
	Renderer->GetAllRenderUnit()[0][17]->Off();	// 손 레드퀸
	Renderer->GetAllRenderUnit()[0][18]->On();	// 등 레드퀸
	Renderer->GetAllRenderUnit()[0][19]->On();	// 블루로즈
}

void PlayerActor_Nero::WeaponIdle()
{
	Renderer->GetAllRenderUnit()[0][17]->Off();	// 손 레드퀸
	Renderer->GetAllRenderUnit()[0][18]->On();	// 등 레드퀸
	Renderer->GetAllRenderUnit()[0][19]->Off();	// 블루로즈
}

		//LookDir((LockOnEnemyTransform->GetWorldPosition() - GetTransform()->GetWorldPosition()).NormalizeReturn());
