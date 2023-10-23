#include "PrecompileHeader.h"
#include "Char_ChoiceButton.h"
#include "UIEnums.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

Char_ChoiceButton::Char_ChoiceButton()
{
}

Char_ChoiceButton::~Char_ChoiceButton()
{
}

void Char_ChoiceButton::Start()
{
	WindowRatio = GameEngineActor::ScreenRatio;
	NoneSelectRender = CreateComponent<GameEngineUIRenderer>(1);
	NoneSelectRender->SetTexture("Char_NoneSelect.png");
	SelectRender = CreateComponent<GameEngineUIRenderer>(2);
	SelectRender->SetTexture("Char_Select.png");
	CharRender = CreateComponent<GameEngineUIRenderer>(3);
	TextNoneRender = CreateComponent<GameEngineUIRenderer>(5);
	TextNoneRender->SetScaleToTexture("Char_NoneText.png");
	TextNoneRender->ColorOptionValue.MulColor.a = 0.8f;
	TextBarRender = CreateComponent<GameEngineUIRenderer>(7);
	TextBarRender->SetScaleToTexture("Char_Bar.png");
	TextBarRender->GetTransform()->SetLocalPosition(float4{0.0f,-135.0f,0.0f} *WindowRatio);
	TextBarRender->ColorOptionValue.MulColor.a = 0.9f;
	TextSelectRender = CreateComponent<GameEngineUIRenderer>(6);
	TextSelectRender->SetTexture("Char_SelectText.png");
	TextSelectRender->GetTransform()->SetLocalScale(SelcetBarScale* WindowRatio);
	TextSelectRender->ImageClippingX(0, ClipXDir::Left);
	TextSelectRender->ColorOptionValue.MulColor.a = 0.9f;
	FontCreate();
}

void Char_ChoiceButton::Update(float _Delta)
{
	GameEngineCamera* Camera = NoneSelectRender->GetCamera();

	// ·£´õ·¯ 
	float4x4 ViewPort = Camera->GetViewPort();
	float4x4 Proj = Camera->GetProjection();
	float4x4 View = Camera->GetView();

	float4 Mouse = GameEngineInput::GetMousePosition();

	Mouse *= ViewPort.InverseReturn();
	Mouse *= Proj.InverseReturn();
	Mouse *= View.InverseReturn();


	CollisionData MouseData;
	MouseData.SPHERE.Center = Mouse.DirectFloat3;
	MouseData.SPHERE.Radius = 0.0f;
	if (true == GameEngineTransform::AABB2DToSpehre2D(NoneSelectRender->GetTransform()->GetCollisionData(), MouseData))
	{
		if (SwichValue == false)
		{
			IsValue = true;
			SwichValue = true;
		}
	}
	else
	{
		IsValue = false;
		SwichValue = false;
	}
	if (IsSelect == true)
	{
		ScaleDownTime = 0.0f;
		ScaleUpTime += _Delta;
		SelectRender->On();
		SelectRender->GetTransform()->SetLocalScale(SizeUpCalCulation(Scale * WindowRatio, SeletScale * WindowRatio, ScaleUpTime * 7.0f));
		NoneSelectRender->Off();
		NoneSelectRender->GetTransform()->SetLocalScale(Scale * WindowRatio);
		CharRender->GetTransform()->SetLocalScale(SizeUpCalCulation(Char_Scale * WindowRatio, Char_SeletScale * WindowRatio, ScaleUpTime * 7.0f));
		TextNoneRender->GetTransform()->SetLocalScale(SizeUpCalCulation(Char_TextScale * WindowRatio, Char_TextSeletScale * WindowRatio, ScaleUpTime * 7.0f));
		TextBarRender->GetTransform()->SetLocalScale(SizeUpCalCulation(Char_BarScale * WindowRatio, Char_BarScaleUp * WindowRatio, ScaleUpTime * 7.0f));
		TextSelectRender->ImageClippingX(ScaleUpTime*10.0f, ClipXDir::Left);
		NameText->SetScale(GameEngineMath::LerpLimit(50.0f * WindowRatio.x,55.0f * WindowRatio.x, ScaleUpTime * 7.0f));
		ExplaneText->SetScale(GameEngineMath::LerpLimit(20.0f * WindowRatio.x, 22.0f * WindowRatio.x, ScaleUpTime * 7.0f));
		SetSaturation();
		if (true == GameEngineInput::IsUp("UI_Enter"))
		{
			if (nullptr != Click)
			{
				Click();
			}
		}
	}
	else
	{
		ScaleUpTime = 0.0f;
		ScaleDownTime += _Delta;
		SelectRender->Off();
		SelectRender->GetTransform()->SetLocalScale(Scale * WindowRatio);
		NoneSelectRender->GetTransform()->SetLocalScale(SizeUpCalCulation(SeletScale * WindowRatio, Scale * WindowRatio, ScaleDownTime * 7.0f));
		NoneSelectRender->On();
		CharRender->GetTransform()->SetLocalScale(SizeUpCalCulation(Char_SeletScale * WindowRatio, Char_Scale * WindowRatio, ScaleDownTime * 7.0f));
		TextNoneRender->GetTransform()->SetLocalScale(SizeUpCalCulation(Char_TextSeletScale * WindowRatio, Char_TextScale * WindowRatio, ScaleDownTime * 7.0f));
		TextBarRender->GetTransform()->SetLocalScale(SizeUpCalCulation(Char_BarScaleUp * WindowRatio, Char_BarScale * WindowRatio, ScaleDownTime * 7.0f));
		TextSelectRender->ImageClippingX(1- ScaleDownTime*10.0f, ClipXDir::Left);
		NameText->SetScale(GameEngineMath::LerpLimit(55.0f * WindowRatio.x, 50.0f * WindowRatio.x, ScaleDownTime * 7.0f));
		ExplaneText->SetScale(GameEngineMath::LerpLimit(22.0f * WindowRatio.x, 20.0f * WindowRatio.x, ScaleDownTime * 7.0f));
		SetSaturation();
	}
}

