#include "PrecompileHeader.h"
#include "l19_Portal.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "LevelChangeZone.h"

l19_Portal::l19_Portal()
{

}

l19_Portal::~l19_Portal()
{

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
}
