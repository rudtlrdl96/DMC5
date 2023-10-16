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


	for (int i = 0; i < 3; i++)
	{
		DTGauges.push_back(UIFBXActorBase::CreateGaugeBar({ -590.0f + i * 16.0f , 372.0f , 30.0f + i *1.0f }, { 0.9f, 0.9f, 0.9f }, { -90.0f,0.0f,0.0f }, "DtGauge.FBX"));
		CreateDTBar("DTLargeGauge.png", { -590.0f + i * 16.0f , 372.0f , 40.0f }, {34.0f,28.0f,0.0f});
		DTGauges[i]->SetMulColor({ 0.0f,0.0f,0.0f,1.0f });
	}
	for (int i = 0; i < 7; i++)
	{
		DTGauges.push_back(UIFBXActorBase::CreateGaugeBar({ -541.0f + i * 13.0f , 375.0f , 30.0f + i * 1.0f }, { 0.7f, 0.7f, 0.7f }, { -90.0f,0.0f,0.0f }, "DtGauge.FBX"));
		CreateDTBar("DTSmallGauge.png", { -541.0f + i * 13.0f , 375.0f , 40.0f }, { 29.0f,28.0f,0.0f });
		DTGauges[i+3]->SetMulColor({ 0.0f,0.0f,0.0f,1.0f });
	}
}

void UI_DTGauge::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("UI_Tab"))
	{
		DTIndex++;
	}
	ActivateDT();
}

void UI_DTGauge::ActivateDT()
{
	if (DTIndex > -1 && DTIndex < 10)
	{
		DTGaugeBars[DTIndex]->On();
		DTGauges[DTIndex]->SetMulColor({ 0.59f,0.588f,0.2925f,1.0f });
		DTGauges[DTIndex]->SetAddColor({ 0.29f,0.088f,0.0925f,0.0f });
	}
	if (DTIndex == 9)
	{
		DTElectroEffect->On();
	}
}

void UI_DTGauge::CreateDTBar(const std::string_view& _Png,  float4 _Pos, float4 _Scale)
{

	DTGauge = CreateComponent< GameEngineUIRenderer>();
	DTGauge->SetTexture(_Png);
	DTGauge->GetTransform()->SetLocalPosition(_Pos);
	DTGauge->GetTransform()->SetLocalScale(_Scale);
	DTGauge->Off();
	DTGaugeBars.push_back(DTGauge);
	
}

//{ 171.0f,34.8f,0.0f }