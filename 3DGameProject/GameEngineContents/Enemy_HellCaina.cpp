#include "PrecompileHeader.h"
#include "Enemy_HellCaina.h"
#include <GameEngineBase/GameEngineDirectory.h>
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
	//Render생성
	EnemyRenderer = CreateComponent<GameEngineFBXRenderer>();
	EnemyRenderer->GetTransform()->AddLocalPosition({0.0f, -50.0f, 0.0f});
	//PhysX(충돌)
	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0, 0, 0);
	PhysXCapsule->CreatePhysXActors({ 90, 60, 90 });
	PhysXCapsule->SetWorldPosition({ 0, 100, 0 });

	EnemyMeshLoad();
	EnemyTypeLoad();
	EnemyAnimationLoad();
	EnemyCreateFSM();
}
void Enemy_HellCaina::Update(float _DeltaTime)
{
	//if (LoadCheck == false) { return; }
	EnemyFSM.Update(_DeltaTime);

	//if (NetControllType::UserControll == GameEngineNetObject::GetControllType())
	//{
	//	if (GameEngineInput::IsDown("Escape"))
	//	{
	//		SetWorldPosition({ 0, 100, 0 });
	//		PhysXCapsule->SetWorldRotation({ 0, 0, 0 });
	//		ChangeState(Nero_Idle);
	//	}
	//	if (GameEngineInput::IsDown("SelectLevel_01"))
	//	{
	//		ChangeState(FSM_State_Nero::Nero_Damage_Fly);
	//		AddBreaker(DevilBreaker::Overture);
	//	}
	//	if (GameEngineInput::IsDown("SelectLevel_02"))
	//	{
	//		AddBreaker(DevilBreaker::Gerbera);
	//	}
	//	if (GameEngineInput::IsDown("SelectLevel_03"))
	//	{
	//		AddBreaker(DevilBreaker::BusterArm);
	//	}
	//}
}

void Enemy_HellCaina::EnemyMeshLoad()
{
	if (nullptr == GameEngineFBXMesh::Find("em0000.FBX"))
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
	}

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

	EnemyRenderer->GetTransform()->SetLocalScale({ 0.8f , 0.8f , 0.8f });
}

void Enemy_HellCaina::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::HellCaina;
	EnemyTypeValue = EnemyType::Normal;
	EnemySizeValue = EnemySize::Small;

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

	AnimationEvent::LoadAll
	(
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
			},
			.CallBacks_float4 = 
			{

			}
		}
	);

	//NewDir.MoveParent();
	//NewDir.Move("Animation");

	//std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".FBX" });

	//for (size_t i = 0; i < Files.size(); i++)
	//{
	//	std::string View = Files[i].GetFullPath().c_str();
	//	GameEngineFBXAnimation::Load(Files[i].GetFullPath());
	//	EnemyRenderer->CreateFBXAnimation(Files[i].GetFileName(), {.Inter = 0.0166f, .Loop = false});
	//}
	//
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
					PhysXCapsule->SetLinearVelocityZero();
					EnemyRenderer->ChangeAnimation("em0000_Idle_01");
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

	EnemyFSM.ChangeState(FSM_State_HellCaina::HellCaina_Idle);
}
