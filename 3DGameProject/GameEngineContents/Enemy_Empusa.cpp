#include "PrecompileHeader.h"
#include "Enemy_Empusa.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>

#include "AnimationEvent.h"
#include "EnemyActor_Normal.h"

Enemy_Empusa::Enemy_Empusa() 
{
}

Enemy_Empusa::~Enemy_Empusa() 
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
	EnemyRenderer->SetFBXMesh("em0100.fbx", "MeshAniTexture");
	EnemyRenderer->GetTransform()->SetLocalScale({ 0.1f , 0.1f , 0.1f });
}

void Enemy_Empusa::EnemyTypeLoad()
{
	//Type설정
	EnemyCodeValue = EnemyCode::Empusa;
	EnemyTypeValue = EnemyType::Normal;
	EnemySizeValue = EnemySize::Middle;
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
				std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1)} //ChangeState,인자있음
		}
	);
}

void Enemy_Empusa::EnemyCreateFSM()
{

}

void Enemy_Empusa::Idle_Enter()
{
	EnemyRenderer->ChangeAnimation("em0100_Idle_undetected");
	//EnemyRenderer->ChangeAnimation("em0100_Idle_variation_A");
	//EnemyRenderer->ChangeAnimation("em0100_Idle_variation_B");
	//EnemyRenderer->ChangeAnimation("em0100_Idle_variation_C");
	//EnemyRenderer->ChangeAnimation("em0100_Idle_variation_D");
}

void Enemy_Empusa::Idle_Update(float _DeltaTime)
{

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
}

void Enemy_Empusa::Attack_Update(float _DeltaTime)
{
}

void Enemy_Empusa::Attack_Exit()
{
}

void Enemy_Empusa::Hit_Enter()
{
	EnemyRenderer->ChangeAnimation("em0100_angledamage_front");
	//EnemyRenderer->ChangeAnimation("em0100_angledamage_left");
	//EnemyRenderer->ChangeAnimation("em0100_angledamage_right");
	//EnemyRenderer->ChangeAnimation("em0100_angledamage_back");
	//EnemyRenderer->ChangeAnimation("em0100_air_damage");
	//EnemyRenderer->ChangeAnimation("em0100_air_damage_under");
	//EnemyRenderer->ChangeAnimation("em0100_blown_front_landing");
	//EnemyRenderer->ChangeAnimation("em0100_blown_front_loop");
	//EnemyRenderer->ChangeAnimation("em0100_blown_front_start");
	//EnemyRenderer->ChangeAnimation("em0100_air_damage_under");
}

void Enemy_Empusa::Hit_Update(float _DeltaTime)
{
	int a = 0;
	if (true==EnemyRenderer->IsAnimationEnd())
	{
		EnemyFSM.ChangeState(EnemyState::M_Idle);
	}
}

void Enemy_Empusa::Hit_Exit()
{
	
}

void Enemy_Empusa::Death_Enter()
{
	EnemyRenderer->ChangeAnimation("em0100_death_front");
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
