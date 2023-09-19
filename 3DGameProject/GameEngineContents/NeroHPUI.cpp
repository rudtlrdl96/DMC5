#include "PrecompileHeader.h"
#include "NeroHPUI.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include "UIFBXRenderer.h"
#include "UI_HPGaege.h"
#include "UI_DTGauge.h"
#include "UI_BulletGauge.h"
NeroHPUI::NeroHPUI() 
{
}

NeroHPUI::~NeroHPUI() 
{
}
/*
후드,후드불
HP바 1줄
변신게이지
총알
*/
void NeroHPUI::Start()
{
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Mesh");
	NewDir.Move("UIMesh");
	NewDir.Move("NeroUIMesh");
	NewDir.Move("NeroHPMesh");

	if (nullptr == GameEngineFBXMesh::Find("NeroIden3.FBX"))
	{
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
		for (GameEngineFile File : Files)
		{
			GameEngineFBXMesh::Load(File.GetFullPath());
		}
	}

	NeroUI_HPGlass = CreateComponent<UIFBXRenderer>(0);
	NeroUI_HPGlass->SetFBXMesh("Nero_HPGlass.FBX", "FBX_Low");
	NeroUI_HPGlass->GetTransform()->SetLocalPosition({ -535.0f,273.0f,172.0f });
	NeroUI_HPGlass->GetTransform()->SetLocalRotation({ -90.0f,0.0f,0.0f });
	NeroUI_HPGlass->GetTransform()->SetLocalScale({ 0.6f,0.6f,0.6f });
	NeroUI_HPGlass->LightOff();
	NeroUI_Hud1 = CreateComponent<UIFBXRenderer>(2);
	NeroUI_Hud1->SetFBXMesh("NeroEx01.FBX", "FBX_Low");
	NeroUI_Hud1->GetTransform()->SetLocalPosition({ -711.0f,303.0f,20.0f });
	NeroUI_Hud1->GetTransform()->SetLocalRotation({ 30.0f,-90.0f,-30.0f });
	NeroUI_Hud1->GetTransform()->SetLocalScale({ 0.7f,0.7f,0.7f });
	NeroUI_Hud1->LightOff();
	NeroUI_Hud2 = CreateComponent<UIFBXRenderer>(2);
	NeroUI_Hud2->SetFBXMesh("NeroEx02.FBX", "FBX_Low");
	NeroUI_Hud2->GetTransform()->SetLocalPosition({ -708.0f,334.0f,0.0f });
	NeroUI_Hud2->GetTransform()->SetLocalRotation({ 30.0f,-90.0f,-30.0f });
	NeroUI_Hud2->GetTransform()->SetLocalScale({ 0.7f,0.7f,0.7f });
	NeroUI_Hud2->LightOff();
	NeroUI_Hud3 = CreateComponent<UIFBXRenderer>(2);
	NeroUI_Hud3->SetFBXMesh("NeroEx03.FBX", "FBX_Low");
	NeroUI_Hud3->GetTransform()->SetLocalPosition({ -703.0f,365.0f,0.0f });
	NeroUI_Hud3->GetTransform()->SetLocalRotation({ 30.0f,-90.0f,-30.0f });
	NeroUI_Hud3->GetTransform()->SetLocalScale({ 0.7f,0.7f,0.7f });
	NeroUI_Hud3->LightOff();

	std::shared_ptr<GameEngineUIRenderer> NeroUI_Back = CreateComponent<GameEngineUIRenderer>(0);
	NeroUI_Back->SetTexture("HudBack.png");
	NeroUI_Back->SetTexture("HudBack.png");
	NeroUI_Back->GetTransform()->SetLocalScale({ 97.0f,131.0f,0.0f });
	NeroUI_Back->GetTransform()->SetLocalPosition({ -696.0f,334.0f,40.0f });

	std::shared_ptr<UI_HPGaege> UI_HPGaegeBar = GetLevel()->CreateActor<UI_HPGaege>();
	std::shared_ptr<UI_DTGauge> UI_DTGaugeBar = GetLevel()->CreateActor<UI_DTGauge>();
	std::shared_ptr<UI_BulletGauge> UI_BulletGaugeBar = GetLevel()->CreateActor<UI_BulletGauge>();

	
}

void NeroHPUI::Update(float _DeltaTime)
{

}

