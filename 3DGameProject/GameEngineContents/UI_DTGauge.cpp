#include "PrecompileHeader.h"
#include "UI_DTGauge.h"
#include "UIEffectRenderer.h"
#include "UIFBXRenderer.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
UI_DTGauge::UI_DTGauge() 
{
}

UI_DTGauge::~UI_DTGauge() 
{
}

void UI_DTGauge::Start()
{
	DTElectroEffect = CreateComponent<UIEffectRenderer>(3);
	DTElectroEffect->RectInit("Effect_2D");
	DTElectroEffect->CreateAnimation({ .AnimationName = "DTELECT", .SpriteName = "Effect_Electric_04.tga", .Start = 0, .End = 3,.FrameInter = 0.07f,.Loop = true });
	DTElectroEffect->GetTransform()->SetLocalScale({ 42.0f,138.0f,3.0f });
	DTElectroEffect->GetTransform()->SetLocalPosition({ -523.0f,376.0f,-150.0f });
	DTElectroEffect->GetTransform()->SetLocalRotation({ 0.0f,0.0f,-90.0f });
	DTElectroEffect->EffectOption.PlusColor = { 0.0f,0.0f,0.0f,-0.2f };
	DTElectroEffect->ChangeAnimation("DTELECT");
	DTElectroEffect->Off();


	DTGauge = CreateComponent< GameEngineUIRenderer>();
	DTGauge->SetTexture("DTGauge.png");
	DTGauge->GetTransform()->SetLocalScale({ 171.0f,34.8f,0.0f });
	DTGauge->GetTransform()->SetLocalPosition({ -525.2f,372.8f,40.0f });

	for (int i = 0; i < 3; i++)
	{
		DTGauges.push_back(UIFBXActorBase::CreateGaugeBar({ -590.0f + i * 16.0f , 372.0f , 0.0f }, { 0.9f, 0.9f, 0.9f }, { -123.0f,0.0f,0.0f }, "DtGauge.FBX"));
	}
	for (int i = 0; i < 7; i++)
	{
		DTGauges.push_back(UIFBXActorBase::CreateGaugeBar({ -541.0f + i * 13.0f , 375.0f , 0.0f }, { 0.7f, 0.7f, 0.7f }, { -123.0f,0.0f,0.0f }, "DtGauge.FBX"));
	}
}

void UI_DTGauge::Update(float _DeltaTime)
{
}

