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
	Shop_ItemButton::CreateItemUI(GetLevel(), { 0.0f,0.0f,0.0f }, { ._Name = "STRAKE",._Price = "800",._png = "StreakSkill.png", });
	//SkillButton = GetLevel()->CreateActor<Shop_ItemButton>();
	//SkillButton->GetTransform()->SetParent(GetTransform());
	//SkillButton->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
}

void Shop_NeroSkillUI::Update(float _Delta)
{
	RedQueenButton->SetUIText("RED QUEEN");
	if (true == GameEngineInput::IsUp("UIDEBUGMODE"))
	{
		if (Test==false)
		{
			Shop_ItemButton::Items[0]->SetSelectValue(true);
			Test = true;
		}
		else
		{
			Shop_ItemButton::Items[0]->SetSelectValue(false);

			Test = false;
		}
	}
	//SkillButton->SetTextFont({ ._Name = "SPREAK",._Price = "800" });
}

