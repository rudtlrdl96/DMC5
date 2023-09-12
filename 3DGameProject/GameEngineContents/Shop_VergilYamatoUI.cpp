#include "PrecompileHeader.h"
#include "Shop_VergilYamatoUI.h"
#include "Shop_TitleButton.h"
#include "Shop_ItemButton.h"
#include "Shop_ExplaneUI.h"
#include "Vergil_ShopTitleBar.h"
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
Shop_VergilYamatoUI::Shop_VergilYamatoUI()
{
}

Shop_VergilYamatoUI::~Shop_VergilYamatoUI()
{
}

void Shop_VergilYamatoUI::Start()
{

	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_0, { ._Name = "RAPID SLASH",._Price = "1200",._png = "StreakSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_1, { ._Name = "UPPER SLASH",._Price = "4000",._png = "SplitSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png" ,.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_2, { ._Name = "AERIAL RAVE B",._Price = "7500",._png = "RedQuennCSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_3, { ._Name = "YAMATO COMBO C",._Price = "9000",._png = "RouletteSpinSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png" ,.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_4, { ._Name = "JUDGMENT CUT END",._Price = "35000",._png = "ShuffleSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));

	ExplanePtr = GetLevel()->CreateActor<Shop_ExplaneUI>();
	ExplanePtr->GetTransform()->SetParent(GetTransform());
	ExplanePtr->GetTransform()->SetWorldPosition({ 0.0f,0.0f,0.0f });
	SetValue();
}

void Shop_VergilYamatoUI::Update(float _Delta)
{
	if (Vergil_ShopTitleBar::Vergil_ShopBar->GetUIIndex() == 1)
	{
		SetIndex();
		Shop_ItemButton::Skills[PrevIndex]->SetSelectValue(false);
		Shop_ItemButton::Skills[Index]->SetSelectValue(true);
		SetExText();
	}
	else
	{
		Shop_ItemButton::Skills[PrevIndex]->SetSelectValue(false);
		Shop_ItemButton::Skills[Index]->SetSelectValue(false);
		PrevIndex = 4;
		Index = 0;
	}
}

void Shop_VergilYamatoUI::SetIndex()
{
	if (true == GameEngineInput::IsUp("UI_UP"))
	{
		PrevIndex = Index;
		if (Index == 0)
		{
			Index = 4;
		}
		else
		{
			Index--;
		}


	}
	if (true == GameEngineInput::IsUp("UI_DOWN"))
	{
		PrevIndex = Index;
		if (Index == 4)
		{
			Index = 0;
		}
		else
		{
			Index++;
		}
	


	}
}

void Shop_VergilYamatoUI::SetExText()
{
	switch (Index)
	{
	case 0:
		ExplanePtr->SetExplaneText({ ._EName = "RAPID SLASH" ,._KName = "���ǵ� ������" ,._Explane1 = "������ �����ϸ鼭 �� �������� ��� �͵���",._Explane2 = "�������� ��������ϴ�.",._Explane3 = "�� ���� Ʈ���� ���¿����� �� ���ݰ� ���ÿ�" 
			,._Explane4 = "�̶��� ���̵�� ������ ���� ���ڷ翡 ����",._Explane5 = "�� ���Ľ��� �߰� �������� �ݴϴ�. �� ���̰�"
			,._Explane6 = "����ؼ� �ߵ��� �� �ֽ��ϴ�."});
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "RapidSlashExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition({ 506.0f,-142.0f,0.0f });
		break;
	case 1:
		ExplanePtr->SetExplaneText({ ._EName = "UPPER SLASH" ,._KName = "���� ������" ,._Explane1 = "���� ������ �������� ���������� ������",._Explane2 = "Ȧ���Ͽ� �������� �Ѿư��ϴ�."});
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "Uperslah2Explane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition({ 502.0f,-145.0f,0.0f });
		break;
	case 2:
		ExplanePtr->SetExplaneText({ ._EName = "AERIAL RAVE B" ,._KName = "����� ���̺� B" ,._Explane1 = "���߿��� ȸ���ϸ鼭 ���� �������",._Explane2 = "����, ȥ���� �ϰ����� ���� �����ȴ�",._Explane3 = "�޺�." });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "AerialRaveBExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition({ 502.0f,-143.0f,0.0f });
		break;
	case 3:
		ExplanePtr->SetExplaneText({ ._EName = "YAMATO COMBO C" ,._KName = "�߸��� �޺� C" ,._Explane1 = "������ ������ �ۺ��� ���� ����",._Explane2 = "�ѷ� ���� �����ϴ�.",._Explane3 = "�� ���� Ʈ���� ���¿����� �ǴϽ�"
			,._Explane4 = "����� ������ �� �о����ϴ�."});
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "YamatoCombocExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition({ 497.0f,-142.0f,0.0f });
		break;
	case 4:
		ExplanePtr->SetExplaneText({ ._EName = "JUDGMENT CUT END" ,._KName = "������Ʈ �� ����" ,._Explane1 = "���� �ӵ��� ���ÿ� ���� ��ҿ� �ڽ���",._Explane2 = "�н��� ����� ���� ���� �����ϴ�.",._Explane3 = "�ߵ��ϸ� SDT �������� �Ҹ��մϴ�." });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "JudgementcutendExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition({ 531.0f,-170.0f,0.0f });
		break;
	default:
		break;
	}
}

void Shop_VergilYamatoUI::SetValue()
{
	for (int i = 0; i < Shop_ItemButton::Skills.size(); i++)
	{
		Shop_ItemButton::Skills[i]->SetSizeValue(true);
	}
}

