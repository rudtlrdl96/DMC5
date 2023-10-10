#include "PrecompileHeader.h"
#include "Player_Snatch.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
Player_Snatch::Player_Snatch() 
{
}

Player_Snatch::~Player_Snatch() 
{
}

void Player_Snatch::SnatchOn(const float4& _Dir)
{
	Renderer_Snatch->SetMulColor({ 1, 1, 1, 0 });
	//GetTransform()->
	// È¸Àü
	float4 LocalForward = GetTransform()->GetWorldForwardVector().NormalizeReturn();
	float4 LookDir = _Dir.NormalizeReturn();
	float Dot = float4::DotProduct3D(LocalForward, LookDir);
	if (1.0f < Dot || LocalForward == LookDir)
	{
		return;
	}
	if (Dot < -1.0f)
	{
		GetTransform()->AddWorldRotation(float4::UP * 180);
		return;
	}
	float Angle = acosf(Dot) * GameEngineMath::RadToDeg;
	float4 Cross = float4::Cross3DReturnNormal(LocalForward, LookDir);

	if (std::isnan(Angle))
	{
		return;
	}
	if (Cross.y < 0.0f)
	{
		GetTransform()->AddWorldRotation(float4::DOWN * Angle);
	}
	else
	{
		GetTransform()->AddWorldRotation(float4::UP * Angle);
	}

}

void Player_Snatch::SnatchOff()
{
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

	Renderer_Snatch = CreateComponent<GameEngineFBXRenderer>();
	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		Renderer_Snatch->SetFBXMesh("Nero_WireArm.FBX", "FBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		Renderer_Snatch->SetFBXMesh("Nero_WireArm.FBX", "FBX");
	}
	break;
	default:
		break;
	}
}

void Player_Snatch::Update(float _DeltaTime)
{
}

