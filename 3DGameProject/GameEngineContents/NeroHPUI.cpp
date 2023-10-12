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
void NeroHPUI::SetPlayerHP(int _HP)
{
	UI_HPGaegeBar->UpdateHPBar(_HP);
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
	NewDir.Move("NeroHPMesh");

	if (nullptr == GameEngineFBXMesh::Find("DtGauge.FBX"))
	{
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
		for (GameEngineFile File : Files)
		{
			GameEngineFBXMesh::Load(File.GetFullPath());
		}
	}
	GameEngineDirectory NewHPDir;
	NewHPDir.MoveParentToDirectory("ContentResources");
	NewHPDir.Move("ContentResources");
	NewHPDir.Move("Texture");
	NewHPDir.Move("UI");
	NewHPDir.Move("PlayLevelUI");
	if (nullptr == GameEngineSprite::Find("HPGaugeAni.png"))
	{
		GameEngineSprite::LoadSheet(NewHPDir.GetPlusFileName("HPGaugeAni.png").GetFullPath(), 4, 4);
		GameEngineTexture::Load(NewHPDir.GetPlusFileName("BossGaugeBase.png").GetFullPath());
		GameEngineTexture::Load(NewHPDir.GetPlusFileName("BossHitGauge.png").GetFullPath());
		GameEngineTexture::Load(NewHPDir.GetPlusFileName("123123123.png").GetFullPath());


	}
//	NeroUI_HPGlass= UIFBXActorBase::CreateUIFBX(NeroUI_HPGlass, { -535.0f,273.0f,172.0f }, { 0.6f,0.6f,0.6f }, { -90.0f,0.0f,0.0f }, "Nero_HPGlass.FBX","FBX_ALPHA");
	NeroUI_Hud1 = UIFBXActorBase::CreateUIFBX(NeroUI_Hud1, { -711.0f,303.0f,20.0f }, { 0.7f,0.7f,0.7f }, { 30.0f,-90.0f,-30.0f }, "NeroEx01.FBX");
	NeroUI_Hud2 = UIFBXActorBase::CreateUIFBX(NeroUI_Hud2, { -708.0f,334.0f,0.0f }, { 0.7f,0.7f,0.7f }, { 30.0f,-90.0f,-30.0f }, "NeroEx02.FBX");
	NeroUI_Hud3 = UIFBXActorBase::CreateUIFBX(NeroUI_Hud3, { -703.0f,365.0f,0.0f }, { 0.7f,0.7f,0.7f }, { 30.0f,-90.0f,-30.0f }, "NeroEx03.FBX");

	std::shared_ptr<GameEngineUIRenderer> NeroUI_Back = CreateComponent<GameEngineUIRenderer>(0);
	NeroUI_Back->SetTexture("HudBack.png");
	NeroUI_Back->GetTransform()->SetLocalScale({ 97.0f,131.0f,0.0f });
	NeroUI_Back->GetTransform()->SetLocalPosition({ -696.0f,334.0f,40.0f });

	UI_HPGaegeBar = GetLevel()->CreateActor<UI_HPGaege>();
	UI_DTGaugeBar = GetLevel()->CreateActor<UI_DTGauge>();
	UI_BulletGaugeBar = GetLevel()->CreateActor<UI_BulletGauge>();
	UI_BulletGaugeBar->GetTransform()->SetParent(GetTransform());

	
}

void NeroHPUI::Update(float _DeltaTime)
{
	PlayerCurHp;
	int a = 0;
}

