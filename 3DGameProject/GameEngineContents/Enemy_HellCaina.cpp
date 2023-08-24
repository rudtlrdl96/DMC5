#include "PrecompileHeader.h"
#include "Enemy_HellCaina.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>

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

	EnemyRenderer->SetFBXMesh("em0000.fbx", "NoneAlphaAniMesh");
	EnemyRenderer->GetTransform()->SetLocalScale({ 0.1f, 0.1f, 0.1f });
}

void Enemy_HellCaina::EnemyTextureLoad()
{
}

void Enemy_HellCaina::EnemyAnimationLoad()
{
	//std::string RootPath = GameEnginePath::GetFileFullPath("ContentResources",
	//	{
	//		"Character", "Enemy", "em0000", "animation",
	//	});
	//
	//GameEngineDirectory Path = GameEngineDirectory(RootPath);
	//
	//{
	//	Path.Move("attack");
	//
	//	std::vector<GameEngineFile> Files = Path.GetAllFile({ ".fbx" });
	//	
	//	for (size_t i = 0; i < Files.size(); i++)
	//	{
	//		GameEngineFBXMesh::Load(Files[i].GetFullPath());
	//	}
	//
	//	Path.MoveParent();
	//}

	std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
		{
			"Character", "Enemy", "em0000", "animation", "attack"
		}, "em0000_attack_01.FBX");

	GameEngineFBXAnimation::Load(Path);

	EnemyRenderer->CreateFBXAnimation("Attack01", "em0000_attack_01.fbx");
	EnemyRenderer->ChangeAnimation("Attack01");
}

void Enemy_HellCaina::EnemyCreateFSM()
{
}
