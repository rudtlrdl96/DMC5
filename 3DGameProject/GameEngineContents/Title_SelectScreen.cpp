#include "PrecompileHeader.h"
#include "Title_SelectScreen.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
Title_SelectScreen::Title_SelectScreen()
{
}

Title_SelectScreen::~Title_SelectScreen()
{
}
void Title_SelectScreen::Start()
{
	ContinueButton = GetLevel()->CreateActor<UIButton>();
	ContinueButton->GetTransform()->SetLocalPosition({ 0.0f,-150.0f,0.0f });
	ContinueButton->GetRender()->SetTexture("NullTexture.png");
	ContinueButton->GetRender()->GetTransform()->SetLocalScale({ 200.0f,40.0f,0.0f });
	ContinueButton->GetRender_Select()->SetTexture("SeletBaseHalf0.png");
	ContinueButton->GetRender_Select()->GetTransform()->SetLocalScale({ 800.0f,50.0f,0.0f });
	ContinueButton->GetRender_Select()->GetTransform()->SetLocalPosition({ -400.0f,0.0f,0.0f });
	ContinueButton->GetRender_Select2()->SetTexture("SeletBaseHalf1.png");
	ContinueButton->GetRender_Select2()->GetTransform()->SetLocalScale({ 800.0f,50.0f,0.0f });
	ContinueButton->GetRender_Select2()->GetTransform()->SetLocalPosition({ 400.0f,0.0f,0.0f });
	ContinueButton->GetRender_Enter()->SetTexture("T_EnterBase.png");
	ContinueButton->GetRender_Enter()->GetTransform()->SetLocalScale({ 800.0f,50.0f,0.0f });
	ContinueButton->GetRender_Enter()->GetTransform()->SetLocalPosition({ -400.0f,0.0f,0.0f });
	ContinueButton->GetRender_Enter2()->SetTexture("T_EnterBase.png");
	ContinueButton->GetRender_Enter2()->GetTransform()->SetLocalScale({ 800.0f,50.0f,0.0f });
	ContinueButton->GetRender_Enter2()->GetTransform()->SetLocalPosition({ 400.0f,0.0f,0.0f });
	ContinueButton->SetExplanePos({ 0.0f, -300.f, 0.0f });
	ContinueButton->SetEvent([this]()
		{
			ContinueValue = true;
		});
	OptionButton = GetLevel()->CreateActor<UIButton>();
	OptionButton->GetTransform()->SetLocalPosition({ 0.0f,-200.0f,0.0f });
	OptionButton->GetRender()->SetTexture("NullTexture.png");
	OptionButton->GetRender()->GetTransform()->SetLocalScale({ 200.0f,40.0f,0.0f });
	OptionButton->GetRender_Select()->SetTexture("SeletBaseHalf0.png");
	OptionButton->GetRender_Select()->GetTransform()->SetLocalScale({800.0f,50.0f,0.0f });
	OptionButton->GetRender_Select()->GetTransform()->SetLocalPosition({ -400.0f,0.0f,0.0f });
	OptionButton->GetRender_Select2()->SetTexture("SeletBaseHalf1.png");
	OptionButton->GetRender_Select2()->GetTransform()->SetLocalScale({ 800.0f,50.0f,0.0f });
	OptionButton->GetRender_Select2()->GetTransform()->SetLocalPosition({ 400.0f,0.0f,0.0f });
	OptionButton->SetExplanePos({ 0.0f, -300.f, 0.0f });

	ExitButton = GetLevel()->CreateActor<UIButton>();
	ExitButton->GetTransform()->SetLocalPosition({ 0.0f,-250.0f,0.0f });
	ExitButton->GetRender()->GetTransform()->SetLocalScale({ 200.0f,40.0f,0.0f });
	ExitButton->GetRender()->SetTexture("NullTexture.png");
	ExitButton->GetRender_Select()->SetTexture("SeletBaseHalf0.png");
	ExitButton->GetRender_Select()->GetTransform()->SetLocalScale({ 800.0f,50.0f,0.0f });
	ExitButton->GetRender_Select()->GetTransform()->SetLocalPosition({ -400.0f,0.0f,0.0f });
	ExitButton->GetRender_Select2()->SetTexture("SeletBaseHalf1.png");
	ExitButton->GetRender_Select2()->GetTransform()->SetLocalScale({ 800.0f,50.0f,0.0f });
	ExitButton->GetRender_Select2()->GetTransform()->SetLocalPosition({ 400.0f,0.0f,0.0f });
	ExitButton->SetExplanePos({ 0.0f, -300.f, 0.0f });

	SetFontText();

}

void Title_SelectScreen::Update(float _DeltaTime)
{
	SetButtonIndex();
	if (GameEngineInput::IsDown("UI_UP"))
	{
		if (SelectValue == 0)
		{
			SelectValue = 2;
		}
		else
		{
			SelectValue--;
		}
	}
	if (GameEngineInput::IsDown("UI_Down"))
	{
		if (SelectValue == 2)
		{
			SelectValue = 0;
		}
		else
		{
			SelectValue++;
		}
	}

	if (ContinueValue == true)
	{
		EnterTime += _DeltaTime;
		ContinueButton->GetRender_Enter2()->ImageClippingX(EnterTime * 4.0f, ClipXDir::Left);
		ContinueButton->GetRender_Enter()->ImageClippingX(EnterTime * 4.0f, ClipXDir::Right);
		if (EnterTime * 4.0f > 1.0f)
		{
			GameEngineCore::ChangeLevel("Char_ChoiceLevel");
			ContinueValue = false;
			EnterTime = 0.0f;
		}
	}

}

void Title_SelectScreen::SetFontText()
{
	ContinueButton->SetUIText({ ._Text = "MAIN MENU",._SeletText = "MAIN MENU",
	._ExplaneText = "메인 메뉴로 이동합니다." });

	OptionButton->SetUIText({ ._Text = "OPTION",._SeletText = "OPTION",._ExplaneText = "설정으로 이동합니다."});

	ExitButton->SetUIText({ ._Text = "EXIT",._SeletText = "EXIT",._ExplaneText = "게임을 종료합니다." });
}

void Title_SelectScreen::SetButtonIndex()
{
	if (true == ContinueButton->GetIsValue())
	{
		SelectValue = 0;
		ContinueButton->SetIsValue(false);
	}
	if (true == OptionButton->GetIsValue())
	{
		SelectValue = 1;
		OptionButton->SetIsValue(false);
	}
	if (true == ExitButton->GetIsValue())
	{
		SelectValue = 2;
		ExitButton->SetIsValue(false);
	}
	if (SelectValue == 0)
	{
		ContinueButton->SetSelectValue(true);
	}
	else
	{
		ContinueButton->SetSelectValue(false);
	}
	if (SelectValue == 1)
	{
		OptionButton->SetSelectValue(true);
	}
	else
	{
		OptionButton->SetSelectValue(false);
	}
	if (SelectValue == 2)
	{
		ExitButton->SetSelectValue(true);
	}
	else
	{
		ExitButton->SetSelectValue(false);
	}
}

