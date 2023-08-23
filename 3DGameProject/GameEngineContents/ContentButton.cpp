#include "PrecompileHeader.h"
#include "ContentButton.h"
#include "UIEnums.h"
#include "UIFontRender.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

ContentButton::ContentButton()
{
}

ContentButton::~ContentButton()
{
}

void ContentButton::Start()
{
	Render = CreateComponent<GameEngineUIRenderer>(UIOrder::SELECT);
	Render_Select = CreateComponent<GameEngineUIRenderer>(UIOrder::SELECT2);
	//Render_Select->Off();
	//FontCreate();
}

void ContentButton::Update(float _Delta)
{

	GameEngineCamera* Camera = Render->GetCamera();

	// 랜더러 
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

	//if (true == GameEngineTransform::AABB2DToSpehre2D(Render->GetTransform()->GetCollisionData(), MouseData))
	//{
	//	Render_Select->On();
	//	Render->Off();
	//	if (true == GameEngineInput::IsUp("EngineMouseLeft"))
	//	{
	//		if (nullptr != Click)
	//		{
	//			Click();
	//		}
	//	}
	//}
	//else
	//{
	//	Render->On();
	//	Render_Select->Off();
	//}
}

void ContentButton::FontCreate()
{
	// 폰트가 아직 안뜸.
	//ExplaneText = CreateComponent<UIFontRender>(153);
	//ExplaneText->SetFont(Font);
	//ExplaneText->SetFontFlag(FW1_LEFT);
	//ExplaneText->SetScale(32);
	//ExplaneText->SetColor(float4::WHITE);
	//ExplaneText->GetTransform()->SetLocalPosition(float4::ZERONULL);

}

void ContentButton::SetUIText(const StringParameter& _Paramter)
{
	Text->SetText(_Paramter._Text);
	SeletText->SetText(_Paramter._SeletText);
	ExplaneText->SetText(_Paramter._ExplaneText);
}


