#include "PrecompileHeader.h"
#include "Shop_ItemButton.h"
#include "UIEnums.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
std::vector<std::shared_ptr<Shop_ItemButton>> Shop_ItemButton::Items; 
std::vector<std::shared_ptr<Shop_ItemButton>> Shop_ItemButton::Skills;
std::vector<std::shared_ptr<Shop_ItemButton>> Shop_ItemButton::SecoundSkills;
bool  Shop_ItemButton::IsScaleValue = false;
Shop_ItemButton::Shop_ItemButton()
{
}

Shop_ItemButton::~Shop_ItemButton()
{
}


void Shop_ItemButton::CreateItemUI(GameEngineLevel* _Level, float4 _Pos, const ShopTextParameter& _Param, GameEngineTransform* _Trans, float4 _RenderScale, float4 _RenderPos)
{
	std::shared_ptr<Shop_ItemButton> ItemButton = _Level->CreateActor<Shop_ItemButton>();
	ItemButton->GetTransform()->SetParent(_Trans);
	ItemButton->GetTransform()->SetLocalPosition(_Pos);
	ItemButton->GetSkillRender()->SetScaleToTexture(_Param._png);
	ItemButton->GetSkillRender()->GetTransform()->SetLocalScale(ItemButton->GetSkillRender()->GetTransform()->GetLocalScale() * GameEngineActor::ScreenRatio);
	ItemButton->GetBaseRender()->SetScaleToTexture(_Param._BaseNone);
	ItemButton->GetBaseRender()->GetTransform()->SetLocalScale(ItemButton->GetBaseRender()->GetTransform()->GetLocalScale() * GameEngineActor::ScreenRatio);
	ItemButton->GetSelectRender()->SetScaleToTexture(_Param._BaseSelect);
	ItemButton->GetSelectRender()->GetTransform()->SetLocalScale(ItemButton->GetSelectRender()->GetTransform()->GetLocalScale() * GameEngineActor::ScreenRatio);
	ItemButton->GetSelectRender()->GetTransform()->SetLocalScale(_RenderScale);
	ItemButton->GetSelectRender()->GetTransform()->SetLocalPosition(_RenderPos);
	ItemButton->GetBaseRender()->GetTransform()->SetLocalScale(_RenderScale);
	ItemButton->GetBaseRender()->GetTransform()->SetLocalPosition(_RenderPos);
	ItemButton->GetNameText()->SetText(_Param._Name);
	ItemButton->GetPriceText()->SetText(_Param._Price);
	IsScaleValue = _Param.IsValue;
	if (_Param.IsValue == true)
	{
		ItemButton->GetRender()->GetTransform()->SetLocalScale(float4{ 480.0f,102.0f,0.0f }*GameEngineActor::ScreenRatio);
		ItemButton->GetRender_Select()->GetTransform()->SetLocalScale(float4{ 480.0f,102.0f,0.0f }*GameEngineActor::ScreenRatio);
		Skills.push_back(ItemButton);
	}
	else
	{
		ItemButton->GetRender()->GetTransform()->SetLocalScale(float4{ 356.0f,102.0f,0.0f }*GameEngineActor::ScreenRatio);
		ItemButton->GetRender_Select()->GetTransform()->SetLocalScale(float4{ 356.0f,102.0f,0.0f }*GameEngineActor::ScreenRatio);
		Items.push_back(ItemButton);
	}
	if (_Param.Value == 1)
	{
		ItemButton->GetRender()->GetTransform()->SetLocalScale(float4{ 480.0f,102.0f,0.0f }*GameEngineActor::ScreenRatio);
		ItemButton->GetRender_Select()->GetTransform()->SetLocalScale(float4{ 480.0f,102.0f,0.0f }*GameEngineActor::ScreenRatio);
		SecoundSkills.push_back(ItemButton);
	}
}
void Shop_ItemButton::Start()
{
	Render = CreateComponent<GameEngineUIRenderer>(1);
	Render->SetTexture("Shop_ItemNoneSelect.png");

	Render->ColorOptionValue.MulColor.a = 0.8f;
	Render->ColorOptionValue.BSCColor = { 0.4f,0.4f,0.4f };

	Render_Select = CreateComponent<GameEngineUIRenderer>(2);
	Render_Select->SetTexture("Shop_ItemSelect.png");
	Render_Select->GetTransform()->SetLocalPosition({ 0.f, 0.0f, 0.0f });


	SkillBase_Render = CreateComponent<GameEngineUIRenderer>(4);
	//SkillBase_Render->ColorOptionValue.MulColor.a = 0.5f;

	SkillSelect_Render = CreateComponent<GameEngineUIRenderer>(4);
	SkillSelect_Render->ColorOptionValue.MulColor.a = 0.5f;

	BuyButton_Render = CreateComponent<GameEngineUIRenderer>(5);
	BuyButton_Render->SetTexture("Shop_ItemBuyBG.png");
	BuyButton_Render->GetTransform()->SetLocalScale(float4{ 159.0f,34.0f,0.0f }*GameEngineActor::ScreenRatio);
	BuyButton_Render->GetTransform()->SetLocalPosition(float4{ 155.0f,-44.0f,0.0f }*GameEngineActor::ScreenRatio);

	Money_Render = CreateComponent<GameEngineUIRenderer>(5);
	Money_Render->SetTexture("Shop_Coin.png");
	Money_Render->GetTransform()->SetLocalScale(float4{ 31.0f,28.0f,0.0f }*GameEngineActor::ScreenRatio);

	Skill_Render = CreateComponent<GameEngineUIRenderer>(4);
	Skill_Render->GetTransform()->SetLocalPosition(float4{ -160.0f,0.0f,0.0f }*GameEngineActor::ScreenRatio);
	FontCreate();
}

