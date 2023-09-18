#include "PrecompileHeader.h"
#include "Enemy_Empusa.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "BaseEnemyActor.h"

#include "AnimationEvent.h"
#include "BasePlayerActor.h"
#include "AttackCollision.h"
#include "NetworkManager.h"

Enemy_Empusa::Enemy_Empusa() 
{
}

Enemy_Empusa::~Enemy_Empusa() 
{
}

void Enemy_Empusa::DamageCollisionCheck()
{

}

void Enemy_Empusa::Start()
{
	BaseEnemyActor::Start();
	SetNetObjectType(Net_ActorType::Empusa);
	ChangeState(FSM_State_Empusa::Empusa_Idle);
}

void Enemy_Empusa::Update(float _DeltaTime)
{
	BaseEnemyActor::Update(_DeltaTime);
	EnemyFSM.Update(_DeltaTime);
}

void Enemy_Empusa::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
}

void Enemy_Empusa::PlayerChase(float _DeltaTime)
{
	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
		ChangeState(FSM_State_Empusa::Empusa_Walk_Start);
		break;
	case EnemyRotation::Left:
		ChangeState(FSM_State_Empusa::Empusa_Turn_Left);
		break;
	case EnemyRotation::Left_180:
		ChangeState(FSM_State_Empusa::Empusa_Turn_Left_180);
		break;
	case EnemyRotation::Right:
		ChangeState(FSM_State_Empusa::Empusa_Turn_Right);
		break;
	case EnemyRotation::Right_180:
		ChangeState(FSM_State_Empusa::Empusa_Turn_Right_180);
		break;
	default:
		break;
	}
}

void Enemy_Empusa::EnemyMeshLoad()
{
	//FBX파일경로를 찾아서 로드
	if (nullptr == GameEngineFBXMesh::Find("em0100.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Character", "Enemy", "em0100", "mesh"
			}, "em0100.FBX");

		GameEngineFBXMesh::Load(Path);
	}
	
	//EnemyBase에서 Start에서 생성된 Component에 Mesh Set
	EnemyRenderer->SetFBXMesh("em0100.fbx", "AniFBX_Low");
	EnemyRenderer->GetTransform()->SetLocalScale({ 1.0f , 1.0f , 1.0f });
}

void Enemy_Empusa::EnemyTypeLoad()
{
	//Type설정
	EnemyCodeValue = EnemyCode::Empusa;
	EnemyTypeValue = EnemyType::Normal;
	EnemySizeValue = EnemySize::Middle;
	EnemyHP = 100.0f;
	RN_Range = float4::ZERO;;
	RN_Player = false;
	MoveSpeed = 50.0f;
}

void Enemy_Empusa::EnemyAnimationLoad()
{
	//Animation정보 경로를 찾아서 모든animation파일 로드
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("em0100");
	NewDir.Move("Animation");
	AnimationEvent::LoadAll(
		{
			.Dir = NewDir.GetFullPath().c_str(), .Renderer = EnemyRenderer,
			.Objects = {(GameEngineObject*)MonsterCollision.get()},
			.CallBacks_void = 
			{
				std::bind([=] {CheckBool = true; }),
			},
			.CallBacks_int = {
				std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1)
			},
			.CallBacks_float4 = {
			
			}
		}
	);
}

void Enemy_Empusa::EnemyCreateFSM()
{
	MonsterCollision->GetTransform()->SetWorldScale({300,100,300});
	MonsterCollision->SetColType(ColType::OBBBOX3D);
	RN_MonsterCollision->GetTransform()->SetWorldScale({800,800,800});
	RN_MonsterCollision->SetColType(ColType::OBBBOX2D);
	MonsterAttackRange->GetTransform()->SetWorldScale({300,300,300});
	MonsterAttackRange->SetColType(ColType::SPHERE3D);

	//Idle
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Idle,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0100_Idle_undetected");
	},
	.Update = [=](float _DeltaTime) {
		CheckHeadingRotationValue();
		PlayerChase(_DeltaTime);
	},
	.End = [=] {}
	});

	/////////////////아기 엠푸사 걷는다
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Walk_Start,
	.Start = [=] {
	PhysXCapsule->AddWorldRotation({ 0, RotationValue, 0 });
	MoveDir = GetTransform()->GetWorldForwardVector().NormalizeReturn();
	MoveDir.RotationYDeg(RotationValue);
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_start");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Walk_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Walk_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_loop");
	},
	.Update = [=](float _DeltaTime) {
	PhysXCapsule->SetMove(MoveDir * 150.0f);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Walk_Stop);
	}
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Walk_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_stop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////Turn/////////////////////////////////////////////////////////////////////

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Left,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0100_turn_90_left");
	},
	.Update = [=](float _DeltaTime) {
	SlerpTurn(_DeltaTime);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
		return;
	}
	},
	.End = [=] {
		SlerpTime = 0.0f;
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Left_180,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0100_turn_180_left");
	},
	.Update = [=](float _DeltaTime) {
	SlerpTurn(_DeltaTime);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
		return;
	}
	},
	.End = [=] {
		SlerpTime = 0.0f;
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Right,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0100_turn_90_right");
	},
	.Update = [=](float _DeltaTime) {
	SlerpTurn(_DeltaTime);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
		return;
	}
	},
	.End = [=] {
		SlerpTime = 0.0f;
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Right_180,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0100_turn_180_right");
	},
	.Update = [=](float _DeltaTime) {
	SlerpTurn(_DeltaTime);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
		return;
	}
	},
	.End = [=] {
		SlerpTime = 0.0f;
	}
		});
}


