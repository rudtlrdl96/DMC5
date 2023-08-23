#include "PrecompileHeader.h"
#include "Char_ChoiceUI.h"
#include "UIEnums.h"
#include <GameEngineCore/GameEngineUIRenderer.h>

Char_ChoiceUI::Char_ChoiceUI()
{
}

Char_ChoiceUI::~Char_ChoiceUI()
{
}

void Char_ChoiceUI::Start()
{
	Nero_ChoiceUIRender = GetLevel()->CreateActor<Char_ChoiceButton>();
	Nero_ChoiceUIRender->GetTransform()->SetLocalPosition(NeroPos);
	Nero_ChoiceUIRender->GetCharRender()->SetTexture("Character_Nero.png");
	Nero_ChoiceUIRender->GetTextNoneRender()->GetTransform()->SetLocalPosition({20.0f,-150.0f,0.0f});
	Nero_ChoiceUIRender->GetTextSelectRender()->GetTransform()->SetLocalPosition({ 20.0f,-150.0f,0.0f });
	Nero_ChoiceUIRender->SetUIText({ ._Text = "NERO",._SeletText = "",
	._ExplaneText = "The Devil Hunter" });
	Vergil_ChoiceUIRender = GetLevel()->CreateActor<Char_ChoiceButton>();
	Vergil_ChoiceUIRender->GetTransform()->SetLocalPosition(VergilPos);
	Vergil_ChoiceUIRender->GetCharRender()->SetTexture("Character_Vergil.png");
	Vergil_ChoiceUIRender->GetTextNoneRender()->GetTransform()->SetLocalPosition({ 20.0f,-150.0f,0.0f });
	Vergil_ChoiceUIRender->GetTextSelectRender()->GetTransform()->SetLocalPosition({ 20.0f,-150.0f,0.0f });
	Vergil_ChoiceUIRender->SetUIText({ ._Text = "VERGIL",._SeletText = "",
	._ExplaneText = "The Alpha and the Omega" });

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

