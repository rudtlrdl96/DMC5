#include "PrecompileHeader.h"
#include "Enemy_RedEmpusa.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

Enemy_RedEmpusa::Enemy_RedEmpusa() 
{
}

Enemy_RedEmpusa::~Enemy_RedEmpusa() 
{
}

void Enemy_RedEmpusa::EnemyMeshLoad()
{
	std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
		{
			"Character", "Enemy", "em0102", "mesh"
		}, "em0102.FBX");

	GameEngineFBXMesh::Load(Path);

	EnemyRenderer->SetFBXMesh("em0102.fbx", "NoneAlphaMesh");
	EnemyRenderer->GetTransform()->SetLocalScale({ 0.1f, 0.1f, 0.1f });
}

void Enemy_RedEmpusa::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::RedEmpusa;
	EnemyTypeValue = EnemyType::Normal;
	EnemySizeValue = EnemySize::Middle;
}

void Enemy_RedEmpusa::EnemyAnimationLoad()
{
}

void Enemy_RedEmpusa::EnemyCreateFSM()
{
}
