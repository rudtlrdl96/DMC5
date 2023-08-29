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

	EnemyRenderer->SetFBXMesh("em0001.fbx", "NoneAlphaMesh");
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
	//Animation���� ��θ� ã�Ƽ� ���animation���� �ε�
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
				//Functional����� �Լ��� ���°�
			},
			.CallBacks_int = {
				std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1)} //ChangeState,��������
		}
	);
}

void Enemy_HellAntenora::EnemyCreateFSM()
{
}
