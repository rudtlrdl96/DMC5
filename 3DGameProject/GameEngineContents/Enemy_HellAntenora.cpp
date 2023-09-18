#include "PrecompileHeader.h"
#include "Enemy_HellAntenora.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

#include "AnimationEvent.h"
Enemy_HellAntenora::Enemy_HellAntenora() 
{
}

Enemy_HellAntenora::~Enemy_HellAntenora() 
{
}

void Enemy_HellAntenora::EnemyMeshLoad()
{
	std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
		{
			"Character", "Enemy", "em0001", "mesh"
		}, "em0001.FBX");

	GameEngineFBXMesh::Load(Path);

	EnemyRenderer->SetFBXMesh("em0001.fbx", "FBX");
	EnemyRenderer->GetTransform()->SetLocalScale({ 0.1f, 0.1f, 0.1f });
}

void Enemy_HellAntenora::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::HellAntenora;
	EnemyTypeValue = EnemyType::Normal;
	EnemySizeValue = EnemySize::Middle;
}

void Enemy_HellAntenora::EnemyAnimationLoad()
{
	//Animation정보 경로를 찾아서 모든animation파일 로드
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("em0001");
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

void Enemy_HellAntenora::EnemyCreateFSM()
{
}

//void Enemy_HellAntenora::Idle_Enter()
//{
//	
//}
//
//void Enemy_HellAntenora::Idle_Update(float _DeltaTime)
//{
//
//}
//
//void Enemy_HellAntenora::Idle_Exit()
//{
//
//}
//
//void Enemy_HellAntenora::Chase_Enter()
//{
//}
//
//void Enemy_HellAntenora::Chase_Update(float _DeltaTime)
//{
//}
//
//void Enemy_HellAntenora::Chase_Exit()
//{
//}
//
//void Enemy_HellAntenora::Attack_Enter()
//{
//}
//
//void Enemy_HellAntenora::Attack_Update(float _DeltaTime)
//{
//}
//
//void Enemy_HellAntenora::Attack_Exit()
//{
//}
//
//void Enemy_HellAntenora::Hit_Enter()
//{
//	
//}
//
//void Enemy_HellAntenora::Hit_Update(float _DeltaTime)
//{
//
//}
//
//void Enemy_HellAntenora::Hit_Exit()
//{
//
//}
//
//void Enemy_HellAntenora::Death_Enter()
//{
//}
//
//void Enemy_HellAntenora::Death_Update(float _DeltaTime)
//{
//}
//
//void Enemy_HellAntenora::Death_Exit()
//{
//}
//
//void Enemy_HellAntenora::Fall_Enter()
//{
//}
//
//void Enemy_HellAntenora::Fall_Update(float _DeltaTime)
//{
//}
//
//void Enemy_HellAntenora::Fall_Exit()
//{
//}
//
//void Enemy_HellAntenora::Snatch_Enter()
//{
//}
//
//void Enemy_HellAntenora::Snatch_Update(float _DeltaTime)
//{
//}
//
//void Enemy_HellAntenora::Snatch_Exit()
//{
//}
//
//void Enemy_HellAntenora::Buster_Enter()
//{
//}
//
//void Enemy_HellAntenora::Buster_Update(float _DeltaTime)
//{
//}
//
//void Enemy_HellAntenora::Buster_Exit()
//{
//}

void Enemy_HellAntenora::EnemyCreateFSM_Client()
{

}