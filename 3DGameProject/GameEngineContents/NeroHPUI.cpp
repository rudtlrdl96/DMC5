#include "PrecompileHeader.h"
#include "NeroHPUI.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "UIFBXRenderer.h"
NeroHPUI::NeroHPUI() 
{
}

NeroHPUI::~NeroHPUI() 
{
}

void NeroHPUI::Start()
{
	std::string Path = GameEnginePath::GetFileFullPath
	(
		"ContentResources",
		{
			"Mesh", "UIMesh" ,"NeroHPGlass" ,
		},
		"NeroHPUI.fbx"
		);
	
	GameEngineFBXMesh::Load(Path);

	NeroUI_HPGlass = CreateComponent<UIFBXRenderer>();
	NeroUI_HPGlass->SetFBXMesh("NeroHPUI.FBX", "FBX");
	NeroUI_HPGlass->GetTransform()->SetLocalPosition({ -480.0f,250.0f,0.0f });
	NeroUI_HPGlass->GetTransform()->SetLocalRotation({ -90.0f,0.0f,0.0f });
	NeroUI_HPGlass->GetTransform()->SetLocalScale({ 0.7f,0.7f,0.7f });
}

void NeroHPUI::Update(float _DeltaTime)
{
}

