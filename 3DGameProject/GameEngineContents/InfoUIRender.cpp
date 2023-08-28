#include "PrecompileHeader.h"
#include "InfoUIRender.h"
#include "UIEnums.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

InfoUIRender::InfoUIRender()
{
}

InfoUIRender::~InfoUIRender()
{
}

void InfoUIRender::Start()
{
	Render = CreateComponent<GameEngineUIRenderer>(2);
	Render->GetTransform()->SetLocalScale(float4(170.f, 200.f, 1.0f));
	Render->GetTransform()->SetLocalPosition(float4(700.f, 50.f, 0.0f));
	Render->BSCControl(0.3f, 0.4f, 0.5f);
	Render_Base = CreateComponent<GameEngineUIRenderer>(3);
	Render_Base->GetTransform()->SetLocalScale(float4(546.f, 25.f, 1.0f));
	Render_Base->GetTransform()->SetLocalPosition(float4(700.f, 26.f, 0.0f));
	Render_Base->BSCControl(0.3f, 0.4f, 0.5f);
	Render_Bar = CreateComponent<GameEngineUIRenderer>(1);
	Render_Bar->GetTransform()->SetLocalScale(float4(546.f, 5.f, 1.0f));
	Render_Bar->GetTransform()->SetLocalPosition(float4(700.f, 90.f, 0.0f));
	Render_Bar->BSCControl(0.3f, 0.4f, 0.5f);
	FontCreate();
}

void InfoUIRender::Update(float _Delta)
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
		
	}
}

void InfoUIRender::FontCreate()
{
	Text = CreateComponent<GameEngineFontRenderer>(4);
	Text->SetFont(Font);
	Text->SetFontFlag(FW1_CENTER);
	Text->SetScale(24);
	Text->SetColor(FontColor);
	Text->GetTransform()->SetLocalPosition({ 485.0f,120.f,0.0f });

	SeletText = CreateComponent<GameEngineFontRenderer>(5);
	SeletText->SetFont(Font);
	SeletText->SetFontFlag(FW1_CENTER);
	SeletText->SetScale(40);
	SeletText->SetColor(FontColor);
	SeletText->GetTransform()->SetLocalPosition({ 515.0f,90.f,0.0f });

	ExplaneText = CreateComponent<GameEngineFontRenderer>(5);
	ExplaneText->SetFont("Tahoma Bold");
	ExplaneText->SetFontFlag(FW1_CENTER);
	ExplaneText->SetScale(16);
	ExplaneText->SetColor(float4(0.701f,0.772f,0.788f, 1.0f));
	ExplaneText->GetTransform()->SetLocalPosition({ 515.0f,40.f,0.0f, });


	ExplaneText_2 = CreateComponent<GameEngineFontRenderer>(8);
	ExplaneText_2->SetFont("Tahoma Bold");
	ExplaneText_2->SetFontFlag(FW1_CENTER);
	ExplaneText_2->SetScale(16);
	ExplaneText_2->SetColor(float4(0.656f, 0.668f, 0.665f,1.0f));
	ExplaneText_2->GetTransform()->SetLocalPosition({ 705.0f,40.f,0.0f });

}


void InfoUIRender::SetUIText(const UIInfoParameter& _Paramter)
{
	Text->SetText(_Paramter._Text);
	SeletText->SetText(_Paramter._SeletText);
	ExplaneText->SetText(_Paramter._ExplaneText);
	ExplaneText_2->SetText(_Paramter._ExplaneText2);
}


