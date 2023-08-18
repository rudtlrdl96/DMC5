#include "PrecompileHeader.h"
#include "Enemy_HellCaina.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

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

	EnemyRenderer->SetFBXMesh("em0000.fbx", "NoneAlphaMesh");
	EnemyRenderer->GetTransform()->SetLocalScale({ 0.1f, 0.1f, 0.1f });
}

void Enemy_HellCaina::EnemyTextureLoad()
{
}

void Enemy_HellCaina::EnemyAnimationLoad()
{
}

void Enemy_HellCaina::EnemyCreateFSM()
{
}
