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

	// Renderer »ý¼º
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
		//AnimationEvent::LoadAll({ .Dir = NewDir.GetFullPath(), .Renderer = Renderer });
	}

	FSM.CreateState({ .StateValue = FSM_State_Nero::Idle,
		.Start = [=] {
			//Renderer->ChangeAnimation("pl0000_Idle_One");
		},
		.Update = [=](float _DeltaTime) {
			if (Controller->GetMoveVector() != float4::ZERO)
			{
				FSM.ChangeState(FSM_State_Nero::Walk);
				return;
			}
		},
		.End = [=] {

		}
	});

	FSM.CreateState({ .StateValue = FSM_State_Nero::Walk,
		.Start = [=] {
			//Renderer->ChangeAnimation("pl0000_Run_Loop2");
		},
		.Update = [=](float _DeltaTime) {
			if (Controller->GetMoveVector() == float4::ZERO)
			{
				FSM.ChangeState(FSM_State_Nero::Idle);
				return;
			}
			
			//LookDir(Controller->GetMoveVector());
			physx::PxTransform PhyTF = PhysXCapsule->GetDynamic()->getGlobalPose();
			float4 MoveDir = Controller->GetMoveVector() * MoveSpeed * _DeltaTime;
			PhyTF.p.x += MoveDir.x;
			PhyTF.p.z += MoveDir.z;
			PhysXCapsule->GetDynamic()->setGlobalPose(PhyTF);
		},
		.End = [=] {

		}
	});

	FSM.ChangeState(FSM_State_Nero::Idle);
}

void PlayerActor_Nero::Update_Character(float _DeltaTime)
{
	FSM.Update(_DeltaTime);
}

		//LookDir((LockOnEnemyTransform->GetWorldPosition() - GetTransform()->GetWorldPosition()).NormalizeReturn());
