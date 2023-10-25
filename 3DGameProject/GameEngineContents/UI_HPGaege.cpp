#include "PrecompileHeader.h"
#include "UI_HPGaege.h"
#include "BasePlayerActor.h"
#include "UIFBXRenderer.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineSpriteRenderer.h>
UI_HPGaege::UI_HPGaege() 
{
}

UI_HPGaege::~UI_HPGaege() 
{
}



void UI_HPGaege::Start()
{
	//GameEngineDirectory NewDir;
	//NewDir.MoveParentToDirectory("ContentResources");
	//NewDir.Move("ContentResources");
	//NewDir.Move("Texture");
	//NewDir.Move("UI");
	//NewDir.Move("PlayLevelUI");
	//if (nullptr == GameEngineSprite::Find("HPGaugeAni.png"))
	//{
	//	GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("HPGaugeAni.png").GetFullPath(), 4, 4);
	//}
	float Ratio = GameEngineWindow::GetScreenSize().y / 900;
	GetTransform()->SetLocalScale(float4::ONE * Ratio);

	NeroHpAni = CreateComponent<GameEngineUIRenderer>(3);
	NeroHpAni->SetTexture("HPGaugeAni.png");
	NeroHpAni->CreateAnimation({ .AnimationName = "HPANI", .SpriteName = "HPGaugeAni.png", .Start = 1, .End = 5, .Loop = true, .FrameTime = {.1f, .1f,.1f, .1f,.1f, .1f,.1f, .1f} });
	NeroHpAni->GetTransform()->SetLocalScale({ 30.0f,170.0f,0.0f });
	NeroHpAni->GetTransform()->SetLocalPosition({ -104.0f,404.0f,0.0f });
	NeroHpAni->GetTransform()->SetLocalRotation({ 0.0f,0.0f,90.0f });
	NeroHpAni->ChangeAnimation("HPANI");

	PlayerHPUI_Front = CreateComponent<GameEngineUIRenderer>(2);
	PlayerHPUI_Front->SetTexture("hpbar.png");
	PlayerHPUI_Front->GetTransform()->SetLocalScale({ 492.0f,8.0f,0.0f });
	PlayerHPUI_Front->GetTransform()->SetLocalPosition({ -350.0f,404.8f,0.0f });
	PlayerHPUI_Back = CreateComponent<GameEngineUIRenderer>(1);
	PlayerHPUI_Back->SetTexture("hpHitbar.png");
	PlayerHPUI_Back->GetTransform()->SetLocalScale({ 492.0f,8.0f,0.0f });
	PlayerHPUI_Back->GetTransform()->SetLocalPosition({ -350.0f,404.8f,0.0f });
	for (int i = 0; i < 10; i++)
	{
		UIFBXActorBase::CreateGaugeBar({ -570.0f + i * 49.0f , 405.0f , -15.0f },{ 0.7f,0.6f,0.6f }, { 90.0f,0.0f,0.0f }, "HpGauge.FBX");
		UIFBXActorBase::CreateGaugeBar({ -570.0f + i * 49.0f , 405.0f , -15.0f }, { 0.7f,0.6f,0.6f }, { 90.0f,0.0f,0.0f }, "HpGaugeBase.FBX" , "UIFBXMesh");

	}
}

void UI_HPGaege::Update(float _DeltaTime)
{
	ClipHPBar(_DeltaTime);
}

void UI_HPGaege::ClipHPBar(float _Delta)
{
	PlayerHPUI_Front->ImageClippingX(static_cast<float>(CurHp) / static_cast<float>(MaxHP), ClipXDir::Left);
	float Cal = static_cast<float>(CurHp) / static_cast<float>(MaxHP);
	float XPos = -104.0f - 492.0f * (1.0f - Cal);
	NeroHpAni->GetTransform()->SetLocalPosition({ XPos,HPAniPos.y,0.0f });
	if (CurHp != RedHp)
	{
		Ratio += _Delta;
		RedHp = static_cast<int>(GameEngineMath::LerpLimit(static_cast<float>(PrevHp), static_cast<float>(CurHp), _Delta * Ratio * 30.0f));
		PlayerHPUI_Back->ImageClippingX(static_cast<float>(RedHp) / static_cast<float>(MaxHP), ClipXDir::Left);
	}
	else
	{
		PrevHp = CurHp;
		Ratio = 0.0f;
	}
}

