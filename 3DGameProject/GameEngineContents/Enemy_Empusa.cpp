#include "PrecompileHeader.h"
#include "Enemy_Empusa.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>

#include "AnimationEvent.h"
#include "EnemyActor_Normal.h"

Enemy_Empusa::Enemy_Empusa() 
{
}

Enemy_Empusa::~Enemy_Empusa() 
{
}

void Enemy_Empusa::EnemyMeshLoad()
{
	//FBX���ϰ�θ� ã�Ƽ� �ε�
	if (nullptr == GameEngineFBXMesh::Find("em0100.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Character", "Enemy", "em0100", "mesh"
			}, "em0100.FBX");

		GameEngineFBXMesh::Load(Path);
	}
	
	//EnemyBase���� Start���� ������ Component�� Mesh Set

	std::vector<std::vector<std::string>> MaterialNames = { {"FBX", "FBX", "FBX", "FBX", "FBX_Mix"}};

	EnemyRenderer->SetFBXMesh("em0100.fbx", MaterialNames);

	std::shared_ptr<GameEngineRenderUnit> MeshUnits = EnemyRenderer->GetRenderUnit(0, 4);

	if (nullptr != MeshUnits)
	{
		if (nullptr == GameEngineTexture::Find("em0100_appear_albm.tga"))
		{
			std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
				{
					"Character", "Enemy", "em0100", "mesh"
				}, "em0100_appear_albm.tga");

			GameEngineTexture::Load(Path);
		}

		MeshUnits->ShaderResHelper.SetTexture("MixTexture", "em0100_appear_albm.tga");
	}

	EnemyRenderer->GetTransform()->SetLocalScale({ 0.1f , 0.1f , 0.1f });
}

void Enemy_Empusa::EnemyTypeLoad()
{
	//Type����
	EnemyCodeValue = EnemyCode::Empusa;
	EnemyTypeValue = EnemyType::Normal;
	EnemySizeValue = EnemySize::Middle;
}

void Enemy_Empusa::EnemyAnimationLoad()
{
	//Animation���� ��θ� ã�Ƽ� ���animation���� �ε�
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("em0100");
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

void Enemy_Empusa::EnemyCreateFSM()
{

}
