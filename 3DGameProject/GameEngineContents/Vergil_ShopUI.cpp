#include "PrecompileHeader.h"
#include "Vergil_ShopUI.h"
#include "Shop_TitleButton.h"
#include "Shop_ExplaneUI.h"
#include "Shop_ItemButton.h"
#include "Shop_VergilYamatoUI.h"
#include "Shop_VergilMirgeUI.h"
#include "Shop_EnterWindow.h"
#include "RedOrbUI.h"
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
Vergil_ShopUI* Vergil_ShopUI::Vergil_ShopBar = nullptr;
Vergil_ShopUI::Vergil_ShopUI()
{
	Vergil_ShopBar = this;
}

Vergil_ShopUI::~Vergil_ShopUI()
{
}

void Vergil_ShopUI::Start()
{
	EnterWindow = GetLevel()->CreateActor<Shop_EnterWindow>();
	EnterWindow->GetTransform()->SetParent(GetTransform());
	SkillButton = GetLevel()->CreateActor<Shop_TitleButton>();
	SkillButton->GetTransform()->SetParent(GetTransform());
	SkillButton->SetUIText("SKILLS");
	SkillButton->GetTransform()->SetLocalPosition(float4{ -600.0f,370.0f,0.0f }*GameEngineActor::ScreenRatio);
	YamatoButton = GetLevel()->CreateActor<Shop_TitleButton>();
	YamatoButton->GetTransform()->SetParent(GetTransform());
	YamatoButton->GetRender()->SetTexture("YamatoBase.png");
	YamatoButton->GetRender()->GetTransform()->SetLocalScale(float4{ 228.0f,62.0f,0.0f }*GameEngineActor::ScreenRatio);
	YamatoButton->GetRender()->GetTransform()->SetLocalPosition(float4{ -630.0f,250.0f,0.0f }*GameEngineActor::ScreenRatio);
	YamatoButton->GetRender_Bottom()->SetTexture("Shop_SkillUnderBar.png");
	YamatoButton->GetRender_Bottom()->GetTransform()->SetLocalScale(float4{ 225.0f,28.0f,0.0f }*GameEngineActor::ScreenRatio);
	YamatoButton->GetRender_Bottom()->GetTransform()->SetLocalPosition(float4{ -628.0f,205.0f,0.0f }*GameEngineActor::ScreenRatio);
	YamatoButton->GetRender_Top()->SetTexture("Shop_SkillTopBar.png");
	YamatoButton->GetRender_Top()->GetTransform()->SetLocalScale(float4{ 84.0f,23.0f,0.0f }*GameEngineActor::ScreenRatio);
	YamatoButton->GetRender_Top()->GetTransform()->SetLocalPosition(float4{ -558.0f,292.0f,0.0f }*GameEngineActor::ScreenRatio);
	YamatoButton->GetRender_Select()->SetTexture("Shop_SkillSelect.png");
	YamatoButton->GetRender_Select()->GetTransform()->SetLocalScale(float4{ 228.0f,62.0f,0.0f }*GameEngineActor::ScreenRatio);
	YamatoButton->GetRender_Select()->GetTransform()->SetLocalPosition(float4{ -630.0f,250.0f,0.0f }*GameEngineActor::ScreenRatio);
	YamatoButton->GetFont()->SetFont("DMC5Font");
	YamatoButton->GetFont()->SetFontFlag(FW1_LEFT);
	YamatoButton->SetIsPosValue(true);
	YamatoButton->GetFont()->GetTransform()->SetLocalPosition(float4{ -736.0f,222.0f,0.0f }*GameEngineActor::ScreenRatio);

	MirgeButton = GetLevel()->CreateActor<Shop_TitleButton>();
	MirgeButton->GetTransform()->SetParent(GetTransform());
	MirgeButton->GetTransform()->SetLocalPosition(float4{ 0.0f,-145.0f,0.0f }*GameEngineActor::ScreenRatio);
	MirgeButton->GetRender()->SetTexture("MirgeBladeBase.png");
	MirgeButton->GetRender()->GetTransform()->SetLocalScale(float4{ 228.0f,62.0f,0.0f }*GameEngineActor::ScreenRatio);
	MirgeButton->GetRender()->GetTransform()->SetLocalPosition(float4{ -630.0f,250.0f,0.0f }*GameEngineActor::ScreenRatio);
	MirgeButton->GetRender_Bottom()->SetTexture("Shop_SkillUnderBar.png");
	MirgeButton->GetRender_Bottom()->GetTransform()->SetLocalScale(float4{ 225.0f,28.0f,0.0f }*GameEngineActor::ScreenRatio);
	MirgeButton->GetRender_Bottom()->GetTransform()->SetLocalPosition(float4{ -628.0f,205.0f,0.0f }*GameEngineActor::ScreenRatio);
	MirgeButton->GetRender_Top()->SetTexture("Shop_SkillTopBar.png");
	MirgeButton->GetRender_Top()->GetTransform()->SetLocalScale(float4{ 84.0f,23.0f,0.0f }*GameEngineActor::ScreenRatio);
	MirgeButton->GetRender_Top()->GetTransform()->SetLocalPosition(float4{ -558.0f,292.0f,0.0f }*GameEngineActor::ScreenRatio);
	MirgeButton->GetRender_Select()->SetTexture("Shop_SkillSelect.png");
	MirgeButton->GetRender_Select()->GetTransform()->SetLocalScale(float4{ 228.0f,62.0f,0.0f }*GameEngineActor::ScreenRatio);
	MirgeButton->GetRender_Select()->GetTransform()->SetLocalPosition(float4{ -630.0f,250.0f,0.0f }*GameEngineActor::ScreenRatio);
	MirgeButton->GetFont()->SetFont("DMC5Font");
	MirgeButton->GetFont()->SetFontFlag(FW1_LEFT);
	MirgeButton->SetIsPosValue(true);
	MirgeButton->GetFont()->GetTransform()->SetLocalPosition(float4{ -736.0f,222.0f,0.0f }*GameEngineActor::ScreenRatio);
	
	YamatoSkill = GetLevel()->CreateActor<Shop_VergilYamatoUI>();
	YamatoSkill->GetTransform()->SetParent(GetTransform());
	YamatoSkill->Off();

	MirgeSkill = GetLevel()->CreateActor<Shop_VergilMirgeUI>();
	MirgeSkill->GetTransform()->SetParent(GetTransform());
	MirgeSkill->Off();
	RedOrbUIPtr = GetLevel()->CreateActor<RedOrbUI>();
	RedOrbUIPtr->GetTransform()->SetParent(GetTransform());

	MiregePersent = CreateComponent<GameEngineFontRenderer>(4);
	MiregePersent->SetFont("DMC5Font");
	MiregePersent->SetFontFlag(FW1_CENTER);
	MiregePersent->SetScale(24.0f);
	MiregePersent->SetColor(float4(0.67f, 0.721f, 0.720f));
	MiregePersent->GetTransform()->SetLocalPosition(float4{ -540.0f,160.0f,0.0f }*GameEngineActor::ScreenRatio);
	YamatoPersent = CreateComponent<GameEngineFontRenderer>(4);
	YamatoPersent->SetFont("DMC5Font");
	YamatoPersent->SetFontFlag(FW1_CENTER);
	YamatoPersent->SetScale(24.0f);
	YamatoPersent->SetColor(float4(0.67f, 0.721f, 0.720f));
	YamatoPersent->GetTransform()->SetLocalPosition(float4{ -540.0f,306.0f,0.0f }*GameEngineActor::ScreenRatio);
}



