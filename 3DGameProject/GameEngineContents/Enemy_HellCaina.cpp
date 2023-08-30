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
	//Animation���� ��θ� ã�Ƽ� ���animation���� �ε�
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
				//Functional����� �Լ��� ���°�
			},
			.CallBacks_int = {
				std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1)} //ChangeState,��������
		}
	);
}

void Enemy_HellCaina::EnemyCreateFSM()
{
}
