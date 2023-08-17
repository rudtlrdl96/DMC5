#include "PrecompileHeader.h"
#include "ContentButton.h"
#include "UIFontRender.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

ContentButton* ContentButton::SelectButton = nullptr;

ContentButton::ContentButton()
{
}

ContentButton::~ContentButton()
{
}

void ContentButton::Start()
{
	Render = CreateComponent<GameEngineUIRenderer>(151);
	Render_Select = CreateComponent<GameEngineUIRenderer>(151);
	Render_Select->Off();
	ExplaneRender = CreateComponent<GameEngineUIRenderer>(152);
	FontCreate();
}

void ContentButton::Update(float _Delta)
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
		ExplaneRender->On();
		StatName->On();
		StatExplane->On();
		TwoExplane->On();
		Render_Select->On();
		Render->Off();
		if (true == GameEngineInput::IsUp("EngineMouseLeft"))
		{
			if (nullptr != Click)
			{
				Click();
			}
		}
	}
	else
	{
		Render->Off();
		Render_Select->Off();
		ExplaneRender->Off();
		StatName->On();
		StatExplane->Off();
		TwoExplane->Off();
	
	}
}

void ContentButton::FontCreate()
{
	StatName = CreateComponent<UIFontRender>(153);
	StatName->SetFont(Font);
	StatName->SetFontFlag(FW1_LEFT);
	StatName->SetScale(32);
	StatName->SetColor(YellowColor);
	StatName->GetTransform()->SetLocalPosition({ -5.0f,-10.0f,0.0f });

	StatExplane = CreateComponent<UIFontRender>(153);
	StatExplane->SetFont(Font);
	StatExplane->SetFontFlag(FW1_LEFT);
	StatExplane->SetScale(24);
	StatExplane->SetColor(float4::WHITE);
	StatExplane->GetTransform()->SetLocalPosition({ -5.0f,-55.0f,0.0f });

	StatValue = CreateComponent<UIFontRender>(151);
	StatValue->SetFont(Font);
	StatValue->SetFontFlag(FW1_LEFT);
	StatValue->SetScale(30);
	StatValue->SetColor(float4::WHITE);
	StatValue->GetTransform()->SetLocalPosition({ 25.0f,20.0f,0.0f });

	TwoExplane = CreateComponent<UIFontRender>(153);
	TwoExplane->SetFont(Font);
	TwoExplane->SetFontFlag(FW1_LEFT);
	TwoExplane->SetScale(24);
	TwoExplane->SetColor(float4::WHITE);
	TwoExplane->GetTransform()->SetLocalPosition({ -5.0f,-90.0f,0.0f });
	TwoExplane->Off();
}

void ContentButton::SetUIText(const StatStringParameter& _Paramter)
{
	StatName->SetText(_Paramter.Name);
	StatExplane->SetText(_Paramter.ExplaneName);
	StatValue->SetText(_Paramter.Value);

	if (_Paramter.Index == 1)
	{
		TwoExplane->On();
		TwoExplane->SetText(_Paramter.TwoExplaneName);


	}
	else
	{
		TwoExplane->Off();
	}
}


