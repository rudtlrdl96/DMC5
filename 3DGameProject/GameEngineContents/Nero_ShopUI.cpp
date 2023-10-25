#include "PrecompileHeader.h"
#include "Nero_ShopUI.h"
#include "Shop_TitleButton.h"
#include "Shop_NeroSkillUI.h"
#include "Shop_ExplaneUI.h"
#include "Shop_ItemButton.h"
#include "Shop_NeroArmUI.h"
#include <GameEngineCore/GameEngineLevel.h>
Nero_ShopUI::Nero_ShopUI()
{
}

Nero_ShopUI::~Nero_ShopUI()
{
}

void Nero_ShopUI::Start()
{
	SkillButton = GetLevel()->CreateActor<Shop_TitleButton>();
	SkillButton->GetTransform()->SetParent(GetTransform());
	SkillButton->SetUIText("SKILLS");
	SkillButton->GetTransform()->SetLocalPosition(float4{ -600.0f,370.0f,0.0f } * GameEngineActor::ScreenRatio);
	SkillButton->SetEvent([this]()
		{
			TitleIndex = 0;
		});
	DBButton = GetLevel()->CreateActor<Shop_TitleButton>();
	DBButton->SetUIText("DEVIL BREAKERS");
	DBButton->GetTransform()->SetLocalPosition(float4{ -300.0f,370.0f,0.0f }*GameEngineActor::ScreenRatio);
	DBButton->GetTransform()->SetParent(GetTransform());
	DBButton->SetEvent([this]()
		{
			TitleIndex = 1;
		});
	SkillScreen = GetLevel()->CreateActor<Shop_NeroSkillUI>();
	SkillScreen->GetTransform()->SetLocalPosition(SkillStartPos * GameEngineActor::ScreenRatio);
	SkillScreen->GetTransform()->SetParent(GetTransform());
	ArmScreen = GetLevel()->CreateActor<Shop_NeroArmUI>();
	ArmScreen->GetTransform()->SetLocalPosition(ItemDefaulPos * GameEngineActor::ScreenRatio);
	ArmScreen->GetTransform()->SetParent(GetTransform());
}	



void Nero_ShopUI::Update(float _Delta)
{
	if (true == GameEngineInput::IsUp("UI_ESC"))
	{
		Off();
	}
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
	if (true == GameEngineInput::IsUp("UI_Tab")&& IsSwichValue==true)
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

void Nero_ShopUI::LerpScreen(float _Delta)
{
	if (IsValue==true)
	{
		IsSwichValue = false;
		LerpTime += _Delta;
		if (IsSwichItem == false)
		{
			SkillScreen->On();
			IsSwichValue = false;
			SkillScreen->GetTransform()->SetLocalPosition(float4::LerpClamp(SkillDefaultPos, SkillStartPos, LerpTime * 2.5f));
			ArmScreen->GetTransform()->SetLocalPosition(float4::LerpClamp(ItemStartPos, ItemEndPos, LerpTime * 2.5f));
			if (ArmScreen->GetTransform()->GetLocalPosition() == ItemEndPos)
			{
				ArmScreen->Off();
				ArmScreen->GetTransform()->SetLocalPosition(ItemDefaulPos);
				IsSwichSkill = false;
				IsSwichItem = true;
				IsValue = false;
				LerpTime = 0.0f;
				IsSwichValue = true;
			}
		}
		
	}
	else if(IsValue2== true)
	{
		LerpTime += _Delta;
		if (IsSwichSkill == false)
		{
			ArmScreen->On();
			IsSwichValue = false;
			SkillScreen->GetTransform()->SetLocalPosition(float4::LerpClamp(SkillStartPos, SkillEndPos, LerpTime * 2.5f));
			ArmScreen->GetTransform()->SetLocalPosition(float4::LerpClamp(ItemDefaulPos, ItemStartPos, LerpTime * 2.5f));
			if (SkillScreen->GetTransform()->GetLocalPosition() == SkillEndPos)
			{
				SkillScreen->Off();
				SkillScreen->GetTransform()->SetLocalPosition(SkillDefaultPos);
				IsSwichSkill = true;
				IsSwichItem = false;
				IsValue2 = false;
				LerpTime = 0.0f;
				IsSwichValue = true;
			}
		}
		
	}
}

