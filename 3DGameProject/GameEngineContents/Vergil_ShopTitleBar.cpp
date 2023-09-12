#include "PrecompileHeader.h"
#include "Vergil_ShopTitleBar.h"
#include "Shop_TitleButton.h"
#include "Shop_ExplaneUI.h"
#include "Shop_ItemButton.h"
#include "Shop_VergilYamatoUI.h"
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
Vergil_ShopTitleBar* Vergil_ShopTitleBar::Vergil_ShopBar = nullptr;
Vergil_ShopTitleBar::Vergil_ShopTitleBar()
{
	Vergil_ShopBar = this;
}

Vergil_ShopTitleBar::~Vergil_ShopTitleBar()
{
}

void Vergil_ShopTitleBar::Start()
{
	SkillButton = GetLevel()->CreateActor<Shop_TitleButton>();
	SkillButton->SetUIText("SKILLS");
	SkillButton->GetTransform()->SetLocalPosition({ -600.0f,370.0f,0.0f });
	YamatoButton = GetLevel()->CreateActor<Shop_TitleButton>();
	YamatoButton->GetTransform()->SetParent(GetTransform());
	YamatoButton->GetRender()->SetTexture("YamatoBase.png");
	YamatoButton->GetRender()->GetTransform()->SetLocalScale({ 228.0f,62.0f,0.0f });
	YamatoButton->GetRender()->GetTransform()->SetLocalPosition({ -630.0f,250.0f,0.0f });
	YamatoButton->GetRender_Bottom()->SetTexture("Shop_SkillUnderBar.png");
	YamatoButton->GetRender_Bottom()->GetTransform()->SetLocalScale({ 225.0f,28.0f,0.0f });
	YamatoButton->GetRender_Bottom()->GetTransform()->SetLocalPosition({ -628.0f,205.0f,0.0f });
	YamatoButton->GetRender_Top()->SetTexture("Shop_SkillTopBar.png");
	YamatoButton->GetRender_Top()->GetTransform()->SetLocalScale({ 84.0f,23.0f,0.0f });
	YamatoButton->GetRender_Top()->GetTransform()->SetLocalPosition({ -558.0f,292.0f,0.0f });
	YamatoButton->GetRender_Select()->SetTexture("Shop_SkillSelect.png");
	YamatoButton->GetRender_Select()->GetTransform()->SetLocalScale({ 228.0f,62.0f,0.0f });
	YamatoButton->GetRender_Select()->GetTransform()->SetLocalPosition({ -630.0f,250.0f,0.0f });
	YamatoButton->GetFont()->SetFont("DMC5Font");
	YamatoButton->GetFont()->SetFontFlag(FW1_LEFT);
	YamatoButton->SetIsPosValue(true);
	YamatoButton->GetFont()->GetTransform()->SetLocalPosition({ -736.0f,222.0f,0.0f });

	MirgeButton = GetLevel()->CreateActor<Shop_TitleButton>();
	MirgeButton->GetTransform()->SetParent(GetTransform());
	MirgeButton->GetTransform()->SetLocalPosition({ 0.0f,-145.0f,0.0f });
	MirgeButton->GetRender()->SetTexture("MirgeBladeBase.png");
	MirgeButton->GetRender()->GetTransform()->SetLocalScale({ 228.0f,62.0f,0.0f });
	MirgeButton->GetRender()->GetTransform()->SetLocalPosition({ -630.0f,250.0f,0.0f });
	MirgeButton->GetRender_Bottom()->SetTexture("Shop_SkillUnderBar.png");
	MirgeButton->GetRender_Bottom()->GetTransform()->SetLocalScale({ 225.0f,28.0f,0.0f });
	MirgeButton->GetRender_Bottom()->GetTransform()->SetLocalPosition({ -628.0f,205.0f,0.0f });
	MirgeButton->GetRender_Top()->SetTexture("Shop_SkillTopBar.png");
	MirgeButton->GetRender_Top()->GetTransform()->SetLocalScale({ 84.0f,23.0f,0.0f });
	MirgeButton->GetRender_Top()->GetTransform()->SetLocalPosition({ -558.0f,292.0f,0.0f });
	MirgeButton->GetRender_Select()->SetTexture("Shop_SkillSelect.png");
	MirgeButton->GetRender_Select()->GetTransform()->SetLocalScale({ 228.0f,62.0f,0.0f });
	MirgeButton->GetRender_Select()->GetTransform()->SetLocalPosition({ -630.0f,250.0f,0.0f });
	MirgeButton->GetFont()->SetFont("DMC5Font");
	MirgeButton->GetFont()->SetFontFlag(FW1_LEFT);
	MirgeButton->SetIsPosValue(true);
	MirgeButton->GetFont()->GetTransform()->SetLocalPosition({ -736.0f,222.0f,0.0f });

	YamatoSkill = GetLevel()->CreateActor<Shop_VergilYamatoUI>();
	YamatoSkill->Off();
}



