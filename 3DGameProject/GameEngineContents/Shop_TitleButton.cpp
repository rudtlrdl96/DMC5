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
	Render->SetScaleToTexture("");
	Render_Select = CreateComponent<GameEngineUIRenderer>(2);
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
		if (true == GameEngineInput::IsUp("CLICK"))
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
	

	}
	else
	{
	}
}

void Shop_TitleButton::FontCreate()
{
	FontRender = CreateComponent<GameEngineFontRenderer>(4);
	FontRender->SetFont(Font);
	FontRender->SetFontFlag(FW1_CENTER);
	FontRender->SetScale(32);
	FontRender->SetColor(float4::WHITE);
	FontRender->GetTransform()->SetLocalPosition({ 0.0f,22.f,0.0f });

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



void Shop_TitleButton::SetTextFont(std::string_view& _Text)
{
	FontRender->SetText(_Text);
}


