#include "PrecompileHeader.h"
#include "Nero_ShopTitleBar.h"
#include "Shop_TitleButton.h"
#include "Shop_NeroSkillUI.h"
#include "Shop_ExplaneUI.h"
#include "Shop_ItemButton.h"
#include "Shop_NeroArmUI.h"
#include <GameEngineCore/GameEngineLevel.h>
Nero_ShopTitleBar::Nero_ShopTitleBar()
{
}

Nero_ShopTitleBar::~Nero_ShopTitleBar()
{
}

void Nero_ShopTitleBar::Start()
{
	SkillButton = GetLevel()->CreateActor<Shop_TitleButton>();
	SkillButton->SetUIText("SKILLS");
	SkillButton->GetTransform()->SetLocalPosition({ -600.0f,370.0f,0.0f });
	SkillButton->SetEvent([this]()
		{
			TitleIndex = 0;
		});
	DBButton = GetLevel()->CreateActor<Shop_TitleButton>();
	DBButton->SetUIText("DEVIL BREAKERS");
	DBButton->GetTransform()->SetLocalPosition({ -300.0f,370.0f,0.0f });
	DBButton->SetEvent([this]()
		{
			TitleIndex = 1;
		});
	//Å×½ºÆ®
	SkillScreen = GetLevel()->CreateActor<Shop_NeroSkillUI>();
	SkillScreen->GetTransform()->SetLocalPosition(SkillStartPos);
	ArmScreen = GetLevel()->CreateActor<Shop_NeroArmUI>();
	ArmScreen->GetTransform()->SetLocalPosition(ItemDefaulPos);
}	



void Nero_ShopTitleBar::Update(float _Delta)
{
	LerpScreen(_Delta);
	if (TitleIndex == 0)
	{
		DBButton->SetSelectValue(false);
		SkillButton->SetSelectValue(true);
		SkillScreen->GetExPlane()->On();
		ArmScreen->GetExPlane()->Off();
	}
	else if (TitleIndex == 1)
	{

		SkillScreen->GetExPlane()->Off();
		ArmScreen->GetExPlane()->On();
		SkillButton->SetSelectValue(false);
		DBButton->SetSelectValue(true);
	}
	if (true == GameEngineInput::IsUp("UI_Tab"))
	{
		PrevIndex = TitleIndex;
		if (TitleIndex == 1)
		{
			TitleIndex = 0;
		}
		else
		{
			TitleIndex++;
		}
		if (TitleIndex == 0 && PrevIndex == 1)
		{
			IsValue = true;
			IsValue2 = false;
		}
		else if (PrevIndex == 0 && TitleIndex == 1)
		{
			IsValue2 = true;
			IsValue = false;
		}
	}
}

void Nero_ShopTitleBar::LerpScreen(float _Delta)
{
	if (IsValue==true)
	{
		LerpTime += _Delta;
		if (IsSwichItem == false)
		{
			SkillScreen->GetTransform()->SetLocalPosition(float4::LerpClamp(SkillDefaultPos, SkillStartPos, LerpTime * 2.5f));
			ArmScreen->GetTransform()->SetLocalPosition(float4::LerpClamp(ItemStartPos, ItemEndPos, LerpTime * 2.5f));
			if (ArmScreen->GetTransform()->GetLocalPosition() == ItemEndPos)
			{
				ArmScreen->GetTransform()->SetLocalPosition(ItemDefaulPos);
				IsSwichSkill = false;
				IsSwichItem = true;
				IsValue = false;
				LerpTime = 0.0f;
			}
		}
		
	}
	else if(IsValue2== true)
	{
		LerpTime += _Delta;
		if (IsSwichSkill == false)
		{
			SkillScreen->GetTransform()->SetLocalPosition(float4::LerpClamp(SkillStartPos, SkillEndPos, LerpTime * 2.5f));
			ArmScreen->GetTransform()->SetLocalPosition(float4::LerpClamp(ItemDefaulPos, ItemStartPos, LerpTime * 2.5f));
			if (SkillScreen->GetTransform()->GetLocalPosition() == SkillEndPos)
			{
				SkillScreen->GetTransform()->SetLocalPosition(SkillDefaultPos);
				IsSwichSkill = true;
				IsSwichItem = false;
				IsValue2 = false;
				LerpTime = 0.0f;
			}
		}
		
	}
}

