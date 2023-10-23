#include "PrecompileHeader.h"
#include "Char_ChoiceUI.h"
#include "UIEnums.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
Char_ChoiceUI* Char_ChoiceUI::CharChoicePtr = nullptr;
ChoicePlayerType Char_ChoiceUI::Type = ChoicePlayerType::NONE;
Char_ChoiceUI::Char_ChoiceUI()
{
	CharChoicePtr = this;
}

Char_ChoiceUI::~Char_ChoiceUI()
{

}

void Char_ChoiceUI::Start()
{
	WindowRatio = GameEngineActor::ScreenRatio;
	Nero_ChoiceUIRender = GetLevel()->CreateActor<Char_ChoiceButton>();
	Nero_ChoiceUIRender->GetTransform()->SetLocalPosition(NeroPos * WindowRatio);
	Nero_ChoiceUIRender->GetCharRender()->SetTexture("Character_Nero.png");
	Nero_ChoiceUIRender->GetTextNoneRender()->GetTransform()->SetLocalPosition(TextScale* WindowRatio);
	Nero_ChoiceUIRender->GetTextSelectRender()->GetTransform()->SetLocalPosition(TextScale * WindowRatio);
	Nero_ChoiceUIRender->SetEvent([this]()
		{
			Type = ChoicePlayerType::NERO;
			GameEngineCore::ChangeLevel("MainMenuLevel");
			if (nullptr != CharTypeCallBack)
			{
				CharTypeCallBack();
			}
		});
	Nero_ChoiceUIRender->SetUIText({ ._Text = "NERO",._SeletText = "",
	._ExplaneText = "The Devil Hunter" });
	Vergil_ChoiceUIRender = GetLevel()->CreateActor<Char_ChoiceButton>();
	Vergil_ChoiceUIRender->GetTransform()->SetLocalPosition(VergilPos * WindowRatio);
	Vergil_ChoiceUIRender->GetCharRender()->SetTexture("Character_Vergil.png");
	Vergil_ChoiceUIRender->GetTextNoneRender()->GetTransform()->SetLocalPosition(TextScale * WindowRatio);
	Vergil_ChoiceUIRender->GetTextSelectRender()->GetTransform()->SetLocalPosition(TextScale * WindowRatio);
	Vergil_ChoiceUIRender->SetUIText({ ._Text = "VERGIL",._SeletText = "",
	._ExplaneText = "The Alpha and the Omega" });
	Vergil_ChoiceUIRender->SetEvent([this]()
		{
			Type = ChoicePlayerType::VERGIL;
			GameEngineCore::ChangeLevel("MainMenuLevel");
			if (nullptr != CharTypeCallBack)
			{
				CharTypeCallBack();
			}
		});
}


void Char_ChoiceUI::Update(float _DeltaTime)
{
	SetButtonIndex();
	if (GameEngineInput::IsDown("UI_Left"))
	{
		if (SelectIndex == 1)
		{
			SelectIndex = 0;
		}
		else
		{
			SelectIndex++;
		}
	}
	if (GameEngineInput::IsDown("UI_Right"))
	{
		if (SelectIndex == 0)
		{
			SelectIndex = 1;
		}
		else
		{
			SelectIndex--;
		}
	}
}

void Char_ChoiceUI::SetButtonIndex()
{
	if (true == Nero_ChoiceUIRender->GetIsValue())
	{
		SelectIndex = 0;
		Nero_ChoiceUIRender->SetIsValue(false);
	}
	if (true == Vergil_ChoiceUIRender->GetIsValue())
	{
		SelectIndex = 1;
		Vergil_ChoiceUIRender->SetIsValue(false);
	}
	if (SelectIndex == 0)
	{
		Nero_ChoiceUIRender->SetSelectValue(true);
	}
	else
	{
		Nero_ChoiceUIRender->SetSelectValue(false);
	}
	if (SelectIndex == 1)
	{
		Vergil_ChoiceUIRender->SetSelectValue(true);
	}
	else
	{
		Vergil_ChoiceUIRender->SetSelectValue(false);
	}
}

