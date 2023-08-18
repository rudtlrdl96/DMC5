#include "PrecompileHeader.h"
#include "Enemy_HellAntenora.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

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

	EnemyRenderer->SetFBXMesh("em0001.fbx", "NoneAlphaMesh");
	EnemyRenderer->GetTransform()->SetLocalScale({ 0.1f, 0.1f, 0.1f });
}

void Enemy_HellAntenora::EnemyTextureLoad()
{
}

void Enemy_HellAntenora::EnemyAnimationLoad()
{
}

void Enemy_HellAntenora::EnemyCreateFSM()
{
}
