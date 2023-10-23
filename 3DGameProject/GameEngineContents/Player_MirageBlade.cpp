#include "PrecompileHeader.h"
#include "Player_MirageBlade.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "AttackCollision.h"
#include "ContentsEnum.h"
#include "EffectRenderer.h"
#include "FXSystem.h"
Player_MirageBlade::Player_MirageBlade()
{
}

Player_MirageBlade::~Player_MirageBlade()
{
}

void Player_MirageBlade::On()
{
	GameEngineActor::On();
	Renderer->EffectOption.MulColor.a = 0.0f;
	RenderAlpha = 0.0f;
	Effect->PlayFX("Mirage_On.effect");
	EffectValue = 0;
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

void Player_MirageBlade::Shoot(int _Damage, DamageType _Type)
{
	Collision->SetAttackData(_Type, _Damage, [=]
		{
			Collision->Off();
		});
	Collision->On();
	IsShoot = true;
	ResetLiveTime();
}

void Player_MirageBlade::SetSpiral()
{
	IsSpiralStop = false;
	Collision->Off();
	Effect->PlayFX("Mirage_Spiral_1.effect");
	EffectValue = 1;
	GetLevel()->TimeEvent.AddEvent(0.08f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
		{
			Effect->PlayFX("Mirage_Spiral_2.effect");
			EffectValue = 2;
		});
}

void Player_MirageBlade::SpiralStop()
{
	IsSpiralStop = true;
	Collision->Off();
	Effect->PlayFX("Mirage_Spiral_3.effect");
	EffectValue = 3;
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

	Renderer = CreateComponent<EffectRenderer>();
	Renderer->SetFBXMesh("MirageBlade.FBX", "Effect_2D");
	Renderer->GetTransform()->SetLocalRotation({ 0, 180, 0 });
	Renderer->GetTransform()->SetLocalScale({ 0.5f, 0.5f, 0.5f });
	//Renderer = CreateComponent<GameEngineFBXRenderer>();
	//Renderer->SetFBXMesh("MirageBlade.FBX", "FBX");
	//Renderer->GetTransform()->SetLocalRotation({ 0, 180, 0 });
	//Renderer->GetTransform()->SetLocalScale({ 0.5f, 0.5f, 0.5f });

	Collision = CreateComponent<AttackCollision>(CollisionOrder::PlayerAttack);
	Collision->SetColType(ColType::OBBBOX3D);
	Collision->GetTransform()->SetLocalPosition({ 0, 0, -50 });
	Collision->GetTransform()->SetLocalScale({ 25, 25, 150 });
	Collision->SetIsPlayerCollision();
	Collision->SetAttackData(DamageType::VergilLight, 9);
	Collision->SetDamageSoundType(DamageSoundType::Gun);
	Collision->Off();

	Effect = CreateComponent<FXSystem>();

	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Effect");
	NewDir.Move("Mirage");
	std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".effect" });
	for (GameEngineFile File : Files)
	{
		if (nullptr == FXData::Find(File.GetFileName()))
		{
			FXData::Load(File.GetFullPath());
		}
		Effect->CreateFX(FXData::Find(File.GetFileName()));
	}
	Effect->GetTransform()->SetLocalScale({ 0.5f, 0.5f, 0.5f });

	SetNetObjectType(Net_ActorType::MirageBlade);

	LinkData_UpdatePacket<int>(EffectValue, [this](int _BeforeData)
		{
			switch (EffectValue)
			{
			case 0:
				Effect->PlayFX("Mirage_On.effect");
				break;
			case 1:
				Effect->PlayFX("Mirage_Spiral_1.effect");
				break;
			case 2:
				Effect->PlayFX("Mirage_Spiral_2.effect");
				break;
			case 3:
				Effect->PlayFX("Mirage_Spiral_3.effect");
				break;
			default:
				break;
			}
		});
	LinkData_UpdatePacket<float>(RenderAlpha);
}

void Player_MirageBlade::Update(float _DeltaTime)
{
	Renderer->EffectOption.MulColor.a = std::min<float>(1.0f, RenderAlpha);
	if (NetControllType::ActiveControll == GameEngineNetObject::GetControllType())
	{
		RenderAlpha += _DeltaTime * 3;
		if (true == IsShoot)
		{
			GetTransform()->AddWorldPosition(GetTransform()->GetWorldForwardVector() * _DeltaTime * ShootSpeed);
			if (3.0f < GetLiveTime())
			{
				Reset();
				EffectValue = -1;
				Off();
			}
		}
	}

	Update_NetworkTrans(_DeltaTime);
}

