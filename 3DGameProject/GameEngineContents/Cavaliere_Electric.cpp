#include "PrecompileHeader.h"
#include "Cavaliere_Electric.h"
#include "FXSystem.h"
#include "AttackCollision.h"
Cavaliere_Electric::Cavaliere_Electric() 
{
}

Cavaliere_Electric::~Cavaliere_Electric() 
{
}

void Cavaliere_Electric::LookTarget(const float4& _Target)
{
	GetTransform()->SetWorldRotation(float4::ZERO);
	// y 축 회전
	float4 LocalForward = GetTransform()->GetWorldForwardVector();
	LocalForward.y = 0;
	LocalForward.Normalize();
	float4 LookDir = _Target - GetTransform()->GetWorldPosition();
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
	LookDir = _Target + float4::UP * 50 - GetTransform()->GetWorldPosition();
	LookDir.Normalize();

	Dot = float4::DotProduct3D(LocalForward, LookDir);
	Angle = acosf(Dot) * GameEngineMath::RadToDeg;
	Cross = float4::Cross3DReturnNormal(LocalForward, LookDir);
	if (GetTransform()->GetWorldPosition().z < _Target.z)
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

void Cavaliere_Electric::Shoot()
{
	GetTransform()->SetLocalPosition({-25, 275, 0});
	GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
	ResetLiveTime();
	On();
	Col->On();
	IsShoot = true;

	Effect->PlayFX("Cavalier_Electric_1.effect");

	TimeEvent.AddEvent(0.17f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
	{
		Effect->PlayFX("Cavalier_Electric_2.effect");
		Effect->Loop = true;
	});
	GameEngineSound::Play("Cavaliere_SFX_11.wav");
}

void Cavaliere_Electric::Start()
{
	Effect = CreateComponent<FXSystem>();
	std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
	{
		"Effect", "CavalierAngelo_Electric"
	});
	if (nullptr == FXData::Find("Cavalier_Electric_1.effect"))
	{
		FXData::Load(Path + "\\Cavalier_Electric_1.effect");
		FXData::Load(Path + "\\Cavalier_Electric_2.effect");
		FXData::Load(Path + "\\Cavalier_Electric_3.effect");
	}
	Effect->CreateFX(FXData::Find("Cavalier_Electric_1.effect"));
	Effect->CreateFX(FXData::Find("Cavalier_Electric_2.effect"));
	Effect->CreateFX(FXData::Find("Cavalier_Electric_3.effect"));
	Effect->GetTransform()->SetLocalPosition({ 0, 0, 100 });
	Effect->GetTransform()->SetLocalScale({ 0.5f, 0.5f, 0.5f });

	Col = CreateComponent<AttackCollision>(CollisionOrder::EnemyAttack);
	Col->GetTransform()->SetLocalScale({ 100, 100, 100 });
	Col->SetAttackData(DamageType::Light, 100, [=] {
		GameEngineSound::Play("Cavaliere_SFX_10.wav");
		IsShoot = false;
		Col->Off();
		TimeEvent.ClearEvent();
		Effect->PlayFX("Cavalier_Electric_3.effect");
		Effect->Loop = false;
		TimeEvent.AddEvent(0.2f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				Off();
			});
	});

	Off();
}

void Cavaliere_Electric::Update(float _DeltaTime)
{
	if (3.0f < GetLiveTime())
	{
		Off();
	}
	TimeEvent.Update(_DeltaTime);
	if (true == IsShoot)
	{
		GetTransform()->AddWorldPosition(GetTransform()->GetWorldForwardVector() * Speed * _DeltaTime);
	}
}

