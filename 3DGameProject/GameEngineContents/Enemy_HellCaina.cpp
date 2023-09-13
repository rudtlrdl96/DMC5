#include "PrecompileHeader.h"
#include "Enemy_HellCaina.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>

#include "NetworkManager.h"
#include "AnimationEvent.h"

Enemy_HellCaina::Enemy_HellCaina() 
{
}

Enemy_HellCaina::~Enemy_HellCaina() 
{
}

void Enemy_HellCaina::Start()
{
	EnemyAnimationLoad();
	EnemyCreateFSM();
}
void Enemy_HellCaina::Update(float _DeltaTime)
{

}

void Enemy_HellCaina::EnemyMeshLoad()
{
	std::string Path = GameEnginePath::GetFileFullPath
	(
		"ContentResources",
		{
			"Character", "Enemy", "HellCaina", "mesh"
		}, 
		"em0000.FBX"
	);
	GameEngineFBXMesh::Load(Path);

	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		EnemyRenderer->SetFBXMesh("em0000.fbx", "AniFBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		EnemyRenderer->SetFBXMesh("em0000.fbx", "AniFBX");
	}
	break;
	default:
		break;
	}

	EnemyRenderer->GetTransform()->SetLocalScale({ 1.0f , 1.0f , 1.0f });
}

void Enemy_HellCaina::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::HellCaina;
	EnemyTypeValue = EnemyType::Normal;
	EnemySizeValue = EnemySize::Middle;

	EnemyHP = 0;
	RN_Range = float4::ZERO;
	RN_Player = false;
	MoveSpeed = 50.0f;
}

void Enemy_HellCaina::EnemyAnimationLoad()
{
	//Animation정보 경로를 찾아서 모든animation파일 로드
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("HellCaina");
	NewDir.Move("Animation");

	AnimationEvent::LoadAll(
		{
			.Dir = NewDir.GetFullPath().c_str(), 
			.Renderer = EnemyRenderer,
			.Objects = {(GameEngineObject*)MonsterCollision.get()},
			.CallBacks_void =
			{
				std::bind([=] {CheckBool = true; }),
			},
			.CallBacks_int =
			{
				std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1)
			}
		}
	);

	NewDir.MoveParent();
	NewDir.Move("Animation");
	std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
	for (GameEngineFile File : Files)
	{
		if (nullptr == GameEngineFBXAnimation::Find(File.GetFileName()))
		{
			GameEngineFBXAnimation::Load(File.GetFullPath());
		}
	}
}

void Enemy_HellCaina::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
}

void Enemy_HellCaina::EnemyCreateFSM()
{
	{
		// Idle
		EnemyFSM.CreateState
		(
			{
				.StateValue = FSM_State_HellCaina::HellCaina_Idle,
				.Start = [=]
				{
					//PhysXCapsule->TurnOffGravity();
					//WeaponIdle();
					//PhysXCapsule->SetLinearVelocityZero();
					//EnemyRenderer->ChangeAnimation("pl0000_Idle_Normal");
				},
				.Update = [=](float _DeltaTime)
				{
					//if (Controller->GetMoveVector() != float4::ZERO)
					//{
					//	ChangeState(FSM_State_HellCaina::HellCaina_Idle);
					//	return;
					//}
					//if (true == Controller->GetIsLockOn())
					//{
					//	ChangeState(FSM_State_HellCaina::HellCaina_Idle);
					//	return;
					//}
				},
				.End = [=]
				{

				}
			}
		);
	}
}
