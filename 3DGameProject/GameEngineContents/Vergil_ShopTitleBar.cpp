#include "PrecompileHeader.h"
#include "Vergil_ShopTitleBar.h"
#include "Shop_TitleButton.h"
#include "Shop_ExplaneUI.h"
#include "Shop_ItemButton.h"
#include "Shop_VergilYamatoUI.h"
#include <GameEngineCore/GameEngineLevel.h>
Vergil_ShopTitleBar::Vergil_ShopTitleBar()
{
}

Vergil_ShopTitleBar::~Vergil_ShopTitleBar()
{
}

void Vergil_ShopTitleBar::Start()
{
	SkillButton = GetLevel()->CreateActor<Shop_TitleButton>();
	SkillButton->SetUIText("SKILLS");
	SkillButton->GetTransform()->SetLocalPosition({ -600.0f,370.0f,0.0f });
	SkillButton->SetEvent([this]()
		{
			TitleIndex = 0;
		});

	YamatoScreen = GetLevel()->CreateActor<Shop_VergilYamatoUI>();
	YamatoScreen->GetTransform()->SetLocalPosition(float4::ZERONULL);
}



void Vergil_ShopTitleBar::Update(float _Delta)
{

		SkillButton->SetSelectValue(true);

	
}
