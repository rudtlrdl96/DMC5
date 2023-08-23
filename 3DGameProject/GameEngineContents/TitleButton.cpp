#include "PrecompileHeader.h"
#include "TitleButton.h"
#include "UIEnums.h"
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

TitleButton::TitleButton()
{
}

TitleButton::~TitleButton()
{
}

void TitleButton::Start()
{
	Render = CreateComponent<GameEngineUIRenderer>(1);
	Render_Select = CreateComponent<GameEngineUIRenderer>(2);
	Render_Select->ColorOptionValue.MulColor.a = 0.9f;
	Render_Select2 = CreateComponent<GameEngineUIRenderer>(2);
	Render_Select2->ColorOptionValue.MulColor.a = 0.9f;
	Render_Select->ImageClippingX(0.0f, ClipXDir::Left);
	Render_Select2->ImageClippingX(0.0f, ClipXDir::Left);

	Render_Enter = CreateComponent<GameEngineUIRenderer>(6);
	Render_Enter->ColorOptionValue.MulColor.a = 0.5f;
	Render_Enter2 = CreateComponent<GameEngineUIRenderer>(6);
	Render_Enter2->ColorOptionValue.MulColor.a = 0.5f;
	Render_Enter->ImageClippingX(0.0f, ClipXDir::Left);
	Render_Enter2->ImageClippingX(0.0f, ClipXDir::Left);

	FontCreate();
}

void TitleButton::Update(float _Delta)
{
	GameEngineCamera* Camera = Render->GetCamera();

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
	if (true == GameEngineTransform::AABB2DToSpehre2D(Render->GetTransform()->GetCollisionData(), MouseData))
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
	if (IsSelect ==true)
	{
		fillTime += _Delta;
		fallTime = 0.0f;
		Render_Select2->ImageClippingX(fillTime *4.0f, ClipXDir::Left);
		Render_Select->ImageClippingX(fillTime * 4.0f, ClipXDir::Right);
		Render->Off();
		ExplaneText->On();
		SeletText->On();
		Text->Off();
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
		fillTime = 0.0f;
		fallTime += _Delta;
		Render_Select2->ImageClippingX(1- fallTime * 4.0f, ClipXDir::Left);
		Render_Select->ImageClippingX(1- fallTime * 4.0f, ClipXDir::Right);
		Text->On();
		SeletText->Off();
		Render->On();
		ExplaneText->Off();
	}
	if (EnterValue == true)
	{
		EnterKeyRender(_Delta);
	}
}

void TitleButton::FontCreate()
{
	Text = CreateComponent<GameEngineFontRenderer>(4);
	Text->SetFont(Font);
	Text->SetFontFlag(FW1_CENTER);
	Text->SetScale(32);
	Text->SetColor(float4::WHITE);
	Text->GetTransform()->SetLocalPosition({0.0f,22.f,0.0f});

	SeletText = CreateComponent<GameEngineFontRenderer>(5);
	SeletText->SetFont(Font);
	SeletText->SetFontFlag(FW1_CENTER);
	SeletText->SetScale(36);
	SeletText->SetColor(float4(0.305f,0.96f,0.94f,1.0f));
	SeletText->GetTransform()->SetLocalPosition({ 0.0f,22.f,0.0f });
	SeletText->Off();
	ExplaneText = CreateComponent<GameEngineFontRenderer>(5);
	ExplaneText->SetFont(Font);
	ExplaneText->SetFontFlag(FW1_CENTER);
	ExplaneText->SetScale(24);
	ExplaneText->SetColor(float4::WHITE);
	ExplaneText->GetTransform()->SetWorldPosition({0.0f,-300.0f,0.0f});
}

void TitleButton::EnterKeyRender(float _Delta)
{
	EnterTime += _Delta;
	Render_Enter2->ImageClippingX(EnterTime * 4.0f, ClipXDir::Left);
	Render_Enter->ImageClippingX(EnterTime * 4.0f, ClipXDir::Right);
	if (EnterTime * 4.0f > 1.0f)
	{
		GameEngineCore::ChangeLevel("Char_ChoiceLevel");
	}
}

void TitleButton::SetUIText(const StringParameter& _Paramter)
{
	Text->SetText(_Paramter._Text);
	SeletText->SetText(_Paramter._SeletText);
	ExplaneText->SetText(_Paramter._ExplaneText);
}


