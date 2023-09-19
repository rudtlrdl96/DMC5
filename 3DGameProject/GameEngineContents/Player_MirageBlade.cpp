#include "PrecompileHeader.h"
#include "Player_MirageBlade.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "AttackCollision.h"
#include "ContentsEnum.h"
Player_MirageBlade::Player_MirageBlade()
{
}

Player_MirageBlade::~Player_MirageBlade()
{
}

void Player_MirageBlade::Reset()
{
	IsShoot = false;
	ResetLiveTime();
}

void Player_MirageBlade::Shoot()
{
	IsShoot = true;
}

void Player_MirageBlade::Start()
{
	if (nullptr == GameEngineFBXMesh::Find("MirageBlade.FBX"))
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Character");
		NewDir.Move("Player");
		NewDir.Move("Vergil");
		NewDir.Move("MirageBlade");
		GameEngineFBXMesh::Load(NewDir.GetPlusFileName("MirageBlade.fbx").GetFullPath());
	}

	Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("MirageBlade.FBX", "FBX");
	Renderer->GetTransform()->SetLocalRotation({ 0, 180, 0 });
	Renderer->GetTransform()->SetLocalScale({ 0.5f, 0.5f, 0.5f });

	Collision = CreateComponent<AttackCollision>(CollisionOrder::PlayerAttack);
	Collision->SetColType(ColType::OBBBOX3D);
	Collision->GetTransform()->SetLocalPosition({ 0, 0, -50 });
	Collision->GetTransform()->SetLocalScale({ 25, 25, 150 });
	Collision->SetAttackData(DamageType::Light, 10);
	//Collision->Off();

}

void Player_MirageBlade::Update(float _DeltaTime)
{
	if (true == IsShoot)
	{
		GetTransform()->AddWorldPosition(GetTransform()->GetWorldForwardVector() * _DeltaTime * ShootSpeed);
		if (5.0f < GetLiveTime())
		{
			Reset();
			Off();
		}
	}
}

