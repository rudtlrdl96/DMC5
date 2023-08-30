#include "PrecompileHeader.h"
#include "Enemy_HellCaina.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>

#include "AnimationEvent.h"
Enemy_HellCaina::Enemy_HellCaina() 
{
}

Enemy_HellCaina::~Enemy_HellCaina() 
{
}

void Enemy_HellCaina::EnemyMeshLoad()
{
	std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
		{
			"Character", "Enemy", "em0000", "mesh"
		}, "em0000.FBX");

	GameEngineFBXMesh::Load(Path);

	EnemyRenderer->SetFBXMesh("em0000.fbx", "AniFBX");
	EnemyRenderer->GetTransform()->SetLocalScale({ 0.1f, 0.1f, 0.1f });
}

void Enemy_HellCaina::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::HellCaina;
	EnemyTypeValue = EnemyType::Normal;
	EnemySizeValue = EnemySize::Middle;
}

void Enemy_HellCaina::EnemyAnimationLoad()
{
	//Animation정보 경로를 찾아서 모든animation파일 로드
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("em0000");
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

void Enemy_HellCaina::EnemyCreateFSM()
{
}

void Enemy_HellCaina::Idle_Enter()
{
	
}

void Enemy_HellCaina::Idle_Update(float _DeltaTime)
{

}

void Enemy_HellCaina::Idle_Exit()
{

}

void Enemy_HellCaina::Chase_Enter()
{
}

void Enemy_HellCaina::Chase_Update(float _DeltaTime)
{
}

void Enemy_HellCaina::Chase_Exit()
{
}

void Enemy_HellCaina::Attack_Enter()
{
}

void Enemy_HellCaina::Attack_Update(float _DeltaTime)
{
}

void Enemy_HellCaina::Attack_Exit()
{
}

void Enemy_HellCaina::Hit_Enter()
{
	
}

void Enemy_HellCaina::Hit_Update(float _DeltaTime)
{

}

void Enemy_HellCaina::Hit_Exit()
{

}

void Enemy_HellCaina::Death_Enter()
{
}

void Enemy_HellCaina::Death_Update(float _DeltaTime)
{
}

void Enemy_HellCaina::Death_Exit()
{
}

void Enemy_HellCaina::Fall_Enter()
{
}

void Enemy_HellCaina::Fall_Update(float _DeltaTime)
{
}

void Enemy_HellCaina::Fall_Exit()
{
}

void Enemy_HellCaina::Snatch_Enter()
{
}

void Enemy_HellCaina::Snatch_Update(float _DeltaTime)
{
}

void Enemy_HellCaina::Snatch_Exit()
{
}

void Enemy_HellCaina::Buster_Enter()
{
}

void Enemy_HellCaina::Buster_Update(float _DeltaTime)
{
}

void Enemy_HellCaina::Buster_Exit()
{
}


