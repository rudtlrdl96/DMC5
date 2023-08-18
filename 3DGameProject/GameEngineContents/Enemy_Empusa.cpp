#include "PrecompileHeader.h"
#include "Enemy_Empusa.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

Enemy_Empusa::Enemy_Empusa() 
{
}

Enemy_Empusa::~Enemy_Empusa() 
{
}

void Enemy_Empusa::EnemyMeshLoad()
{
	std::string Path = GameEnginePath::GetFileFullPath("ContentResources", 
		{
			"Character", "Enemy", "em0100", "mesh"
		}, "em0100.FBX");

	GameEngineFBXMesh::Load(Path);

	EnemyRenderer->SetFBXMesh("em0100.fbx", "NoneAlphaMesh");
}

void Enemy_Empusa::EnemyTextureLoad()
{
}

void Enemy_Empusa::EnemyAnimationLoad()
{
}

void Enemy_Empusa::EnemyCreateFSM()
{
}