void Vergil_ShopUI::Update(float _Delta)
{
	if (SkillButton->IsUpdate() == true)
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
			MirgeSkill->Off();
			for (int i = 0; i < Shop_ItemButton::Skills.size(); i++)
			{
				Shop_ItemButton::Skills[i]->On();
			}
			for (int i = 0; i < Shop_ItemButton::SecoundSkills.size(); i++)
			{
				Shop_ItemButton::SecoundSkills[i]->Off();
			}
		}
		else
		{
			ScaleUpDown(_Delta, MirgeButton);
			MirgeButton->SetSelectValue(true);
			YamatoButton->SetSelectValue(false);
			YamatoSkill->Off();
			MirgeSkill->On();
			for (int i = 0; i < Shop_ItemButton::Skills.size(); i++)
			{
				Shop_ItemButton::Skills[i]->Off();
			}
			for (int i = 0; i < Shop_ItemButton::SecoundSkills.size(); i++)
			{
				Shop_ItemButton::SecoundSkills[i]->On();
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
	SetPercent();
}
void Vergil_ShopUI::ApperCusterWindow(bool _Value)
{
	EnterWindow->IsCustermizing(_Value);
}
void Vergil_ShopUI::ShopOff()
{
	SkillButton->Off();
	YamatoButton->Off();
	MirgeButton->Off();
	YamatoSkill->Off();
	MirgeSkill->Off();
	RedOrbUIPtr->Off();
	MiregePersent->Off();
	YamatoPersent->Off();
}
void Vergil_ShopUI::ShopOn()
{
	SkillButton->On();
	YamatoButton->On();
	MirgeButton->On();
	YamatoSkill->On();
	MirgeSkill->On();
	RedOrbUIPtr->On();
	MiregePersent->On();
	YamatoPersent->On();
}
void Vergil_ShopUI::SetIndexCount()
{
	if (Index == 0)
	{
		YamatoSkill->IsActiveYamato(false);
		MirgeSkill->IsActiveMirge(false);
	}
	else if (Index == 1)
	{
		YamatoSkill->IsActiveYamato(true);
		MirgeSkill->IsActiveMirge(true);
	}
	if (Index == 0 && true == GameEngineInput::IsDown("UI_UP"))
	{
		GameEngineSound::Play("ButtonSound.wav");
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
	if (Index == 0 && true == GameEngineInput::IsDown("UI_DOWN"))
	{
		GameEngineSound::Play("ButtonSound.wav");
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
			GameEngineSound::Play("UIButtonSelectSound.wav");
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
			GameEngineSound::Play("UIButtonSelectSound.wav");
			Index--;
		}
	}
}

void Vergil_ShopUI::SetPercent()
{
	float MirgeRatio = static_cast<float>(MirgeSkill->GetMirgeCount()) / static_cast<float>(4);
	int MirgePercent = static_cast<int>(MirgeRatio * 100);
	MiregePersent->SetText(std::to_string(MirgePercent) + "%");

	float YamatoRatio = static_cast<float>(YamatoSkill->GetYamatoCount()) / static_cast<float>(5);
	int YamatoPercent = static_cast<int>(YamatoRatio * 100);
	YamatoPersent->SetText(std::to_string(YamatoPercent) + "%");
}

void Vergil_ShopUI::ScaleUpDown(float _Delta , std::shared_ptr<Shop_TitleButton> _Button)
{
	LerpTime += _Delta;
	if (ScaleValue == false)
	{
		_Button->GetRender_Select()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderScale * GameEngineActor::ScreenRatio, RenderScale * 1.05f * GameEngineActor::ScreenRatio, LerpTime * 5.0f));
		_Button->GetRender()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderScale * GameEngineActor::ScreenRatio, RenderScale * 1.05f * GameEngineActor::ScreenRatio, LerpTime * 5.0f));
		_Button->GetRender_Top()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderTopScale * GameEngineActor::ScreenRatio, RenderTopScale * 1.05f * GameEngineActor::ScreenRatio, LerpTime * 5.0f));
		_Button->GetRender_Bottom()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderBottomScale * GameEngineActor::ScreenRatio, RenderBottomScale * 1.05f * GameEngineActor::ScreenRatio, LerpTime * 5.0f));
	}

	if (ScaleValue == false && _Button->GetRender()->GetTransform()->GetLocalScale() == float4(RenderScale * 1.05f * GameEngineActor::ScreenRatio))
	{
		ScaleValue = true;
		LerpTime = 0.0f;
	}
	if (ScaleValue == true)
	{
		_Button->GetRender_Select()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderScale * 1.05f * GameEngineActor::ScreenRatio, RenderScale * GameEngineActor::ScreenRatio, LerpTime * 5.0f));
		_Button->GetRender()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderScale * 1.05f * GameEngineActor::ScreenRatio, RenderScale * GameEngineActor::ScreenRatio, LerpTime * 5.0f));
		_Button->GetRender_Top()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderTopScale * 1.05f * GameEngineActor::ScreenRatio, RenderTopScale * GameEngineActor::ScreenRatio, LerpTime * 5.0f));
		_Button->GetRender_Bottom()->GetTransform()->SetLocalScale(float4::LerpClamp(RenderBottomScale * 1.05f * GameEngineActor::ScreenRatio, RenderBottomScale * GameEngineActor::ScreenRatio, LerpTime * 5.0f));
	}
}
