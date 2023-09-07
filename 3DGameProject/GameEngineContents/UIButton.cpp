#include "PrecompileHeader.h"
#include "UIButton.h"
#include "UIEnums.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

UIButton::UIButton()
{
}

UIButton::~UIButton()
{
}

void UIButton::Start()
{
	Render = CreateComponent<GameEngineUIRenderer>(1);
	Render_Select = CreateComponent<GameEngineUIRenderer>(2);
	Render_Select->ColorOptionValue.MulColor.a = 0.8f;
	Render_Select2 = CreateComponent<GameEngineUIRenderer>(2);
	Render_Select2->ColorOptionValue.MulColor.a = 0.8f;
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

void UIButton::Update(float _Delta)
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
		Render_Select2->ImageClippingX(fillTime *7.0f, ClipXDir::Left);
		Render_Select->ImageClippingX(fillTime * 7.0f, ClipXDir::Right);
		RenderOnOff();
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
		fillTime = 0.0f;
		fallTime += _Delta;
		Render_Select2->ImageClippingX(1- fallTime * 7.0f, ClipXDir::Left);
		Render_Select->ImageClippingX(1- fallTime * 7.0f, ClipXDir::Right);
		RenderOnOff();
	}
	SetFontPos();
}

void UIButton::FontCreate()
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
	ExplaneText->SetFont("³ª´®¹Ù¸¥°íµñ ¿¾ÇÑ±Û");
	ExplaneText->SetFontFlag(Flag);
	ExplaneText->SetScale(24);
	ExplaneText->SetColor(float4::WHITE);

	ExplaneText_1 = CreateComponent<GameEngineFontRenderer>(4);
	ExplaneText_1->SetFont("³ª´®¹Ù¸¥°íµñ ¿¾ÇÑ±Û");
	ExplaneText_1->SetFontFlag(Flag);
	ExplaneText_1->SetScale(24);
	ExplaneText_1->SetColor(float4(0.0f, 0.0f, 0.0f, 0.8f));

	ExplaneText_2 = CreateComponent<GameEngineFontRenderer>(4);
	ExplaneText_2->SetFont("³ª´®¹Ù¸¥°íµñ ¿¾ÇÑ±Û");
	ExplaneText_2->SetFontFlag(Flag);
	ExplaneText_2->SetScale(24);
	ExplaneText_2->SetColor(float4(0.0f, 0.0f, 0.0f, 0.8f));

	ExplaneText_3 = CreateComponent<GameEngineFontRenderer>(4);
	ExplaneText_3->SetFont("³ª´®¹Ù¸¥°íµñ ¿¾ÇÑ±Û");
	ExplaneText_3->SetFontFlag(Flag);
	ExplaneText_3->SetScale(24);
	ExplaneText_3->SetColor(float4(0.0f,0.0f,0.0f,0.8f));
}

void UIButton::RenderOnOff()
{
	if (IsSelect == true)
	{
		Render->Off();
		ExplaneText->On();
		SeletText->On();
		Text->Off();
		ExplaneText_1->On();
		ExplaneText_2->On();
		ExplaneText_3->On();
	}
	else
	{
		Text->On();
		SeletText->Off();
		Render->On();
		ExplaneText->Off();
		ExplaneText_1->Off();
		ExplaneText_2->Off();
		ExplaneText_3->Off();
	}
}

void UIButton::SetFontPos()
{
	ExplaneText_3->GetTransform()->SetWorldPosition({ ExPlanePos.x + 2.0f,ExPlanePos.y + 2.0f,ExPlanePos.z });
	ExplaneText_2->GetTransform()->SetWorldPosition({ ExPlanePos.x,ExPlanePos.y + 2,ExPlanePos.z });
	ExplaneText_1->GetTransform()->SetWorldPosition({ ExPlanePos.x + 2.0f,ExPlanePos.y,ExPlanePos.z });
	ExplaneText->GetTransform()->SetWorldPosition(ExPlanePos);
	ExplaneText->SetFontFlag(Flag);
	ExplaneText_1->SetFontFlag(Flag);
	ExplaneText_2->SetFontFlag(Flag);
	ExplaneText_3->SetFontFlag(Flag);

}


void UIButton::SetUIText(const StringParameter& _Paramter)
{
	Text->SetText(_Paramter._Text);
	SeletText->SetText(_Paramter._SeletText);
	ExplaneText->SetText(_Paramter._ExplaneText);
	ExplaneText_1->SetText(_Paramter._ExplaneText);
	ExplaneText_2->SetText(_Paramter._ExplaneText);
	ExplaneText_3->SetText(_Paramter._ExplaneText);

}