float4 Char_ChoiceButton::SizeUpCalCulation(float4 _size, float4 _size2, float Ratio)
{
	float4 LerpUpScale = float4::LerpClamp(_size, _size2, Ratio);
	return LerpUpScale;
}

void Char_ChoiceButton::FontCreate()
{
	NameText = CreateComponent<GameEngineFontRenderer>(6);
	NameText->SetFont(Font);
	NameText->SetFontFlag(FW1_CENTER);
	NameText->SetScale(50.0f * WindowRatio.x);
	NameText->SetColor(float4(0.407f,0.682f,0.698f));
	NameText->GetTransform()->SetLocalPosition(float4{ 0.0f,-130.f,0.0f }*WindowRatio);

	SelectText = CreateComponent<GameEngineFontRenderer>(6);
	SelectText->SetFont(Font);
	SelectText->SetFontFlag(FW1_CENTER);
	SelectText->SetScale(36.0f * WindowRatio.x);
	SelectText->SetColor(float4(0.305f, 0.96f, 0.94f, 1.0f));
	SelectText->GetTransform()->SetLocalPosition(float4{ 0.0f,22.f,0.0f }*WindowRatio);
	SelectText->Off();

	ExplaneText = CreateComponent<GameEngineFontRenderer>(6);
	ExplaneText->SetFont(Font);
	ExplaneText->SetFontFlag(FW1_CENTER);
	ExplaneText->SetScale(20.0f * WindowRatio.x);
	ExplaneText->SetColor(float4(0.455f,0.541f,0.572f));
	ExplaneText->GetTransform()->SetLocalPosition(float4{ 0.0f,-105.0f,0.0f }*WindowRatio);
}

void Char_ChoiceButton::SetSaturation()
{
	if (IsSelect == false)
	{
		CharRender->BSCControl(0.1f, 0.5f, 0.5f);
		NoneSelectRender->BSCControl(0.1f, 0.5f, 0.5f);
		TextNoneRender->BSCControl(0.1f, 0.5f, 0.5f);
		TextBarRender->BSCControl(0.1f, 0.5f, 0.5f);
		NameText->SetColor(float4(0.513f, 0.55f, 0.55f));
		ExplaneText->SetColor(float4(0.388f, 0.388f, 0.388f));
	}
	else
	{
		CharRender->BSCControl(0.5f, 0.5f, 0.5f);
		NoneSelectRender->BSCControl(0.5f, 0.5f, 0.5f);
		TextNoneRender->BSCControl(0.5f, 0.5f, 0.5f);
		TextBarRender->BSCControl(0.5f, 0.5f, 0.5f);
		NameText->SetColor(float4(0.407f, 0.682f, 0.698f));
		ExplaneText->SetColor(float4(0.455f, 0.541f, 0.572f));

	}
	
}


void Char_ChoiceButton::SetUIText(const Char_ChoiceParameter& _Paramter)
{
	NameText->SetText(_Paramter._Text);
	SelectText->SetText(_Paramter._SeletText);
	ExplaneText->SetText(_Paramter._ExplaneText);
}


