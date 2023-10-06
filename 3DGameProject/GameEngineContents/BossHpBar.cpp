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

	//BossFrontGaege->SetMulColor(float4(0.27f, 0.27f, 0.8f, 1.0f));
	//BossFrontGaege->SetClipData(float4(0.0f, 1.0f, 0.0f, 1.0f));

	BossHpAni = CreateComponent<GameEngineUIRenderer>(4);
	BossHpAni->SetTexture("HPGaugeAni.png");
	BossHpAni->CreateAnimation({ .AnimationName = "BossHPANI", .SpriteName = "HPGaugeAni.png", .Start = 5, .End = 7, .Loop = true, .FrameTime = {.15f, .15f,.15f, .15f,.15f, .15f,.15f, .15f} });
	BossHpAni->GetTransform()->SetLocalScale({ 46.0f,170.0f,0.0f });
	BossHpAni->GetTransform()->SetLocalPosition({ 350.0f,3.0f,0.0f });
	BossHpAni->GetTransform()->SetLocalRotation({ 0.0f,0.0f,90.0f });
	BossHpAni->ChangeAnimation("BossHPANI");
	
	BossHPUI_Front = CreateComponent<GameEngineUIRenderer>(1);
	BossHPUI_Front->SetTexture("BossGaugeBase.png");
	BossHPUI_Front->GetTransform()->SetLocalScale({ 720.0f,720.0f,0.0f });
	BossHPUI_Front->GetTransform()->SetLocalPosition({ 0.0f,0.8f,0.0f });
	BossHPUI_Front = CreateComponent<GameEngineUIRenderer>(2);
	BossHPUI_Back = CreateComponent<GameEngineUIRenderer>(0);
	BossHPUI_Back->SetTexture("BossHitGauge.png");
	BossHPUI_Back->GetTransform()->SetLocalScale({701.0f,31.0f,0.0f });
	BossHPUI_Back->GetTransform()->SetLocalPosition({ 0.0f,0.8f,0.0f });
	BossFrontGaege = UIFBXActorBase::CreateGaugeBar({ 0.0f , 0.3f , 0.0f , 0.0f }, { 7.4f,9.7f,5.0f }, { 0.0f,0.0f,0.0f }, "BossHPBar.FBX", "FBX_ClipAlpha");

}

void BossHpBar::Update(float _Delta)
{
}

