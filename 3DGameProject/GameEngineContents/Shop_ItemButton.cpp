#include "PrecompileHeader.h"
#include "Shop_ItemButton.h"
#include "UIEnums.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
std::vector<std::shared_ptr<Shop_ItemButton>> Shop_ItemButton::Items;
Shop_ItemButton::Shop_ItemButton()
{
}

Shop_ItemButton::~Shop_ItemButton()
{
}


void Shop_ItemButton::CreateItemUI(GameEngineLevel* _Level, float4 _Pos, const ShopTextParameter& _Param, GameEngineTransform* _Trans)
{
	std::shared_ptr<Shop_ItemButton> ItemButton = _Level->CreateActor<Shop_ItemButton>();
	ItemButton->GetTransform()->SetParent(_Trans);
	ItemButton->GetTransform()->SetLocalPosition(_Pos);
	ItemButton->GetSkillRender()->SetTexture(_Param._png);
	ItemButton->GetNameText()->SetText(_Param._Name);
	ItemButton->GetPriceText()->SetText(_Param._Price);
	Items.push_back(ItemButton);
}
void Shop_ItemButton::Start()
{
	Render = CreateComponent<GameEngineUIRenderer>(1);
	Render->SetTexture("Shop_ItemNoneSelect.png");
	Render->GetTransform()->SetLocalScale({ 480.0f,102.0f,0.0f });
	Render->ColorOptionValue.MulColor.a = 0.8f;
	Render->ColorOptionValue.BSCColor = { 0.4f,0.4f,0.4f };

	Render_Select = CreateComponent<GameEngineUIRenderer>(2);
	Render_Select->SetTexture("Shop_ItemSelect.png");
	Render_Select->GetTransform()->SetLocalScale({ 480.0f,102.0f,0.0f });
	Render_Select->GetTransform()->SetLocalPosition({ 0.f, 0.0f, 0.0f });


	SkillBase_Render = CreateComponent<GameEngineUIRenderer>(4);
	SkillBase_Render->SetTexture("Shop_SkillMovieNone.png");
	SkillBase_Render->GetTransform()->SetLocalScale({ 157.0f,100.0f,0.0f });
	SkillBase_Render->GetTransform()->SetLocalPosition({ -160.0f,0.0f,0.0f });
	SkillBase_Render->ColorOptionValue.MulColor.a = 0.5f;

	SkillSelect_Render = CreateComponent<GameEngineUIRenderer>(4);
	SkillSelect_Render->SetTexture("Shop_SkillMovie.png");
	SkillSelect_Render->GetTransform()->SetLocalScale({ 157.0f,100.0f,0.0f });
	SkillSelect_Render->GetTransform()->SetLocalPosition({ -160.0f,0.0f,0.0f });
	SkillSelect_Render->ColorOptionValue.MulColor.a = 0.5f;

	BuyButton_Render = CreateComponent<GameEngineUIRenderer>(5);
	BuyButton_Render->SetTexture("Shop_ItemBuyBG.png");
	BuyButton_Render->GetTransform()->SetLocalScale({ 159.0f,34.0f,0.0f });
	BuyButton_Render->GetTransform()->SetLocalPosition({ 155.0f,-44.0f,0.0f });

	Money_Render = CreateComponent<GameEngineUIRenderer>(5);
	Money_Render->SetTexture("Shop_Coin.png");
	Money_Render->GetTransform()->SetLocalScale({ 31.0f,28.0f,0.0f });

	Skill_Render = CreateComponent<GameEngineUIRenderer>(3);
	Skill_Render->GetTransform()->SetLocalPosition({ -160.0f,0.0f,0.0f });
	FontCreate();
}

void Shop_ItemButton::Update(float _Delta)
{
	if (IsSelect == true)
	{
		if (true == GameEngineInput::IsUp("CLICK"))
		{
			if (nullptr != Click)
			{
				Click();
			}
		}

		RenderOnOff();
		BlinkRender(_Delta);
		MoveRender(_Delta);
		SizeUpDown(_Delta);
		NameText_Render->SetColor(SelectColor);
		PriceText_Render->SetColor(SelectColor);
	}
	else
	{

		MoveRender(_Delta);
		RenderOnOff();
		SizeUpDown(_Delta);
		NameText_Render->SetColor(NoneColor);
		PriceText_Render->SetColor(NoneColor);
	}
}

