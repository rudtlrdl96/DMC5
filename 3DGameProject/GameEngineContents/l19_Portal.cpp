#include "PrecompileHeader.h"
#include "l19_Portal.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "LevelChangeZone.h"
#include <GameEngineCore/PhysXTriangleComponent.h>

l19_Portal::l19_Portal()
{

}

l19_Portal::~l19_Portal()
{

}

void l19_Portal::On()
{
	GameEngineObject::On();
}

void l19_Portal::Start()
{
	FBXMesh = CreateComponent<GameEngineFBXRenderer>();

	if (GameEngineOption::GetOption("Shader") == GameEngineOptionValue::Low)
	{
		FBXMesh->SetFBXMesh("sm0906_m14finaldoor.fbx", "FBX" + std::string("_Low"));
	}
	else
	{
		FBXMesh->SetFBXMesh("sm0906_m14finaldoor.fbx", "FBX");
	}
	FBXMesh->SetDynamic();
	FBXMesh->ShadowOn();

	AcLevelChangeZone = GetLevel()->CreateActor<LevelChangeZone>();
	AcLevelChangeZone->SetChangeLevel("Location11_Level");
	AcLevelChangeZone->GetTransform()->SetParent(GetTransform());
	AcLevelChangeZone->GetTransform()->SetLocalPosition({0.f,350.f,0.f});
	AcLevelChangeZone->GetTransform()->SetLocalScale({450.f,700.f,50.f});

	GetTransform()->SetLocalPosition(StartPos);
	Off();
}

void l19_Portal::Update(float _DeltaTime)
{
	if (Ratio >= 1.f)
	{
		return;
	}
	Time += _DeltaTime;
	Ratio = Time / 2;
	GetTransform()->SetWorldPosition(float4::LerpClamp(StartPos, EndPos, Ratio));
}