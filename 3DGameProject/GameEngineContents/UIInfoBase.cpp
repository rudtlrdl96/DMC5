#include "PrecompileHeader.h"
#include "UIInfoBase.h"
#include "UIEnums.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

UIInfoBase::UIInfoBase()
{
}

UIInfoBase::~UIInfoBase()
{
}

void UIInfoBase::Start()
{
	Render = CreateComponent<GameEngineUIRenderer>(2);
	Render_Base = CreateComponent<GameEngineUIRenderer>(3);
	Render_Bar = CreateComponent<GameEngineUIRenderer>(1);
	FontCreate();
}

void UIInfoBase::Update(float _Delta)
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

void UIInfoBase::FontCreate()
{
	Text = CreateComponent<GameEngineFontRenderer>(4);
	Text->SetFont(Font);
	Text->SetFontFlag(FW1_CENTER);
	Text->SetScale(32);
	Text->SetColor(float4::WHITE);

	SeletText = CreateComponent<GameEngineFontRenderer>(5);
	SeletText->SetFont(Font);
	SeletText->SetFontFlag(FW1_CENTER);
	SeletText->SetScale(36);
	SeletText->SetColor(float4(0.305f, 0.96f, 0.94f, 1.0f));

	ExplaneText = CreateComponent<GameEngineFontRenderer>(5);
	ExplaneText->SetFont("¸¼Àº °íµñ");
	ExplaneText->SetFontFlag(FW1_CENTER);
	ExplaneText->SetScale(24);
	ExplaneText->SetColor(float4::WHITE);


	ExplaneText_2 = CreateComponent<GameEngineFontRenderer>(5);
	ExplaneText_2->SetFont("¸¼Àº °íµñ");
	ExplaneText_2->SetFontFlag(FW1_CENTER);
	ExplaneText_2->SetScale(24);
	ExplaneText_2->SetColor(float4::WHITE);

}


void UIInfoBase::SetUIText(const UIInfoParameter& _Paramter)
{
	Text->SetText(_Paramter._Text);
	SeletText->SetText(_Paramter._SeletText);
	ExplaneText->SetText(_Paramter._ExplaneText);

}


