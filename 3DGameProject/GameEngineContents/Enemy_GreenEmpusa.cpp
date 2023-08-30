#include "PrecompileHeader.h"
#include "Enemy_GreenEmpusa.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

Enemy_GreenEmpusa::Enemy_GreenEmpusa() 
{
}

Enemy_GreenEmpusa::~Enemy_GreenEmpusa() 
{
}

void Enemy_GreenEmpusa::EnemyMeshLoad()
{
	std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
		{
			"Character", "Enemy", "em0101", "mesh"
		}, "em0101.FBX");

	GameEngineFBXMesh::Load(Path);

	EnemyRenderer->SetFBXMesh("em0101.fbx", "FBX");
	EnemyRenderer->GetTransform()->SetLocalScale({ 0.1f, 0.1f, 0.1f });
}

void Enemy_GreenEmpusa::EnemyTypeLoad()
{
}

void Enemy_GreenEmpusa::EnemyAnimationLoad()
{
}

void Enemy_GreenEmpusa::EnemyCreateFSM()
{
}