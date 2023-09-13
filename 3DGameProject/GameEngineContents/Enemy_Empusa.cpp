#include "PrecompileHeader.h"
#include "Enemy_Empusa.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "BaseEnemyActor.h"

#include "AnimationEvent.h"
#include "EnemyActor_Normal.h"
#include "BasePlayerActor.h"

Enemy_Empusa::Enemy_Empusa() 
{
}

Enemy_Empusa::~Enemy_Empusa() 
{
}

void Enemy_Empusa::Start()
{
	SetNetObjectType(Net_ActorType::Empusa);
}

void Enemy_Empusa::Update(float _DeltaTime)
{

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
	EnemyHP = 0;
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
				//Functional사용할 함수들 적는곳
			},
			.CallBacks_int = {
				std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1)
			}, //ChangeState,인자있음
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
}

void Enemy_Empusa::Idle_Enter()
{
	RN_Idle();
}

void Enemy_Empusa::Idle_Update(float _DeltaTime)
{
	if (true==RN_Player)
	{
		Move(_DeltaTime);
	}
}

void Enemy_Empusa::Idle_Exit()
{

}

void Enemy_Empusa::Chase_Enter()
{
}

void Enemy_Empusa::Chase_Update(float _DeltaTime)
{
}

void Enemy_Empusa::Chase_Exit()
{
}

void Enemy_Empusa::Attack_Enter()
{
	//GetPlayer Pos(위치 받아서 어떤 공격을할지 설정)
	//Debug(공격=플레이어를 인식했다고 가정)
	EnemyRenderer->ChangeAnimation("em0100_attack_D");
	//Debug
	//"em0100_attack_A" //slow attack
	//"em0100_attack_B" //fast attack
	//"em0100_attack_C" //side attack
	//"em0100_attack_D" 
	//"em0100_attack_W" //twin attack
}

void Enemy_Empusa::Attack_Update(float _DeltaTime)
{
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		EnemyFSM.ChangeState(EnemyState::M_Idle);
	}
}

void Enemy_Empusa::Attack_Exit()
{
	
}

void Enemy_Empusa::Hit_Enter()
{
	//EnemyRenderer->ChangeAnimation("em0100_angledamage_front");
	//EnemyRenderer->ChangeAnimation("em0100_angledamage_left");
	//EnemyRenderer->ChangeAnimation("em0100_angledamage_right");
	//EnemyRenderer->ChangeAnimation("em0100_angledamage_back");
	//EnemyRenderer->ChangeAnimation("em0100_air_damage");
	//EnemyRenderer->ChangeAnimation("em0100_air_damage_under");
}

void Enemy_Empusa::Hit_Update(float _DeltaTime)
{
	/*if (true==EnemyRenderer->IsAnimationEnd())
	{
		EnemyFSM.ChangeState(EnemyState::M_Idle);
	}*/
}

void Enemy_Empusa::Hit_Exit()
{
	
}

void Enemy_Empusa::Death_Enter()
{
	//EnemyRenderer->ChangeAnimation("em0100_death_front");
	//EnemyRenderer->ChangeAnimation("em0100_death_left");
	//EnemyRenderer->ChangeAnimation("em0100_death_right");
	//EnemyRenderer->ChangeAnimation("em0100_death_back");
}

void Enemy_Empusa::Death_Update(float _DeltaTime)
{
}

void Enemy_Empusa::Death_Exit()
{
}

void Enemy_Empusa::Fall_Enter()
{
}

void Enemy_Empusa::Fall_Update(float _DeltaTime)
{
}

void Enemy_Empusa::Fall_Exit()
{
}

void Enemy_Empusa::Snatch_Enter()
{
}

void Enemy_Empusa::Snatch_Update(float _DeltaTime)
{
}

void Enemy_Empusa::Snatch_Exit()
{
}

void Enemy_Empusa::Buster_Enter()
{
}

void Enemy_Empusa::Buster_Update(float _DeltaTime)
{
}

void Enemy_Empusa::Buster_Exit()
{
}



//--------------------------------------------------------------------------------------------------------
void Enemy_Empusa::RN_Idle()
{
	//플레이어 인식x
	if (false == RN_Player)
	{
		EnemyRenderer->ChangeAnimation("em0100_Idle_undetected");
	}
	//플레이어 인식o
	else
	{
		//EnemyRenderer->ChangeAnimation("em0100_Idle_variation_B"); //달리기직전
		EnemyRenderer->ChangeAnimation("em0100_Idle_variation_C");
		//EnemyRenderer->ChangeAnimation("em0100_Idle_variation_D");
	}
}

void Enemy_Empusa::Move(float _DeltaTime)
{
	static bool Moves = true;
	if (Moves == true)
	{
		EnemyRenderer->ChangeAnimation("em0100_biped_walk_start");
		Moves = false;
	}
	if (false == Moves)
	{
		ChasePlayer(_DeltaTime);
		EnemyRenderer->ChangeAnimation("em0100_biped_walk_loop");
		if (MonsterAttackRange->Collision(CollisionOrder::Player, ColType::OBBBOX3D, ColType::OBBBOX3D))
		{
			EnemyFSM.ChangeState(EnemyState::M_Attack);
			Moves = true;
		}
	}
}