void Shop_ItemButton::Update(float _Delta)
{
	if (IsSelect == true)
	{
		if (true == GameEngineInput::IsUp("UI_CLICK"))
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
	if (SizeValue == false)
	{
		BuyText_Render->SetText("±¸¸Å");

	}


}

void Shop_ItemButton::FontCreate()
{
	NameText_Render = CreateComponent<GameEngineFontRenderer>(6);
	NameText_Render->GetTransform()->SetParent(GetTransform());
	NameText_Render->SetFont(Font);
	NameText_Render->SetFontFlag(FW1_LEFT);
	NameText_Render->SetScale(28.0f * GameEngineActor::ScreenRatio.x);


	PriceText_Render = CreateComponent<GameEngineFontRenderer>(6);
	PriceText_Render->GetTransform()->SetParent(GetTransform());
	PriceText_Render->SetFont(Font);
	PriceText_Render->SetFontFlag(FW1_LEFT);
	PriceText_Render->SetScale(28.0f * GameEngineActor::ScreenRatio.x);

	BuyText_Render = CreateComponent<GameEngineFontRenderer>(6);
	BuyText_Render->GetTransform()->SetParent(GetTransform());
	BuyText_Render->SetFont("³ª´®¹Ù¸¥°íµñ ¿¾ÇÑ±Û");
	BuyText_Render->SetFontFlag(FW1_CENTER);
	BuyText_Render->SetScale(20.0f * GameEngineActor::ScreenRatio.x);
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
		SkillSelect_Render->On();
	}
	else
	{
		Render->On();
		BuyButton_Render->Off();
		BuyText_Render->Off();
		SkillSelect_Render->Off();
	}
}
void Shop_ItemButton::BlinkRender(float _Delta)
{
	if (IsSelect == true)
	{
		AddTime += _Delta;
		if (IsValue == false)
		{
			M0 = GameEngineMath::LerpLimit(0.5f, 1.5f, AddTime * 2.0f);
			if (M0 == 1.5f)
			{
				IsValue = true;
				AddTime = 0.0f;
			}
		}
		else
		{
			M0 = GameEngineMath::LerpLimit(1.5f, 0.5f, AddTime *2.0f);
			if (M0 == 0.5f)
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
		BuyText_Render->GetTransform()->SetLocalPosition(float4{ 153.0f,-31.f,0.0f }*GameEngineActor::ScreenRatio);
		PriceText_Render->GetTransform()->SetLocalPosition(LerpScaleSize(float4{ -40.0f,-18.f,0.0f }*GameEngineActor::ScreenRatio, float4{ -9.0f,-18.f,0.0f }*GameEngineActor::ScreenRatio, FillTime * Ratio));
		NameText_Render->GetTransform()->SetLocalPosition(LerpScaleSize(float4{ -66.0f,36.f,0.0f }*GameEngineActor::ScreenRatio, float4{ -32.0f,36.f,0.0f }*GameEngineActor::ScreenRatio, FillTime * Ratio));
		Money_Render->GetTransform()->SetLocalPosition(LerpScaleSize(float4{ -56.0f,-36.0f,0.0f }*GameEngineActor::ScreenRatio, float4{ -24.0f,-36.0f,0.0f }*GameEngineActor::ScreenRatio, FillTime * Ratio));
	}
	else
	{
		FallTime += _Delta;
		FillTime = 0.0f;
		PriceText_Render->GetTransform()->SetLocalPosition(LerpScaleSize(float4{ -9.0f,-18.f,0.0f }*GameEngineActor::ScreenRatio, float4{ -40.0f,-18.f,0.0f }*GameEngineActor::ScreenRatio, FallTime * Ratio));
		NameText_Render->GetTransform()->SetLocalPosition(LerpScaleSize(float4{ -32.0f,36.f,0.0f }*GameEngineActor::ScreenRatio, float4{ -66.0f,36.f,0.0f }*GameEngineActor::ScreenRatio, FallTime * Ratio));
		Money_Render->GetTransform()->SetLocalPosition(LerpScaleSize(float4{ -24.0f,-36.0f,0.0f }*GameEngineActor::ScreenRatio, float4{ -56.0f,-36.0f,0.0f }*GameEngineActor::ScreenRatio, FallTime * Ratio));

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
		float4 LerpUpScale = float4::LerpClamp(DownScale * GameEngineActor::ScreenRatio, UpScale * GameEngineActor::ScreenRatio, ScaleUpTime * 10.0f);
		if (SizeValue == true)
		{
			SkillSelect_Render->GetTransform()->SetLocalScale(LerpUpScale);
			Skill_Render->GetTransform()->SetLocalScale({ LerpUpScale.x - 5.0f,LerpUpScale.y - 9.0f,LerpUpScale.z });
		}
		Skill_Render->BSCControl(1.0f, 0.5f, 0.5f);
		Render_Select->ImageClippingX(ScaleUpTime * 4.0f, ClipXDir::Left);

	}
	else
	{
		ScaleUpTime = 0.0f;
		ScaleDownTime += _Delta;
		float4 LerpDownScale = float4::LerpClamp(UpScale * GameEngineActor::ScreenRatio, DownScale * GameEngineActor::ScreenRatio, ScaleDownTime * 10.0f);
		if (SizeValue == true)
		{
			SkillSelect_Render->GetTransform()->SetLocalScale(LerpDownScale);
			Skill_Render->GetTransform()->SetLocalScale({ LerpDownScale.x - 5.0f,LerpDownScale.y - 9.0f,LerpDownScale.z });
		}
		Render_Select->ImageClippingX(1 - ScaleDownTime * 4.0f, ClipXDir::Left);
		Skill_Render->BSCControl(0.5f, 0.5f, 0.5f);
		if (LerpDownScale == DownScale * GameEngineActor::ScreenRatio)
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
