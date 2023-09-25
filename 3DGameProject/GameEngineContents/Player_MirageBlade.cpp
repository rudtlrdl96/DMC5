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
	TargetTransform = nullptr;
}

void Player_MirageBlade::SetTarget(GameEngineTransform* _Transform)
{
	TargetTransform = _Transform;
}

void Player_MirageBlade::LookTarget()
{
	if (IsShoot == true) { return; }
	if (nullptr != TargetTransform)
	{
		// y 축 회전
		float4 LocalForward = GetTransform()->GetWorldForwardVector();
		LocalForward.y = 0;
		LocalForward.Normalize();
		float4 LookDir = TargetTransform->GetWorldPosition() - GetTransform()->GetWorldPosition();
		LookDir.y = 0;
		LookDir.Normalize();
		float Dot = float4::DotProduct3D(LocalForward, LookDir);
		float Angle = acosf(Dot) * GameEngineMath::RadToDeg;
		float4 Cross = float4::Cross3DReturnNormal(LocalForward, LookDir);
		if (false == std::isnan(Angle))
		{
			if (Cross.y < 0.0f)
			{
				GetTransform()->AddWorldRotation({ 0, -Angle, 0 });
			}
			else
			{
				GetTransform()->AddWorldRotation({ 0, Angle, 0 });
			}
		}

		// x 축 회전
		LocalForward = GetTransform()->GetWorldForwardVector();
		LocalForward.Normalize();
		LookDir = TargetTransform->GetWorldPosition() + float4::UP * 100 - GetTransform()->GetWorldPosition();
		LookDir.Normalize();

		Dot = float4::DotProduct3D(LocalForward, LookDir);
		Angle = acosf(Dot) * GameEngineMath::RadToDeg;
		Cross = float4::Cross3DReturnNormal(LocalForward, LookDir);
		if (GetTransform()->GetWorldPosition().z < TargetTransform->GetWorldPosition().z)
		{
			Cross = -Cross;
		}
		if (false == std::isnan(Angle))
		{
			if (Cross.x < 0.0f)
			{
				GetTransform()->AddWorldRotation({ Angle, 0, 0 });
			}
			else
			{
				GetTransform()->AddWorldRotation({ -Angle, 0, 0 });
			}
		}
	}
}

void Player_MirageBlade::Shoot()
{
	IsShoot = true;
	ResetLiveTime();
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
	SetNetObjectType(Net_ActorType::MirageBlade);

	//LinkData_UpdatePacket<bool>(TestBool);
	//LinkData_UpdatePacket<int>(TestInt);
}

void Player_MirageBlade::Update(float _DeltaTime)
{
	Update_NetworkTrans(_DeltaTime);
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

