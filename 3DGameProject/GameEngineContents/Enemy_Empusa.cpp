#include "PrecompileHeader.h"
#include "Enemy_Empusa.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "BaseEnemyActor.h"

#include "AnimationEvent.h"
#include "BasePlayerActor.h"
#include "AttackCollision.h"

Enemy_Empusa::Enemy_Empusa() 
{
}

Enemy_Empusa::~Enemy_Empusa() 
{
}

void Enemy_Empusa::DamageCollisionCheck()
{

}

void Enemy_Empusa::Start()
{
	BaseEnemyActor::Start();
	SetNetObjectType(Net_ActorType::Empusa);
}

void Enemy_Empusa::Update(float _DeltaTime)
{
	BaseEnemyActor::Update(_DeltaTime);
}

void Enemy_Empusa::EnemyMeshLoad()
{
	//FBX파일경로를 찾아서 로드
	if (nullptr == GameEngineFBXMesh::Find("em0100.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Character", "Enemy", "em0100", "mesh"
			}, "em0100.FBX");

		GameEngineFBXMesh::Load(Path);
	}
	
	//EnemyBase에서 Start에서 생성된 Component에 Mesh Set
	EnemyRenderer->SetFBXMesh("em0100.fbx", "AniFBX_Low");
	EnemyRenderer->GetTransform()->SetLocalScale({ 1.0f , 1.0f , 1.0f });
}

void Enemy_Empusa::EnemyTypeLoad()
{
	//Type설정
	EnemyCodeValue = EnemyCode::Empusa;
	EnemyTypeValue = EnemyType::Normal;
	EnemySizeValue = EnemySize::Middle;
	EnemyHP = 100.0f;
	RN_Range = float4::ZERO;;
	RN_Player = false;
	MoveSpeed = 50.0f;
}

void Enemy_Empusa::EnemyAnimationLoad()
{
	//Animation정보 경로를 찾아서 모든animation파일 로드
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
				//Functional사용할 함수들 적는곳
			},
			.CallBacks_int = {
				std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1)
			}, //ChangeState,인자있음
			.CallBacks_float4 = {
			
			}
		}
	);
}

void Enemy_Empusa::EnemyCreateFSM()
{
	MonsterCollision->GetTransform()->SetWorldScale({300,100,300});
	MonsterCollision->SetColType(ColType::OBBBOX3D);
	RN_MonsterCollision->GetTransform()->SetWorldScale({800,800,800});
	RN_MonsterCollision->SetColType(ColType::OBBBOX2D);
	MonsterAttackRange->GetTransform()->SetWorldScale({300,300,300});
	MonsterAttackRange->SetColType(ColType::SPHERE3D);
}
