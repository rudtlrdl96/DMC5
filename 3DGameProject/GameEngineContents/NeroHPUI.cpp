#include "PrecompileHeader.h"
#include "NeroHPUI.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include "UIFBXRenderer.h"
NeroHPUI::NeroHPUI() 
{
}

NeroHPUI::~NeroHPUI() 
{
}

void NeroHPUI::Start()
{
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Mesh");
	NewDir.Move("UIMesh");
	NewDir.Move("NeroUIMesh");
	if (nullptr == GameEngineFBXMesh::Find("NeroIden3.FBX"))
	{
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
		for (GameEngineFile File : Files)
		{
			GameEngineFBXMesh::Load(File.GetFullPath());
		}
	}

	NeroUI_HPGlass = CreateComponent<UIFBXRenderer>();
	NeroUI_HPGlass->SetFBXMesh("NeroHPUI.FBX", "FBX");
	NeroUI_HPGlass->GetTransform()->SetLocalPosition({ -535.0f,275.0f,0.0f });
	NeroUI_HPGlass->GetTransform()->SetLocalRotation({ -90.0f,0.0f,0.0f });
	NeroUI_HPGlass->GetTransform()->SetLocalScale({ 0.6f,0.6f,0.6f });

	NeroUI_Hud1 = CreateComponent<UIFBXRenderer>();
	NeroUI_Hud1->SetFBXMesh("NeroHud123.FBX", "FBX");
	NeroUI_Hud1->GetTransform()->SetLocalPosition({ -702.0f,245.0f,-153.0f });
	NeroUI_Hud1->GetTransform()->SetLocalRotation({ 29.0f,-102.0f,-170.0f });
	NeroUI_Hud1->GetTransform()->SetLocalScale({ 0.9f,0.9f,0.9f });
	
	NeroUI_Hud2 = CreateComponent<UIFBXRenderer>();
	NeroUI_Hud2->SetFBXMesh("NeroIden2.FBX", "FBX");
	NeroUI_Hud2->GetTransform()->SetLocalPosition({ -698.0f,289.0f,-300.0f });
	NeroUI_Hud2->GetTransform()->SetLocalRotation({ 25.0f,-87.0f,1.0f });
	NeroUI_Hud2->GetTransform()->SetLocalScale({ 0.9f,0.9f,0.9f });
	
	NeroUI_Hud3 = CreateComponent<UIFBXRenderer>();
	NeroUI_Hud3->SetFBXMesh("NeroIden3.FBX", "FBX");
	NeroUI_Hud3->GetTransform()->SetLocalPosition({ -690.0f,340.0f,-300.0f });
	NeroUI_Hud3->GetTransform()->SetLocalRotation({ 22.0f,-60.0f,10.0f });
	NeroUI_Hud3->GetTransform()->SetLocalScale({ 0.9f,0.9f,0.9f });

	std::shared_ptr<GameEngineUIRenderer> NeroUI_Back = CreateComponent<GameEngineUIRenderer>();
	NeroUI_Back->SetTexture("HudBack.png");
	NeroUI_Back->GetTransform()->SetLocalScale({ 97.0f,131.0f,0.0f });
	NeroUI_Back->GetTransform()->SetLocalPosition({-696.0f,337.0f,0.0f});
}

void NeroHPUI::Update(float _DeltaTime)
{
}

