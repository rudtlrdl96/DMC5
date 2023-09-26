#include "PrecompileHeader.h"
#include "UI_HPGaege.h"
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
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Texture");
	NewDir.Move("UI");
	NewDir.Move("PlayLevelUI");
	if (nullptr == GameEngineSprite::Find("HPGaugeAni.png"))
	{
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("HPGaugeAni.png").GetFullPath(), 4, 4);
	}

	std::shared_ptr<GameEngineSpriteRenderer> NeroHpAni = CreateComponent<GameEngineSpriteRenderer>(0);
	NeroHpAni->SetTexture("HPGaugeAni.png");
	NeroHpAni->CreateAnimation({ .AnimationName = "HPANI", .SpriteName = "HPGaugeAni.png", .Start = 0, .End = 7, .Loop = true, .FrameTime = {.1f, .1f,.1f, .1f,.1f, .1f,.1f, .1f} });
	NeroHpAni->GetTransform()->SetLocalScale({ 50.0f,170.0f,0.0f });
	NeroHpAni->GetTransform()->SetLocalPosition({ -240.0f,940.0f,50.0f });
	NeroHpAni->GetTransform()->SetLocalRotation({ 0.0f,0.0f,90.0f });
	NeroHpAni->ChangeAnimation("HPANI");

	std::shared_ptr<GameEngineUIRenderer> NeroHPUI_Front = CreateComponent<GameEngineUIRenderer>(0);
	NeroHPUI_Front->SetTexture("hpbar.png");
	NeroHPUI_Front->GetTransform()->SetLocalScale({ 492.0f,7.0f,0.0f });
	NeroHPUI_Front->GetTransform()->SetLocalPosition({ -350.0f,404.3f,40.0f });
	std::shared_ptr<GameEngineUIRenderer> NeroHPUI_Back = CreateComponent<GameEngineUIRenderer>(0);
	NeroHPUI_Back->SetTexture("hpHitbar.png");
	NeroHPUI_Back->GetTransform()->SetLocalScale({ 492.0f,7.0f,0.0f });
	NeroHPUI_Back->GetTransform()->SetLocalPosition({ -350.0f,404.3f,50.0f });
	for (int i = 0; i < 10; i++)
	{
		UIFBXActorBase::CreateGaugeBar({ -570.0f + i * 49.0f , 405.0f , 0.0f },{ 0.7f,0.6f,0.6f }, { 90.0f,0.0f,0.0f }, "HpGauge.FBX");
		UIFBXActorBase::CreateGaugeBar({ -570.0f + i * 49.0f , 405.0f , 0.0f }, { 0.7f,0.6f,0.6f }, { 90.0f,0.0f,0.0f }, "HpGaugeBase.FBX" , "FBX_Alpha");

	}
}

void UI_HPGaege::Update(float _DeltaTime)
{

}

