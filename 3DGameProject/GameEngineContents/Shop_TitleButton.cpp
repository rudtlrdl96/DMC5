#include "PrecompileHeader.h"
#include "Shop_TitleButton.h"
#include "UIEnums.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

Shop_TitleButton::Shop_TitleButton()
{
}

Shop_TitleButton::~Shop_TitleButton()
{
}

void Shop_TitleButton::Start()
{
	Render = CreateComponent<GameEngineUIRenderer>(1);
	Render->SetScaleToTexture("Shop_TitleButton.png");
	Render_Bottom = CreateComponent<GameEngineUIRenderer>(1);
	Render_Bottom->SetScaleToTexture("NullTexture.png");
	Render_Top = CreateComponent<GameEngineUIRenderer>(1);
	Render_Top->SetScaleToTexture("NullTexture.png");
	Render_Select = CreateComponent<GameEngineUIRenderer>(2);
	Render_Select->SetScaleToTexture("NullTexture.png");
	Render_Select->ColorOptionValue.MulColor.a = 0.5f;
	Render->Off();
	FontCreate();
}

void Shop_TitleButton::Update(float _Delta)
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
		if (true == GameEngineInput::IsUp("UI_CLICK"))
		{
			if (nullptr != Click)
			{
				Click();
			}
		}
	}
	else
	{
		
	}
	if (IsSelect == true)
	{
		AddTime += _Delta;
		Render->On();
		if (IsValue == false)
		{
			M0 = GameEngineMath::LerpLimit(0.4f, 1.2f, AddTime * 2.0f);
			if (M0 == 1.2f)
			{
				IsValue = true;
				AddTime = 0.0f;
			}
		}
		else
		{
			M0 = GameEngineMath::LerpLimit(1.2f, 0.4f, AddTime * 2.0f);
			if (M0 == 0.4f)
			{
				IsValue = false;
				AddTime = 0.0f;
			}
		}
		FontRender->SetColor(float4::WHITE);
		if (IsPosValue == false)
		{
			FontRender->GetTransform()->SetLocalPosition({ 0.0f,20.f,0.0f });
			Render->BSCControl(M0, 0.5f, 0.5f);
		}
		else
		{
			Render_Select->BSCControl(M0, 0.5f, 0.5f);
		}

	}
	else
	{
		Render->Off();
		FontRender->SetColor(float4(0.462f,0.58f,0.576f));
		if (IsPosValue == false)
		{
			FontRender->GetTransform()->SetLocalPosition({ 0.0f,19.f,0.0f });
		}
	}
	SetTextFont(Text);
}

void Shop_TitleButton::FontCreate()
{
	FontRender = CreateComponent<GameEngineFontRenderer>(4);
	FontRender->GetTransform()->SetParent(GetTransform());
	FontRender->SetFont("³ª´®¹Ù¸¥°íµñ ¿¾ÇÑ±Û");
	FontRender->SetFontFlag(FW1_CENTER);
	FontRender->SetScale(28);
	FontRender->SetColor(float4::WHITE);
	FontRender->GetTransform()->SetLocalPosition({ 0.0f,20.f,0.0f });

}

//void Shop_TitleButton::RenderOnOff()
//{
//	if (IsSelect == true)
//	{
//		Render->Off();
//		ExplaneText->On();
//		SeletText->On();
//		Text->Off();
//		ExplaneText_1->On();
//		ExplaneText_2->On();
//		ExplaneText_3->On();
//	}
//	else
//	{
//		Text->On();
//		SeletText->Off();
//		Render->On();
//		ExplaneText->Off();
//		ExplaneText_1->Off();
//		ExplaneText_2->Off();
//		ExplaneText_3->Off();
//	}
//}



void Shop_TitleButton::SetTextFont(std::string_view _Text)
{
	FontRender->SetText(_Text);
}


