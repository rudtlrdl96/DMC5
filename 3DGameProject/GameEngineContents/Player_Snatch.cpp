#include "PrecompileHeader.h"
#include "Player_Snatch.h"
#include "EffectRenderer.h"
Player_Snatch::Player_Snatch()
{
}

Player_Snatch::~Player_Snatch()
{
}

void Player_Snatch::SnatchOn(const float4& _Dir)
{
	Dir = _Dir;
	FSM.ChangeState(Snatch_Shoot);
	Renderer_Snatch->EffectOption.MulColor.a = 0;
	GetTransform()->SetLocalPosition({ 55, 65, 110 });
	GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
	GetTransform()->SetWorldRotation(float4::ZERO);

	// y 축 회전
	float4 LocalForward = GetTransform()->GetWorldForwardVector();
	LocalForward.y = 0;
	LocalForward.Normalize();
	float4 LookDir = _Dir;
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
	LookDir = _Dir;

	Dot = float4::DotProduct3D(LocalForward, LookDir);
	Angle = acosf(Dot) * GameEngineMath::RadToDeg;
	Cross = float4::Cross3DReturnNormal(LocalForward, LookDir);
	if (0 < _Dir.z)
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

void Player_Snatch::SnatchOff()
{
	FSM.ChangeState(Snatch_Back);
}

void Player_Snatch::Start()
{
	std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
		{
			"Character", "Player", "Nero", "Wire"
		});
	if (nullptr == GameEngineFBXMesh::Find("Nero_WireArm.FBX"))
	{
		GameEngineFBXMesh::Load(Path + "\\Nero_WireArm.fbx");
	}

	Renderer_Snatch = CreateComponent<EffectRenderer>();
	Renderer_Snatch->SetFBXMesh("Nero_WireArm.FBX", "Effect_2D");
	Renderer_Snatch->GetTransform()->SetLocalScale({ 2.0f, 2.0f, 1.0f });

	FSM.CreateState({ .StateValue = Snatch_Off,
	.Start = [=] {
	},
	.Update = [=](float _DeltaTime)
	{
		Renderer_Snatch->EffectOption.MulColor.a = std::clamp(Renderer_Snatch->EffectOption.MulColor.a - _DeltaTime * 5.0f, 0.0f, 1.0f);
	},
	.End = [=]
	{

	}
	});

	FSM.CreateState({ .StateValue = Snatch_Shoot,
	.Start = [=] {
		FSMTimer = 0;
	},
	.Update = [=](float _DeltaTime)
	{
		FSMTimer += _DeltaTime;
		Renderer_Snatch->EffectOption.MulColor.a = std::clamp(FSMTimer * 5.0f, 0.0f, 1.0f);
		GetTransform()->AddWorldPosition(Dir * 4000.0f * _DeltaTime);
		if (0.3f < FSMTimer)
		{
			FSM.ChangeState(Snatch_Back);
			return;
		}

	},
	.End = [=]
	{

	}
		});

	FSM.CreateState({ .StateValue = Snatch_Back,
	.Start = [=] {
	},
	.Update = [=](float _DeltaTime)
	{
		FSMTimer -= _DeltaTime;
		Renderer_Snatch->EffectOption.MulColor.a = std::clamp(FSMTimer * 5.0f, 0.0f, 1.0f);
		GetTransform()->AddWorldPosition(Dir * -4000.0f * _DeltaTime);

		if (GetTransform()->GetLocalPosition().z < 100.0f)
		{
			FSM.ChangeState(Snatch_Off);
		}
	},
	.End = [=]
	{
	
	}
	});
	FSM.ChangeState(Snatch_Off);
	Renderer_Snatch->EffectOption.MulColor.a = 0.0f;
}

void Player_Snatch::Update(float _DeltaTime)
{
	FSM.Update(_DeltaTime);
}