void Shop_ItemButton::FontCreate()
{
	NameText_Render = CreateComponent<GameEngineFontRenderer>(6);
	NameText_Render->GetTransform()->SetParent(GetTransform());
	NameText_Render->SetFont(Font);
	NameText_Render->SetFontFlag(FW1_LEFT);
	NameText_Render->SetScale(28);


	PriceText_Render = CreateComponent<GameEngineFontRenderer>(6);
	PriceText_Render->GetTransform()->SetParent(GetTransform());
	PriceText_Render->SetFont(Font);
	PriceText_Render->SetFontFlag(FW1_LEFT);
	PriceText_Render->SetScale(28);

	BuyText_Render = CreateComponent<GameEngineFontRenderer>(6);
	BuyText_Render->GetTransform()->SetParent(GetTransform());
	BuyText_Render->SetFont("Tahoma Bold");
	BuyText_Render->SetFontFlag(FW1_CENTER);
	BuyText_Render->SetScale(20);
	BuyText_Render->SetColor(float4::WHITE);
	BuyText_Render->SetText("¹è¿ì±â");


}
void Shop_ItemButton::RenderOnOff()
{
	if (IsSelect == true)
	{
		Render->On();
		BuyButton_Render->On();
		BuyText_Render->On();
		Render_Select->On();
		SkillBase_Render->Off();
		SkillSelect_Render->On();
	}
	else
	{
		Render->On();
		BuyButton_Render->Off();
		BuyText_Render->Off();
	}
}
void Shop_ItemButton::BlinkRender(float _Delta)
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
		SkillSelect_Render->BSCControl(M0, 0.5f, 0.5f);
		BuyButton_Render->BSCControl(M0, 0.5f, 0.5f);
	}
}
void Shop_ItemButton::MoveRender(float _Delta)
{
	if (IsSelect == true)
	{
		FillTime += _Delta;
		FallTime = 0.0f;
		BuyText_Render->GetTransform()->SetLocalPosition({ 153.0f,-31.f,0.0f });
		PriceText_Render->GetTransform()->SetLocalPosition(LerpScaleSize({ -40.0f,-18.f,0.0f }, { -9.0f,-18.f,0.0f }, FillTime * Ratio));
		NameText_Render->GetTransform()->SetLocalPosition(LerpScaleSize({ -66.0f,36.f,0.0f }, { -32.0f,36.f,0.0f }, FillTime * Ratio));
		Money_Render->GetTransform()->SetLocalPosition(LerpScaleSize({ -56.0f,-36.0f,0.0f }, { -24.0f,-36.0f,0.0f }, FillTime * Ratio));
	}
	else
	{
		FallTime += _Delta;
		FillTime = 0.0f;
		PriceText_Render->GetTransform()->SetLocalPosition(LerpScaleSize({ -9.0f,-18.f,0.0f }, { -40.0f,-18.f,0.0f }, FallTime * Ratio));
		NameText_Render->GetTransform()->SetLocalPosition(LerpScaleSize({ -32.0f,36.f,0.0f }, { -66.0f,36.f,0.0f }, FallTime * Ratio));
		Money_Render->GetTransform()->SetLocalPosition(LerpScaleSize({ -24.0f,-36.0f,0.0f }, { -56.0f,-36.0f,0.0f }, FallTime * Ratio));

	}
}
void Shop_ItemButton::SizeUpDown(float _Delta)
{
	if (IsSelect == true)
	{
		ScaleDownTime = 0.0f;
		ScaleUpTime += _Delta;
		float LerpPos = GameEngineMath::LerpLimit(0.0f, 40.0f, ScaleUpTime * 10.0f);
		Render_Select->GetTransform()->SetLocalPosition({ LerpPos, 0.0f, 0.0 });
		float4 LerpUpScale = float4::LerpClamp(DownScale, UpScale, ScaleUpTime * 10.0f);
		SkillSelect_Render->GetTransform()->SetLocalScale(LerpUpScale);
		Skill_Render->GetTransform()->SetLocalScale({ LerpUpScale.x - 5.0f,LerpUpScale.y - 9.0f,LerpUpScale.z });
		Skill_Render->BSCControl(1.0f, 0.5f, 0.5f);
		Render_Select->ImageClippingX(ScaleUpTime * 4.0f, ClipXDir::Left);

	}
	else
	{
		ScaleUpTime = 0.0f;
		ScaleDownTime += _Delta;
		float4 LerpDownScale = float4::LerpClamp(UpScale, DownScale, ScaleDownTime * 10.0f);
		SkillSelect_Render->GetTransform()->SetLocalScale(LerpDownScale);
		Skill_Render->GetTransform()->SetLocalScale({ LerpDownScale.x - 5.0f,LerpDownScale.y - 9.0f,LerpDownScale.z });
		Render_Select->ImageClippingX(1 - ScaleDownTime * 4.0f, ClipXDir::Left);
		Skill_Render->BSCControl(0.5f, 0.5f, 0.5f);
		if (LerpDownScale == DownScale)
		{
			SkillBase_Render->On();
			SkillSelect_Render->Off();
		}
	}
}

float4 Shop_ItemButton::LerpScaleSize(float4 _Start, float4 _End, float _Ratio)
{
	float4 LerpValue = float4::LerpClamp(_Start, _End, _Ratio);
	return LerpValue;
}
