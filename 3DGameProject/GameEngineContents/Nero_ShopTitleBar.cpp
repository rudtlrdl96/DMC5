#include "PrecompileHeader.h"
#include "Nero_ShopTitleBar.h"
#include "Shop_TitleButton.h"
#include "Shop_NeroSkillUI.h"

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

	std::shared_ptr<Shop_NeroSkillUI> RQButton = GetLevel()->CreateActor<Shop_NeroSkillUI>();
	RQButton->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });

}

void Nero_ShopTitleBar::Update(float _Delta)
{
	if (TitleIndex == 0)
	{
		DBButton->SetSelectValue(false);
		SkillButton->SetSelectValue(true);
	}
	else if (TitleIndex == 1)
	{
		SkillButton->SetSelectValue(false);
		DBButton->SetSelectValue(true);
	}
	if (true == GameEngineInput::IsUp("UI_Tab"))
	{
		if (TitleIndex == 1)
		{
			TitleIndex = 0;
		}
		else
		{
			TitleIndex++;
		}
	}
}

