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
	FontCreate();
}

void Shop_TitleButton::Update(float _Delta)
{

	if (IsSelect == true)
	{
		AddTime += _Delta;
		Render->On();
		Render_Bottom->On();
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
			//Å©±â 1.1¹è Ä¿Áö±â.
			//Render,Renderbottom render top
			if (IsBlink == true)
			{
				Render_Select->On();
				Render_Select->BSCControl(M0, 0.5f, 0.5f);
				Render->BSCControl(1.0f, 0.7f, 0.5f);
			}
			else
			{
				Render_Select->Off();
				Render->BSCControl(0.5f, 0.5f, 0.5f);

			}


		}

	}
	else
	{

		if (IsPosValue == false)
		{
			FontRender->GetTransform()->SetLocalPosition({ 0.0f,19.f,0.0f });
			Render->Off();
		}
		else
		{
			Render_Select->Off();
			Render_Bottom->Off();
			Render->BSCControl(0.3f, 0.3f, 0.5f);

		}
		FontRender->SetColor(float4(0.462f, 0.58f, 0.576f));

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




void Shop_TitleButton::SetTextFont(std::string_view _Text)
{
	FontRender->SetText(_Text);
}


