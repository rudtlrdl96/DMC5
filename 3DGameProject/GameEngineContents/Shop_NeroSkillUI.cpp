#include "PrecompileHeader.h"
#include "Shop_NeroSkillUI.h"
#include "Shop_TitleButton.h"
#include "Shop_ItemButton.h"
#include "Shop_ExplaneUI.h"

#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>

Shop_NeroSkillUI::Shop_NeroSkillUI()
{
}

Shop_NeroSkillUI::~Shop_NeroSkillUI()
{
}

void Shop_NeroSkillUI::Start()
{
	RedQueenButton = GetLevel()->CreateActor<Shop_TitleButton>();
	RedQueenButton->GetTransform()->SetParent(GetTransform());
	RedQueenButton->GetRender()->SetTexture("Shop_RedQueenUI.png");
	RedQueenButton->GetRender()->GetTransform()->SetLocalScale({ 228.0f,62.0f,0.0f });
	RedQueenButton->GetRender()->GetTransform()->SetLocalPosition({ -630.0f,250.0f,0.0f });
	RedQueenButton->GetRender_Bottom()->SetTexture("Shop_SkillUnderBar.png");
	RedQueenButton->GetRender_Bottom()->GetTransform()->SetLocalScale({ 225.0f,28.0f,0.0f });
	RedQueenButton->GetRender_Bottom()->GetTransform()->SetLocalPosition({ -628.0f,205.0f,0.0f });
	RedQueenButton->GetRender_Top()->SetTexture("Shop_SkillTopBar.png");
	RedQueenButton->GetRender_Top()->GetTransform()->SetLocalScale({ 84.0f,23.0f,0.0f });
	RedQueenButton->GetRender_Top()->GetTransform()->SetLocalPosition({ -558.0f,292.0f,0.0f });
	RedQueenButton->GetRender_Select()->SetTexture("Shop_SkillSelect.png");
	RedQueenButton->GetRender_Select()->GetTransform()->SetLocalScale({ 228.0f,62.0f,0.0f });
	RedQueenButton->GetRender_Select()->GetTransform()->SetLocalPosition({ -630.0f,250.0f,0.0f });
	RedQueenButton->SetSelectValue(true);
	RedQueenButton->GetFont()->SetFont("DMC5Font");
	RedQueenButton->SetIsPosValue(true);
	RedQueenButton->GetFont()->GetTransform()->SetLocalPosition({ -680.0f,224.0f,0.0f });
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_0, { ._Name = "STREAK",._Price = "800",._png = "StreakSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_1, { ._Name = "SPLIT",._Price = "1200",._png = "SplitSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png" ,.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_2, { ._Name = "RED QUEEN COMBO C",._Price = "5000",._png = "RedQuennCSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_3, { ._Name = "ROULETTE SPIN",._Price = "7000",._png = "RouletteSpinSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png" ,.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_4, { ._Name = "SHUFLE",._Price = "8000",._png = "ShuffleSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_5, { ._Name = "CALIBUR",._Price = "25000",._png = "CaliburSkill.png", ._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));

	ExplanePtr = GetLevel()->CreateActor<Shop_ExplaneUI>();
	ExplanePtr->GetTransform()->SetParent(GetTransform());
	ExplanePtr->GetTransform()->SetWorldPosition({ 0.0f,0.0f,0.0f });
	SetValue();
}

void Shop_NeroSkillUI::Update(float _Delta)
{
	SetIndex();
	Shop_ItemButton::Skills[PrevIndex]->SetSelectValue(false);
	Shop_ItemButton::Skills[Index]->SetSelectValue(true);
	LerpItem(_Delta);
	SetExText();

}

void Shop_NeroSkillUI::LerpItem(float _Delta)
{
	if (DownSwich == true)
	{
		IsValue = true;
		LerpTime += _Delta;
		Shop_ItemButton::Skills[Index]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_5, Pos_4, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 1]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_4, Pos_3, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 2]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_3, Pos_2, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 3]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_2, Pos_1, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 4]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_1, Pos_0, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 5]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_0, Pos_Null, LerpTime * 5.0f));

		if (Shop_ItemButton::Skills[Index - 5]->GetTransform()->GetLocalPosition().y >= (Pos_Null.y + Pos_0.y) / 2)
		{
			Shop_ItemButton::Skills[Index - 5]->Off();
		}
		if (Shop_ItemButton::Skills[Index]->GetTransform()->GetLocalPosition() == Pos_4)
		{
			DownSwich = false;
			LerpTime = 0.0f;
			IsValue = false;
		}
	}
	if (UpSwich == true)
	{
		IsValue = true;
		LerpTime += _Delta;
		Shop_ItemButton::Skills[Index]->On();
		Shop_ItemButton::Skills[Index]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_Null, Pos_0, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 1]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_0, Pos_1, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 2]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_1, Pos_2, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 3]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_2, Pos_3, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 4]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_3, Pos_4, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 5]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_4, Pos_5, LerpTime * 5.0f));

		if (Shop_ItemButton::Skills[Index]->GetTransform()->GetLocalPosition() == Pos_0)
		{
			IsValue = false;
			UpSwich = false;
			LerpTime = 0.0f;
		}
	}
}

