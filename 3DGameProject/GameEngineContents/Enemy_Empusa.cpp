#include "PrecompileHeader.h"
#include "Enemy_Empusa.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>

Enemy_Empusa::Enemy_Empusa() 
{
}

Enemy_Empusa::~Enemy_Empusa() 
{
}

void Enemy_Empusa::EnemyMeshLoad()
{
	if (nullptr == GameEngineFBXMesh::Find("em0100.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Character", "Enemy", "em0100", "mesh"
			}, "em0100.FBX");

		GameEngineFBXMesh::Load(Path);
	}
	
	EnemyRenderer->SetFBXMesh("em0100.fbx", "MeshAniTexture");
	EnemyRenderer->GetTransform()->SetLocalScale({ 0.1f , 0.1f , 0.1f });
}

void Enemy_Empusa::EnemyTextureLoad()
{
	std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
		{
			"Character", "Enemy", "em0100", "animation", "attack"
		}, "em0100_attack_A.FBX");

	GameEngineFBXAnimation::Load(Path);
	EnemyRenderer->CreateFBXAnimation("AttackA", "em0100_attack_A.FBX");
	EnemyRenderer->ChangeAnimation("AttackA");
}

void Enemy_Empusa::EnemyAnimationLoad()
{
}

void Enemy_Empusa::EnemyCreateFSM()
{
}
