#include "PrecompileHeader.h"
#include "PlayerHPUI.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include "UIFBXRenderer.h"
#include "UI_HPGaege.h"
#include "UI_DTGauge.h"
#include "UI_BulletGauge.h"
#include "UIEffectRenderer.h"
#include "PlayerHPUI.h"
int PlayerHPUI::RedOrbValue = 0;;
PlayerHPUI::PlayerHPUI() 
{
}

PlayerHPUI::~PlayerHPUI() 
{
}
void PlayerHPUI::SetPlayerHP(int _HP)
{
	UI_HPGaegeBar->UpdateHPBar(_HP);
}
void PlayerHPUI::ShootBullet()
{
	UI_BulletGaugeBar->SetBullet();
}
bool PlayerHPUI::IsChargeBullet()
{
	if (UI_BulletGaugeBar->GetBullet() >= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void PlayerHPUI::SetVergilUI()
{
	IsNero = false;
}
void PlayerHPUI::SetNeroUI()
{

	NeroUI_Hud1 = UIFBXActorBase::CreateNoneLightFBX(NeroUI_Hud1, { -711.0f,303.0f,20.0f }, { 0.7f,0.7f,0.7f }, { 30.0f,-90.0f,-30.0f }, "NeroEx01.FBX");
	NeroUI_Hud2 = UIFBXActorBase::CreateNoneLightFBX(NeroUI_Hud2, { -708.0f,334.0f,0.0f }, { 0.7f,0.7f,0.7f }, { 30.0f,-90.0f,-30.0f }, "NeroEx02.FBX");
	NeroUI_Hud3 = UIFBXActorBase::CreateNoneLightFBX(NeroUI_Hud3, { -703.0f,365.0f,0.0f }, { 0.7f,0.7f,0.7f }, { 30.0f,-90.0f,-30.0f }, "NeroEx03.FBX");

	std::shared_ptr<GameEngineUIRenderer> NeroUI_Back = CreateComponent<GameEngineUIRenderer>(0);
	NeroUI_Back->SetTexture("HudBack.png");
	NeroUI_Back->GetTransform()->SetLocalScale({ 97.0f,131.0f,0.0f });
	NeroUI_Back->GetTransform()->SetLocalPosition({ -696.0f,334.0f,40.0f });
	UI_BulletGaugeBar = GetLevel()->CreateActor<UI_BulletGauge>();
	UI_BulletGaugeBar->GetTransform()->SetParent(GetTransform());
	CreatFireEffect();
	IsNero = true;
}
/*
후드,후드불
HP바 1줄
변신게이지
총알
*/
void PlayerHPUI::Start()
{
	float Ratio = GameEngineWindow::GetScreenSize().y / 900;
	GetTransform()->SetLocalScale(float4::ONE * Ratio);

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
		GameEngineTexture::Load(NewHPDir.GetPlusFileName("RedOrb.png").GetFullPath());


	}
//	NeroUI_HPGlass= UIFBXActorBase::CreateUIFBX(NeroUI_HPGlass, { -535.0f,273.0f,172.0f }, { 0.6f,0.6f,0.6f }, { -90.0f,0.0f,0.0f }, "Nero_HPGlass.FBX","FBX_ALPHA");


	RedOrbRender = CreateComponent<GameEngineUIRenderer>(0);
	RedOrbRender->SetTexture("RedOrb.png");
	RedOrbRender->GetTransform()->SetLocalScale({ 43.0f,60.0f,0.0f });
	RedOrbRender->GetTransform()->SetLocalPosition({ 550.0f,380.0f,40.0f });

	RedOrbFont = CreateComponent<GameEngineFontRenderer>();
	RedOrbFont->SetFont("DMC5Font");
	RedOrbFont->SetScale(50.0f);
	RedOrbFont->SetColor(float4::WHITE);
	RedOrbFont->SetFontFlag(FW1_TEXT_FLAG::FW1_LEFT);
	RedOrbFont->GetTransform()->SetLocalPosition({ 582.0f,407.0f,40.0f });

	UI_HPGaegeBar = GetLevel()->CreateActor<UI_HPGaege>();
	UI_HPGaegeBar->GetTransform()->SetParent(GetTransform());
	UI_DTGaugeBar = GetLevel()->CreateActor<UI_DTGauge>();
	UI_DTGaugeBar->GetTransform()->SetParent(GetTransform());


}

void PlayerHPUI::Update(float _DeltaTime)
{
	if (IsNero == true)
	{
		SetExceed();
	}
	RedOrbFont->SetText(std::to_string(RedOrbValue));
}

void PlayerHPUI::CreatFireEffect()
{
	ExFire_Effect1 = CreateComponent<GameEngineUIRenderer>(3);
	ExFire_Effect1->SetTexture("NullTexture.png");
	ExFire_Effect1->CreateAnimation({ .AnimationName = "HudAni_1", .SpriteName = "Effect_Fire_07.tga", .Start = 0, .End = 63,.FrameInter = 0.001f, .Loop = true });
	ExFire_Effect1->GetTransform()->SetLocalScale({ 76.0f,100.0f,.0f });
	ExFire_Effect1->GetTransform()->SetLocalPosition({ -656.0f,335.0f,12.0f });
	ExFire_Effect1->GetTransform()->SetLocalRotation({ 0.0f,0.0f,-50.0f });
	ExFire_Effect1->ChangeAnimation("HudAni_1");
	ExFire_Effect1->Off();
	
	ExFire_Effect2 = CreateComponent<GameEngineUIRenderer>(3);
	ExFire_Effect2->SetTexture("NullTexture.png");
	ExFire_Effect2->CreateAnimation({ .AnimationName = "HudAni_2", .SpriteName = "Effect_Fire_07.tga", .Start = 0, .End = 63,.FrameInter = 0.001f, .Loop = true});
	ExFire_Effect2->GetTransform()->SetLocalScale({ 76.0f,100.0f,.0f });
	ExFire_Effect2->GetTransform()->SetLocalPosition({ -648.0f,372.0f,9.0f });
	ExFire_Effect2->GetTransform()->SetLocalRotation({ 0.0f,0.0f,-50.0f });
	ExFire_Effect2->ChangeAnimation("HudAni_2");
	ExFire_Effect2->Off();

	ExFire_Effect3 = CreateComponent<GameEngineUIRenderer>(3);
	ExFire_Effect3->SetTexture("NullTexture.png");
	ExFire_Effect3->CreateAnimation({ .AnimationName = "HudAni_3", .SpriteName = "Effect_Fire_07.tga", .Start = 0, .End = 63,.FrameInter = 0.001f, .Loop = true});
	ExFire_Effect3->GetTransform()->SetLocalScale({ 76.0f,100.0f,.0f });
	ExFire_Effect3->GetTransform()->SetLocalPosition({ -645.0f,408.0f,7.0f });
	ExFire_Effect3->GetTransform()->SetLocalRotation({ 0.0f,0.0f,-50.0f });
	ExFire_Effect3->ChangeAnimation("HudAni_3");
	ExFire_Effect3->Off();
}

void PlayerHPUI::SetExceed()
{
	if (ExceedCount == 0)
	{
		ExFire_Effect1->Off();
		ExFire_Effect2->Off();
		ExFire_Effect3->Off();

	}
	else if (ExceedCount == 1)
	{
		ExFire_Effect1->On();
		ExFire_Effect2->Off();
		ExFire_Effect3->Off();
	}
	else if (ExceedCount == 2)
	{
		ExFire_Effect1->On();
		ExFire_Effect2->On();
		ExFire_Effect3->Off();
	}
	else if (ExceedCount == 3)
	{
		ExFire_Effect1->On();
		ExFire_Effect2->On();
		ExFire_Effect3->On();
	}
}