void Shop_NeroSkillUI::SetIndex()
{
	RedQueenButton->SetUIText("RED QUEEN");
	if (IsValue == false && true == GameEngineInput::IsUp("UI_UP"))
	{
		PrevIndex = Index;
		if (Index == 0)
		{
			Index = 5;
			DownSwich = true;
		}
		else
		{
			Index--;
		}
		if (Shop_ItemButton::Skills[1]->GetTransform()->GetLocalPosition() != Pos_1 && PrevIndex == 1 && Index == 0)
		{
			UpSwich = true;
		}


	}
	if (IsValue == false && true == GameEngineInput::IsUp("UI_DOWN"))
	{
		PrevIndex = Index;
		if (Index == 5)
		{
			Index = 0;
			UpSwich = true;
		}
		else
		{
			Index++;
		}
		if (Shop_ItemButton::Skills[5]->GetTransform()->GetLocalPosition() != Pos_4 && PrevIndex == 4 && Index == 5)
		{
			DownSwich = true;
		}


	}
}

void Shop_NeroSkillUI::SetExText()
{
	switch (Index)
	{
	case 0:
		ExplanePtr->SetExplaneText({ ._EName = "STREAK" ,._KName = "스트리크" ,._Explane1 = "멋진 돌진공격으로 주변의",._Explane2 = "적들을 쓸어버립니다.",._Explane3 = "" });
		break;
	case 1:
		ExplanePtr->SetExplaneText({ ._EName = "SPLIT" ,._KName = "스플릿" ,._Explane1 = "적을 무지막지한 공격으로",._Explane2 = "내려쳐 정확히 둘로 갈라버립니다.",._Explane3 = "" });
		break;
	case 2:
		ExplanePtr->SetExplaneText({ ._EName = "RED QUEEN COMBO C" ,._KName = "레드 퀸 콤보 C" ,._Explane1 = "화려한 검 콤보로",._Explane2 = "보다 넓은 범위에 걸쳐",._Explane3 = "적들을 해치웁니다." });
		break;
	case 3:
		ExplanePtr->SetExplaneText({ ._EName = "ROULETTE SPIN" ,._KName = "룰렛 스핀" ,._Explane1 = "공중에서 레드 퀸의 칼날을 회전시켜",._Explane2 = "적을 썰어버립니다.",._Explane3 = "몸이 약간 떠오릅니다." });
		break;
	case 4:
		ExplanePtr->SetExplaneText({ ._EName = "SHUFLE" ,._KName = "셔플" ,._Explane1 = "백스텝으로 공격을 피한 후,",._Explane2 = "앞으로 돌진하면서 빠르고.",._Explane3 = "강력한 카운터를 날립니다." });
		break;
	case 5:
		ExplanePtr->SetExplaneText({ ._EName = "CALIBUR" ,._KName = "칼리버" ,._Explane1 = "공중에서 적에게 돌진해",._Explane2 = "강력한 한방을 날립니다.",._Explane3 = "" });
		break;


	default:
		break;
	}
}

void Shop_NeroSkillUI::SetValue()
{
	for (int i = 0; i < Shop_ItemButton::Skills.size(); i++)
	{
		Shop_ItemButton::Skills[i]->SetSizeValue(true);
	}
}

