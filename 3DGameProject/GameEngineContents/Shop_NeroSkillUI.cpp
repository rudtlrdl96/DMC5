#include "PrecompileHeader.h"
#include "Shop_NeroSkillUI.h"
#include "Shop_TitleButton.h"
#include "Shop_ItemButton.h"
#include <GameEngineCore/GameEngineFontRenderer.h>
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
	Shop_ItemButton::CreateItemUI(GetLevel(),Pos_0, { ._Name = "STRAKE",._Price = "800",._png = "StreakSkill.png", });
	Shop_ItemButton::CreateItemUI(GetLevel(),Pos_1, { ._Name = "SPLIT",._Price = "1200",._png = "SplitSkill.png", });
	Shop_ItemButton::CreateItemUI(GetLevel(),Pos_2, { ._Name = "RED QUEEN COMBO C",._Price = "5000",._png = "RedQuennCSkill.png", });
	Shop_ItemButton::CreateItemUI(GetLevel(),Pos_3, { ._Name = "ROULETTE SPIN",._Price = "7000",._png = "RouletteSpinSkill.png", });
	Shop_ItemButton::CreateItemUI(GetLevel(),Pos_4, { ._Name = "SHUFLE",._Price = "8000",._png = "ShuffleSkill.png", });
	Shop_ItemButton::CreateItemUI(GetLevel(),Pos_5, { ._Name = "CALIBUR",._Price = "25000",._png = "CaliburSkill.png", });

}

void Shop_NeroSkillUI::Update(float _Delta)
{
	RedQueenButton->SetUIText("RED QUEEN");
	if (true == GameEngineInput::IsUp("UI_UP"))
	{
		PrevIndex = Index;
		if (Index == 0)
		{
			Index = 5;
		}
		else
		{
			Index--;
		}
		if (PrevIndex == 5 && Index == 4)
		{
			int a = 0;
		}
	}
	if (true == GameEngineInput::IsUp("UI_DOWN"))
	{
		PrevIndex = Index;
		if (Index == 5)
		{
			Index = 0;
		}
		else
		{
			Index++;
		}
		if (PrevIndex == 4 && Index == 5)
		{
			int a = 0;//5는 4로 4는 3으로 3은 2로 2는 1로 1은 0 으로 0은 중간에 사라지기.
			DownSwich = true;
		}
		
	}
	Shop_ItemButton::Items[PrevIndex]->SetSelectValue(false);
	Shop_ItemButton::Items[Index]->SetSelectValue(true);
	if (DownSwich == true)
	{
		LerpTime += _Delta;
		Shop_ItemButton::Items[Index]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_5, Pos_4, LerpTime *3.0f));
		
		Shop_ItemButton::Items[Index-1]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_4, Pos_3, LerpTime * 3.0f));
		
		Shop_ItemButton::Items[Index-2]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_3, Pos_2, LerpTime * 3.0f));

		Shop_ItemButton::Items[Index - 3]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_2, Pos_1, LerpTime * 3.0f));
		
		Shop_ItemButton::Items[Index - 4]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_1, Pos_0, LerpTime * 3.0f));
	
		Shop_ItemButton::Items[Index -5]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_0, Pos_Null, LerpTime * 3.0f));

		if (Shop_ItemButton::Items[Index - 5]->GetTransform()->GetLocalPosition().y == (Pos_Null.y + Pos_0.y)/2)
		{
			Shop_ItemButton::Items[Index - 5]->Off();
		}
	}
}

