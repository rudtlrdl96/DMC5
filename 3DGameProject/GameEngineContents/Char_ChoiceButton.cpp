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
	NoneSelectRender = CreateComponent<GameEngineUIRenderer>(1);
	NoneSelectRender->SetTexture("Char_NoneSelect.png");
	SelectRender = CreateComponent<GameEngineUIRenderer>(2);
	SelectRender->SetTexture("Char_Select.png");
	CharRender = CreateComponent<GameEngineUIRenderer>(3);
	TextNoneRender = CreateComponent<GameEngineUIRenderer>(4);
	TextNoneRender->SetScaleToTexture("Char_NoneText.png");
	TextBarRender = CreateComponent<GameEngineUIRenderer>(6);
	TextBarRender->SetScaleToTexture("Char_Bar.png");
	TextBarRender->GetTransform()->SetLocalPosition({0.0f,-130.0f,0.0f});
	TextBarRender->ColorOptionValue.MulColor.a = 0.9f;
	TextSelectRender = CreateComponent<GameEngineUIRenderer>(6);
	TextSelectRender->SetScaleToTexture("Char_SelectText.png");
	TextSelectRender->ImageClippingX(0, ClipXDir::Left);
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
		SelectRender->GetTransform()->SetLocalScale(SizeUpCalCulation(Scale, SeletScale, ScaleUpTime * 7.0f));
		NoneSelectRender->Off();
		NoneSelectRender->GetTransform()->SetLocalScale(Scale);
		CharRender->GetTransform()->SetLocalScale(SizeUpCalCulation(Char_Scale, Char_SeletScale, ScaleUpTime * 7.0f));
		TextNoneRender->GetTransform()->SetLocalScale(SizeUpCalCulation(Char_TextScale, Char_TextSeletScale, ScaleUpTime * 7.0f));
		TextBarRender->GetTransform()->SetLocalScale(SizeUpCalCulation(Char_BarScale, Char_BarScaleUp, ScaleUpTime * 7.0f));
		TextSelectRender->ImageClippingX(ScaleUpTime*10.0f, ClipXDir::Left);
		NameText->SetScale(GameEngineMath::LerpLimit(50.0f,55.0f, ScaleUpTime * 7.0f));
		ExplaneText->SetScale(GameEngineMath::LerpLimit(20.0f, 22.0f, ScaleUpTime * 7.0f));

		if (true == GameEngineInput::IsUp("UI_Enter"))
		{
			if (nullptr != Click)
			{
				EnterValue = true;
				Click();
			}
		}
	}
	else
	{
		ScaleUpTime = 0.0f;
		ScaleDownTime += _Delta;
		SelectRender->Off();
		SelectRender->GetTransform()->SetLocalScale(Scale);
		NoneSelectRender->GetTransform()->SetLocalScale(SizeUpCalCulation(SeletScale, Scale, ScaleDownTime * 7.0f));
		NoneSelectRender->On();
		CharRender->GetTransform()->SetLocalScale(SizeUpCalCulation(Char_SeletScale, Char_Scale, ScaleDownTime * 7.0f));
		TextNoneRender->GetTransform()->SetLocalScale(SizeUpCalCulation(Char_TextSeletScale, Char_TextScale, ScaleDownTime * 7.0f));
		TextBarRender->GetTransform()->SetLocalScale(SizeUpCalCulation(Char_BarScaleUp, Char_BarScale, ScaleDownTime * 7.0f));
		TextSelectRender->ImageClippingX(1- ScaleDownTime*10.0f, ClipXDir::Left);
		NameText->SetScale(GameEngineMath::LerpLimit(55.0f, 50.0f, ScaleDownTime * 7.0f));
		ExplaneText->SetScale(GameEngineMath::LerpLimit(22.0f, 20.0f, ScaleDownTime * 7.0f));

	}
	if (EnterValue == true)
	{
		EnterKeyRender(_Delta);
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
	NameText->SetScale(50);
	NameText->SetColor(float4(0.407f,0.682f,0.698f));
	NameText->GetTransform()->SetLocalPosition({ 0.0f,-130.f,0.0f });

	SelectText = CreateComponent<GameEngineFontRenderer>(6);
	SelectText->SetFont(Font);
	SelectText->SetFontFlag(FW1_CENTER);
	SelectText->SetScale(36);
	SelectText->SetColor(float4(0.305f, 0.96f, 0.94f, 1.0f));
	SelectText->GetTransform()->SetLocalPosition({ 0.0f,22.f,0.0f });
	SelectText->Off();

	ExplaneText = CreateComponent<GameEngineFontRenderer>(6);
	ExplaneText->SetFont(Font);
	ExplaneText->SetFontFlag(FW1_CENTER);
	ExplaneText->SetScale(20);
	ExplaneText->SetColor(float4(0.455f,0.541f,0.572f));
	ExplaneText->GetTransform()->SetLocalPosition({ 0.0f,-105.0f,0.0f });
}

void Char_ChoiceButton::EnterKeyRender(float _Delta)
{

}

void Char_ChoiceButton::SetUIText(const Char_ChoiceParameter& _Paramter)
{
	NameText->SetText(_Paramter._Text);
	SelectText->SetText(_Paramter._SeletText);
	ExplaneText->SetText(_Paramter._ExplaneText);
}


