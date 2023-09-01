#include "PrecompileHeader.h"
#include "Shop_ItemButton.h"
#include "UIEnums.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

Shop_ItemButton::Shop_ItemButton()
{
}

Shop_ItemButton::~Shop_ItemButton()
{
}



void Shop_ItemButton::Start()
{
	Render = CreateComponent<GameEngineUIRenderer>(1);
	Render->SetTexture("Shop_ItemNoneSelect.png");
	Render->GetTransform()->SetLocalScale({ 480.0f,102.0f,0.0f });

	Render_Select = CreateComponent<GameEngineUIRenderer>(2);
	Render_Select->SetTexture("Shop_ItemSelect.png");
	Render_Select->GetTransform()->SetLocalScale({ 480.0f,102.0f,0.0f });

	SkillBase_Render = CreateComponent<GameEngineUIRenderer>(3);
	SkillBase_Render->SetTexture("Shop_SkillMovieNone.png");
	SkillBase_Render->GetTransform()->SetLocalScale({ 157.0f,100.0f,0.0f });
	SkillBase_Render->GetTransform()->SetLocalPosition({ -160.0f,0.0f,0.0f });

	SkillSelect_Render = CreateComponent<GameEngineUIRenderer>(3);
	SkillSelect_Render->SetTexture("Shop_SkillMovie.png");
	SkillSelect_Render->GetTransform()->SetLocalScale({ 157.0f,100.0f,0.0f });
	SkillSelect_Render->GetTransform()->SetLocalPosition({ -160.0f,0.0f,0.0f });

	BuyButton_Render = CreateComponent<GameEngineUIRenderer>(4);
	BuyButton_Render->SetTexture("Shop_ItemBuyBG.png");
	BuyButton_Render->GetTransform()->SetLocalScale({ 159.0f,34.0f,0.0f });
	BuyButton_Render->GetTransform()->SetLocalPosition({ 155.0f,-44.0f,0.0f });

	Skill_Render = CreateComponent<GameEngineUIRenderer>(4);
	Skill_Render->SetScaleToTexture("NullTexture.png");
	FontCreate();
}

void Shop_ItemButton::Update(float _Delta)
{
	if (IsSelect == true)
	{
		AddTime += _Delta;
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
		if (true == GameEngineInput::IsUp("CLICK"))
		{
			if (nullptr != Click)
			{
				Click();
			}
		}
		Render->On();
		ScaleDownTime = 0.0f;
		ScaleUpTime += _Delta;
		float LerpPos= GameEngineMath::LerpLimit(0.0f, 40.0f, ScaleUpTime * 10.0f);
		Render_Select->GetTransform()->SetLocalPosition({ LerpPos, 0.0f, 0.0});
		float4 LerpUpScale = float4::LerpClamp(DownScale, UpScale, ScaleUpTime * 10.0f);
		SkillSelect_Render->GetTransform()->SetLocalScale(LerpUpScale);
		Render_Select->ImageClippingX(ScaleUpTime * 8.0f, ClipXDir::Left);
		Render_Select->On();
		SkillBase_Render->Off();
		SkillSelect_Render->On();
		SkillSelect_Render->BSCControl(M0, 0.5f, 0.5f);
		BuyButton_Render->BSCControl(M0, 0.5f, 0.5f);
		BuyButton_Render->On();
		
		
	}
	else
	{
		ScaleUpTime = 0.0f;
		ScaleDownTime += _Delta;
		float4 LerpDownScale = float4::LerpClamp(UpScale, DownScale, ScaleDownTime * 10.0f);
		SkillSelect_Render->GetTransform()->SetLocalScale(LerpDownScale);
		Render_Select->ImageClippingX(1 - ScaleDownTime * 8.0f, ClipXDir::Left);
		if (LerpDownScale == DownScale)
		{
			SkillBase_Render->On();
			SkillSelect_Render->Off();
		}
		Render_Select->GetTransform()->SetLocalPosition({0.f, 0.0f, 0.0 });
		Render->On();
		//Render_Select->Off();
		BuyButton_Render->Off();

	}
}

void Shop_ItemButton::FontCreate()
{
	NameText_Render = CreateComponent<GameEngineFontRenderer>(5);
	NameText_Render->GetTransform()->SetParent(GetTransform());
	NameText_Render->SetFont(Font);
	NameText_Render->SetFontFlag(FW1_CENTER);
	NameText_Render->SetScale(28);
	NameText_Render->SetColor(float4::WHITE);
	NameText_Render->GetTransform()->SetLocalPosition({ 0.0f,22.f,0.0f });

	PriceText_Render = CreateComponent<GameEngineFontRenderer>(5);
	PriceText_Render->GetTransform()->SetParent(GetTransform());
	PriceText_Render->SetFont(Font);
	PriceText_Render->SetFontFlag(FW1_CENTER);
	PriceText_Render->SetScale(28);
	PriceText_Render->SetColor(float4::WHITE);
	PriceText_Render->GetTransform()->SetLocalPosition({ 0.0f,22.f,0.0f });

	BuyText_Render = CreateComponent<GameEngineFontRenderer>(5);
	BuyText_Render->GetTransform()->SetParent(GetTransform());
	BuyText_Render->SetFont("Tahoma Bold");
	BuyText_Render->SetFontFlag(FW1_CENTER);
	BuyText_Render->SetScale(28);
	BuyText_Render->SetColor(float4::WHITE);
	BuyText_Render->GetTransform()->SetLocalPosition({ 0.0f,22.f,0.0f });

}
void Shop_ItemButton::SetTextFont(const ShopTextParameter& _Parameter)
{
	NameText_Render->SetText(_Parameter._Name);
	NameText_Render->SetText(_Parameter._Price);
	NameText_Render->SetText("¹è¿ì±â");
}
//void Shop_ItemButton::RenderOnOff()
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