void Vergil_ShopTitleBar::Update(float _Delta)
{

	SkillButton->SetSelectValue(true);
	YamatoButton->SetUIText("YAMATO");
	MirgeButton->SetUIText("MIRGE BLADE");
	SetIndexCount();
	if (ButtonIndex == 0)
	{
		ScaleUpDown(_Delta, YamatoButton);
		YamatoButton->SetSelectValue(true);
		MirgeButton->SetSelectValue(false);
		YamatoSkill->On();
		for (int i = 0; i < Shop_ItemButton::Skills.size(); i++)
		{
			Shop_ItemButton::Skills[i]->On();
		}
	}
	else
	{
		ScaleUpDown(_Delta, MirgeButton);
		MirgeButton->SetSelectValue(true);
		YamatoButton->SetSelectValue(false);
		YamatoSkill->Off();
		for (int i = 0; i < Shop_ItemButton::Skills.size(); i++)
		{
			Shop_ItemButton::Skills[i]->Off();
		}
	}
	if (Index == 1)
	{
		YamatoButton->SetBlinkValue(false);
		MirgeButton->SetBlinkValue(false);

	}
	else
	{
		YamatoButton->SetBlinkValue(true);
		MirgeButton->SetBlinkValue(true);
	}
}

void Vergil_ShopTitleBar::SetIndexCount()
{
	if (Index == 0 && true == GameEngineInput::IsUp("UI_UP"))
	{
		ScaleValue = false;
		if (ButtonIndex == 0)
		{
			ButtonIndex = 1;
		}
		else
		{
			ButtonIndex--;

		}

	}
	if (Index == 0 && true == GameEngineInput::IsUp("UI_DOWN"))
	{
		ScaleValue = false;
		if (ButtonIndex == 1)
		{
			ButtonIndex = 0;
		}
		else
		{
			ButtonIndex++;
		}
	}
	if (true == GameEngineInput::IsUp("UI_RIGHT"))
	{
		if (Index == 1)
		{
			Index = 1;
		}
		else
		{
			Index++;
		}

	}
	if (true == GameEngineInput::IsUp("UI_LEFT"))
	{
		if (Index == 0)
		{
			Index = 0;
		}
		else
		{
			Index--;
		}
	}
}

void Vergil_ShopTitleBar::ScaleUpDown(float _Delta , std::shared_ptr<Shop_TitleButton> _Button)
{
	LerpTime += _Delta;
	if (ScaleValue == false)
	{
		_Button->GetRender_Select()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderScale, RenderScale * 1.05f, LerpTime * 5.0f));
		_Button->GetRender()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderScale, RenderScale * 1.05f, LerpTime * 5.0f));
		_Button->GetRender_Top()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderTopScale, RenderTopScale * 1.05f, LerpTime * 5.0f));
		_Button->GetRender_Bottom()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderBottomScale, RenderBottomScale * 1.05f, LerpTime * 5.0f));

	}

	if (ScaleValue == false && _Button->GetRender()->GetTransform()->GetLocalScale() == float4(RenderScale * 1.05f))
	{
		ScaleValue = true;
		LerpTime = 0.0f;
	}
	if (ScaleValue == true)
	{
		_Button->GetRender_Select()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderScale * 1.05f, RenderScale, LerpTime * 5.0f));
		_Button->GetRender()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderScale * 1.05f, RenderScale, LerpTime * 5.0f));
		_Button->GetRender_Top()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderTopScale * 1.05f, RenderTopScale, LerpTime * 5.0f));
		_Button->GetRender_Bottom()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderBottomScale * 1.05f, RenderBottomScale, LerpTime * 5.0f));
	}
}
