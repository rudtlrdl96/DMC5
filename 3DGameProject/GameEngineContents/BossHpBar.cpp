#include "PrecompileHeader.h"
#include "BossHpBar.h"
#include "UIFBXRenderer.h"
#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include "UIFBXActorBase.h"
BossHpBar::BossHpBar() 
{
}

BossHpBar::~BossHpBar() 
{
}

void BossHpBar::Start()
{

	BossFrontGaege = UIFBXActorBase::CreateGaugeBar({ 0.0f , 0.3f , 0.0f , 0.0f }, { 7.4f,9.7f,0.6f }, { 0.0f,0.0f,0.0f }, "BossHPBar.FBX", "FBX_Alpha");
	BossFrontGaege->SetMulColor(float4(0.27f, 0.27f, 0.9f, 0.8f));

	BossHpAni = CreateComponent<GameEngineUIRenderer>(4);
	BossHpAni->SetTexture("HPGaugeAni.png");
	BossHpAni->CreateAnimation({ .AnimationName = "BossHPANI", .SpriteName = "HPGaugeAni.png", .Start = 3, .End = 5, .Loop = true, .FrameTime = {.2f, .2f,.2f, .2f,.2f, .2f,.2f, .2f} });
	BossHpAni->GetTransform()->SetLocalScale({ 46.0f,170.0f,0.0f });
	BossHpAni->GetTransform()->SetLocalPosition({ 350.0f,3.0f,0.0f });
	BossHpAni->GetTransform()->SetLocalRotation({ 0.0f,0.0f,90.0f });
	BossHpAni->ChangeAnimation("BossHPANI");
	
	BossHPUI_Front = CreateComponent<GameEngineUIRenderer>(1);
	BossHPUI_Front->SetTexture("BossGaugeBase.png");
	BossHPUI_Front->GetTransform()->SetLocalScale({ 720.0f,720.0f,0.0f });
	BossHPUI_Front->GetTransform()->SetLocalPosition({ 0.0f,0.8f,0.0f });
	BossHPUI_Front = CreateComponent<GameEngineUIRenderer>(2);
	//BossHPUI_Back = CreateComponent<GameEngineUIRenderer>(3);
	//BossHPUI_Back->SetTexture("hpHitbar.png");
	//BossHPUI_Back->GetTransform()->SetLocalScale({ 492.0f,8.0f,0.0f });
	//BossHPUI_Back->GetTransform()->SetLocalPosition({ 0.0f,0.8f,0.0f });
}

void BossHpBar::Update(float _Delta)
{
}

