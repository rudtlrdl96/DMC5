#include "PrecompileHeader.h"
#include "Shop_VergilMirgeUI.h"
#include "Shop_TitleButton.h"
#include "Shop_ItemButton.h"
#include "Shop_ExplaneUI.h"
#include "Vergil_ShopUI.h"
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
bool Shop_VergilMirgeUI::IsSpiralBlade = false;
bool Shop_VergilMirgeUI::IsStormBlade = false;
bool Shop_VergilMirgeUI::IsLesteringBlade = false;
bool Shop_VergilMirgeUI::IsHeavyRainBlade = false;
Shop_VergilMirgeUI::Shop_VergilMirgeUI()
{
}

Shop_VergilMirgeUI::~Shop_VergilMirgeUI()
{
}

void Shop_VergilMirgeUI::Start()
{

	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_0 * GameEngineActor::ScreenRatio, { ._Name = "SPIRAL BLADE",._Price = "1200",._png = "StreakSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true ,.Value = 1 }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_1 * GameEngineActor::ScreenRatio, { ._Name = "STORM BLADE",._Price = "4000",._png = "SplitSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png" ,.IsValue = true,.Value = 1 }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_2 * GameEngineActor::ScreenRatio, { ._Name = "BLESTERING BLADE",._Price = "7500",._png = "RedQuennCSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true ,.Value = 1 }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_3 * GameEngineActor::ScreenRatio, { ._Name = "HEAVY RAIN BLAND",._Price = "9000",._png = "RouletteSpinSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png" ,.IsValue = true ,.Value = 1 }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);

	ExplanePtr = GetLevel()->CreateActor<Shop_ExplaneUI>();
	ExplanePtr->GetTransform()->SetParent(GetTransform());
	ExplanePtr->GetTransform()->SetWorldPosition({ 0.0f,0.0f,0.0f });
	SetValue();
}

void Shop_VergilMirgeUI::Update(float _Delta)
{
	if (Vergil_ShopUI::Vergil_ShopBar->GetUIIndex() == 1)
	{
		SetIndex();
		Shop_ItemButton::SecoundSkills[PrevIndex]->SetSelectValue(false);
		Shop_ItemButton::SecoundSkills[Index]->SetSelectValue(true);
		SetExText();
	}
	else
	{
		Shop_ItemButton::SecoundSkills[PrevIndex]->SetSelectValue(false);
		Shop_ItemButton::SecoundSkills[Index]->SetSelectValue(false);
		PrevIndex = 3;
		Index = 0;
	}
	ActivSkill();
}

void Shop_VergilMirgeUI::SetIndex()
{
	if (true == GameEngineInput::IsUp("UI_UP"))
	{
		PrevIndex = Index;
		if (Index == 0)
		{
			Index = 3;
		}
		else
		{
			Index--;
		}


	}
	if (true == GameEngineInput::IsUp("UI_DOWN"))
	{
		PrevIndex = Index;
		if (Index == 3)
		{
			Index = 0;
		}
		else
		{
			Index++;
		}



	}
}

void Shop_VergilMirgeUI::SetExText()
{
	switch (Index)
	{
	case 0:
		ExplanePtr->SetExplaneText({ ._EName = "SPIRAL BLADES" ,._KName = "�����̷� ���̵�" ,._Explane1 = "�̶��� ���̵�� �ڽ��� �ֺ��� ����",._Explane2 = "�� ���з� Ȱ���մϴ�.",._Explane3 = "�ߵ��ϸ� DT�������� �Ҹ��մϴ�." });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "SpiralbladeExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 457.0f,-139.0f,0.0f }*GameEngineActor::ScreenRatio);
		break;
	case 1:
		ExplanePtr->SetExplaneText({ ._EName = "STORM BLADES" ,._KName = "���� ���̵�" ,._Explane1 = "���� ���� ������ ���� ������ ����,",._Explane2 = "���ÿ� �����Ͽ� �������� ���������ϴ�." ,._Explane3 = "�ߵ��ϸ� DT�������� �Ҹ��մϴ�." });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "StormBladeExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 506.0f,-154.0f,0.0f } * GameEngineActor::ScreenRatio);
		break;
	case 2:
		ExplanePtr->SetExplaneText({ ._EName = "BLESTERING BLADES" ,._KName = "�����͸� ���̵�" ,._Explane1 = "�ڽ��� �ֺ��� �̶��� ���̵带",._Explane2 = "����� ���� ���� �������� �߻��մϴ�.",._Explane3 = "�ߵ��ϸ� DT �������� �Ҹ��մϴ�." });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "BlesteringBladeExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 512.0f,-154.0f,0.0f } * GameEngineActor::ScreenRatio);
		break;
	case 3:
		ExplanePtr->SetExplaneText({ ._EName = "HEAVY RAIN BLAND" ,._KName = "����� ���̵�" ,._Explane1 = "���� �Ӹ� ���� ������ ���� �̶���",._Explane2 = "���̵带 ����� ���� ���� ��",._Explane3 = "������, �Ͻ������� ���ڸ��� ����"
			,._Explane4 = "������ �����ϴ�." ,._Explane5 = "�ߵ��ϸ� DT �������� �Ҹ��մϴ�."});
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "HeavyRainBladeExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 540.0f,-154.0f,0.0f } * GameEngineActor::ScreenRatio);
		break;
	default:
		break;
	}
}

void Shop_VergilMirgeUI::SetValue()
{
	for (int i = 0; i < Shop_ItemButton::SecoundSkills.size(); i++)
	{
		Shop_ItemButton::SecoundSkills[i]->SetSizeValue(true);
	}
}

void Shop_VergilMirgeUI::ActivSkill()
{
	if (Index == 0)
	{
		if (IsSpiralBlade == false && GameEngineInput::IsUp("UI_Enter") == true)
		{
			IsSpiralBlade = true;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");
		}
	}
	else if (Index == 1)
	{
		if (IsStormBlade == false && GameEngineInput::IsUp("UI_Enter") == true)
		{
			IsStormBlade = true;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");
		}
	}
	else if (Index == 2)
	{
		if (IsLesteringBlade == false && GameEngineInput::IsUp("UI_Enter") == true)
		{
			IsLesteringBlade = true;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");

		}
	}
	else if (Index == 3)
	{
		if (IsHeavyRainBlade == false && GameEngineInput::IsUp("UI_Enter") == true)
		{
			IsHeavyRainBlade = true;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");

		}
	}
}

