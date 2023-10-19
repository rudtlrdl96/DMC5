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
	DTElectroEffect->CreateAnimation({ .AnimationName = "DTELECT", .SpriteName = "Effect_Electric_04.tga", .Start = 0, .End = 3,.FrameInter = 0.036f,.Loop = true });
	DTElectroEffect->GetTransform()->SetLocalScale({ 50.0f,138.0f,3.0f });
	DTElectroEffect->GetTransform()->SetLocalPosition({ -523.0f,376.0f,-150.0f });
	DTElectroEffect->GetTransform()->SetLocalRotation({ 0.0f,0.0f,-90.0f });
	DTElectroEffect->EffectOption.PlusColor = { 1.0f,1.0f,1.0f,-0.3f };
	DTElectroEffect->ChangeAnimation("DTELECT");
	DTElectroEffect->Off();

	DTElectroEffect_Down = CreateComponent<UIEffectRenderer>(3);
	DTElectroEffect_Down->RectInit("Effect_2D");
	DTElectroEffect_Down->CreateAnimation({ .AnimationName = "DTELECT", .SpriteName = "Effect_Electric_04.tga", .Start = 0, .End = 3,.FrameInter = 0.046f,.Loop = true });
	DTElectroEffect_Down->GetTransform()->SetLocalScale({ 50.0f,138.0f,3.0f });
	DTElectroEffect_Down->GetTransform()->SetLocalPosition({ -523.0f,376.0f,-150.0f });
	DTElectroEffect_Down->GetTransform()->SetLocalRotation({ 0.0f,180.0f,-90.0f });
	DTElectroEffect_Down->EffectOption.PlusColor = { 1.0f,1.0f,1.0f,-0.3f };
	DTElectroEffect_Down->ChangeAnimation("DTELECT");
	DTElectroEffect_Down->Off();
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
	Transfor(_DeltaTime);

}

void UI_DTGauge::ActivateDtUI(float _DtGauge)
{
	// 0 ~ 10
	//ActiveCount = static_cast<int>(_DtGauge);	// 소수점 버리고
	ActiveCount = static_cast<int>(std::ceil(_DtGauge));	// 올림

	for (int i = 0; i < ActiveCount; i++)
	{
		DTGauges[i]->SetMulColor({ 0.59f,0.588f,0.2925f,1.0f });
		DTGauges[i]->SetAddColor({ 0.29f,0.088f,0.0925f,0.0f });
		DTGaugeBars[i]->On();
		DTGaugeBars[i]->ColorOptionValue.MulColor = { 1.0f, 1.0f, 1.0f, std::clamp(_DtGauge - static_cast<float>(i), 0.0f, 1.0f) };
	}

	for (int i = ActiveCount; i < 10; i++)
	{
		DTGauges[i]->SetMulColor(float4::ZERO);
		DTGauges[i]->SetAddColor(float4::ZERONULL);

		DTGaugeBars[i]->Off();
	}

	//DTGauges[ActiveCount]->SetMulColor({ 0.59f,0.588f,0.2925f,1.0f });
	//DTGauges[ActiveCount]->SetAddColor({ 0.29f,0.088f,0.0925f,0.0f });
	//DTGaugeBars[ActiveCount]->On();
	//DTGaugeBars[ActiveCount]->ColorOptionValue.MulColor = { 1.0f, 1.0f, 1.0f,_DtGauge - static_cast<float>(ActiveCount)};

	//for (int i = 0; i < ActiveCount; i++)
	//{
	//	DTGaugeBars[ActiveCount]->On();
	//	DTGaugeBars[ActiveCount]->ColorOptionValue.MulColor = float4::ONE;
	//}
}

//void UI_DTGauge::Transfor(float _Delta)
//{
//	if (IsTrans == true)
//	{
//		TransforTime += _Delta;
//		DownTime += _Delta;
//		DTElectroEffect->GetTransform()->SetLocalScale(float4::LerpClamp({ 50.0f,138.0f,3.0f }, { 50.0f,0.0f,3.0f }, TransforTime/27.0f));
//		DTElectroEffect_Down->GetTransform()->SetLocalScale(float4::LerpClamp({ 50.0f,138.0f,3.0f }, { 50.0f,0.0f,3.0f }, TransforTime / 27.0f));
//		DTElectroEffect->GetTransform()->SetLocalPosition(float4::LerpClamp({ -530.0f,376.0f,-150.0f }, { -591.0f,376.0f,-150.0f }, TransforTime / 27.0f));
//		DTElectroEffect_Down->GetTransform()->SetLocalPosition(float4::LerpClamp({ -530.0f,376.0f,-150.0f }, { -591.0f,376.0f,-150.0f }, TransforTime / 27.0f));
//		if (DownTime >= 2.7f)
//		{
//			if (DTIndex >= 0)
//			{
//				DTGaugeBars[DTIndex]->Off();
//				DTGauges[DTIndex]->SetMulColor({ 0.0f,0.0f,0.0f,1.0f });
//				DTGauges[DTIndex]->SetAddColor({ 0.0f,0.0f,0.0f,0.0f });	
//				DTIndex--;
//				DownTime = 0.0f;
//			}
//			else
//			{
//				IsTrans = false;
//			}
//
//		}
//	}
//
//}

void UI_DTGauge::Transfor(float _Delta)
{
}

void UI_DTGauge::CreateDTBar(const std::string_view& _Png,  float4 _Pos, float4 _Scale)
{

	DTGauge = CreateComponent< GameEngineUIRenderer>();
	DTGauge->SetTexture(_Png);
	DTGauge->GetTransform()->SetLocalPosition(_Pos);
	DTGauge->GetTransform()->SetLocalScale(_Scale);
	DTGauge->Off();
	DTGauge->ColorOptionValue.MulColor = float4::ZERONULL;
	DTGaugeBars.push_back(DTGauge);
	
}

//{ 171.0f,34.8f,0.0f }