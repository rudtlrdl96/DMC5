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
	ContinueButton->GetTransform()->SetLocalPosition(Pos_1*GameEngineActor::ScreenRatio);
	ContinueButton->GetRender()->SetTexture("NullTexture.png");
	ContinueButton->GetRender()->GetTransform()->SetLocalScale(Scale*GameEngineActor::ScreenRatio);
	ContinueButton->GetRender_Select()->SetTexture("SeletBaseHalf0.png");
	ContinueButton->GetRender_Select()->GetTransform()->SetLocalScale(BarScale* GameEngineActor::ScreenRatio);
	ContinueButton->GetRender_Select()->GetTransform()->SetLocalPosition(BarLeftPos* GameEngineActor::ScreenRatio);
	ContinueButton->GetRender_Select2()->SetTexture("SeletBaseHalf1.png");
	ContinueButton->GetRender_Select2()->GetTransform()->SetLocalScale(BarScale* GameEngineActor::ScreenRatio);
	ContinueButton->GetRender_Select2()->GetTransform()->SetLocalPosition(BarRightPos* GameEngineActor::ScreenRatio);
	ContinueButton->GetRender_Enter()->SetTexture("T_EnterBase.png");
	ContinueButton->GetRender_Enter()->GetTransform()->SetLocalScale(BarScale* GameEngineActor::ScreenRatio);
	ContinueButton->GetRender_Enter()->GetTransform()->SetLocalPosition(BarLeftPos* GameEngineActor::ScreenRatio);
	ContinueButton->GetRender_Enter2()->SetTexture("T_EnterBase.png");
	ContinueButton->GetRender_Enter2()->GetTransform()->SetLocalScale(BarScale* GameEngineActor::ScreenRatio);
	ContinueButton->GetRender_Enter2()->GetTransform()->SetLocalPosition(BarRightPos* GameEngineActor::ScreenRatio);
	ContinueButton->SetExplanePos(ExplanePos);
	ContinueButton->SetEvent([this]()
		{			
			GameEngineSound::Play("StartButtonSelect.wav");
			ContinueValue = true;
		});
	OptionButton = GetLevel()->CreateActor<UIButton>();
	OptionButton->GetTransform()->SetLocalPosition(Pos_2 * GameEngineActor::ScreenRatio);
	OptionButton->GetRender()->SetTexture("NullTexture.png");
	OptionButton->GetRender()->GetTransform()->SetLocalScale(Scale * GameEngineActor::ScreenRatio);
	OptionButton->GetRender_Select()->SetTexture("SeletBaseHalf0.png");
	OptionButton->GetRender_Select()->GetTransform()->SetLocalScale(BarScale * GameEngineActor::ScreenRatio);
	OptionButton->GetRender_Select()->GetTransform()->SetLocalPosition(BarLeftPos * GameEngineActor::ScreenRatio);
	OptionButton->GetRender_Select2()->SetTexture("SeletBaseHalf1.png");
	OptionButton->GetRender_Select2()->GetTransform()->SetLocalScale(BarScale * GameEngineActor::ScreenRatio);
	OptionButton->GetRender_Select2()->GetTransform()->SetLocalPosition(BarRightPos * GameEngineActor::ScreenRatio);
	OptionButton->SetExplanePos(ExplanePos);
	OptionButton->SetEvent([this]()
		{
			GameEngineSound::Play("UIButtonSelectSound.wav");
		});
	ExitButton = GetLevel()->CreateActor<UIButton>();
	ExitButton->GetTransform()->SetLocalPosition(Pos_3 * GameEngineActor::ScreenRatio);
	ExitButton->GetRender()->GetTransform()->SetLocalScale(Scale * GameEngineActor::ScreenRatio);
	ExitButton->GetRender()->SetTexture("NullTexture.png");
	ExitButton->GetRender_Select()->SetTexture("SeletBaseHalf0.png");
	ExitButton->GetRender_Select()->GetTransform()->SetLocalScale(BarScale * GameEngineActor::ScreenRatio);
	ExitButton->GetRender_Select()->GetTransform()->SetLocalPosition(BarLeftPos * GameEngineActor::ScreenRatio);
	ExitButton->GetRender_Select2()->SetTexture("SeletBaseHalf1.png");
	ExitButton->GetRender_Select2()->GetTransform()->SetLocalScale(BarScale * GameEngineActor::ScreenRatio);
	ExitButton->GetRender_Select2()->GetTransform()->SetLocalPosition(BarRightPos * GameEngineActor::ScreenRatio);
	ExitButton->SetExplanePos(ExplanePos);
	ExitButton->SetEvent([this]()
		{
			GameEngineSound::Play("UIButtonSelectSound.wav");
		});
	SetFontText();

}

void Title_SelectScreen::Update(float _DeltaTime)
{
	SetButtonIndex();
	if (GameEngineInput::IsDown("UI_UP"))
	{
		GameEngineSound::Play("ButtonSound.wav");
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
		GameEngineSound::Play("ButtonSound.wav");
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

