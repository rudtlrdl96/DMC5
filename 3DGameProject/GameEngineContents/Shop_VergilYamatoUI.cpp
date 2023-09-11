#include "PrecompileHeader.h"
#include "Shop_VergilYamatoUI.h"
#include "Shop_TitleButton.h"
#include "Shop_ItemButton.h"
#include "Shop_ExplaneUI.h"

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
	YamatoButton->SetSelectValue(true);
	YamatoButton->GetFont()->SetFont("DMC5Font");
	YamatoButton->SetIsPosValue(true);
	YamatoButton->GetFont()->GetTransform()->SetLocalPosition({ -680.0f,224.0f,0.0f });
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
	SetIndex();
	Shop_ItemButton::Skills[PrevIndex]->SetSelectValue(false);
	Shop_ItemButton::Skills[Index]->SetSelectValue(true);
	//LerpItem(_Delta);
	SetExText();

}

void Shop_VergilYamatoUI::LerpItem(float _Delta)
{
	if (DownSwich == true)
	{
		IsValue = true;
		LerpTime += _Delta;
		Shop_ItemButton::Skills[Index]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_5, Pos_4, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 1]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_4, Pos_3, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 2]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_3, Pos_2, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 3]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_2, Pos_1, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 4]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_1, Pos_0, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 5]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_0, Pos_Null, LerpTime * 5.0f));

		if (Shop_ItemButton::Skills[Index - 5]->GetTransform()->GetLocalPosition().y >= (Pos_Null.y + Pos_0.y) / 2)
		{
			Shop_ItemButton::Skills[Index - 5]->Off();
		}
		if (Shop_ItemButton::Skills[Index]->GetTransform()->GetLocalPosition() == Pos_4)
		{
			DownSwich = false;
			LerpTime = 0.0f;
			IsValue = false;
		}
	}
	if (UpSwich == true)
	{
		IsValue = true;
		LerpTime += _Delta;
		Shop_ItemButton::Skills[Index]->On();
		Shop_ItemButton::Skills[Index]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_Null, Pos_0, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 1]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_0, Pos_1, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 2]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_1, Pos_2, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 3]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_2, Pos_3, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 4]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_3, Pos_4, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 5]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_4, Pos_5, LerpTime * 5.0f));

		if (Shop_ItemButton::Skills[Index]->GetTransform()->GetLocalPosition() == Pos_0)
		{
			IsValue = false;
			UpSwich = false;
			LerpTime = 0.0f;
		}
	}
}

void Shop_VergilYamatoUI::SetIndex()
{
	YamatoButton->SetUIText("YAMATO");
	if (IsValue == false && true == GameEngineInput::IsUp("UI_UP"))
	{
		PrevIndex = Index;
		if (Index == 0)
		{
			Index = 4;
			DownSwich = true;
		}
		else
		{
			Index--;
		}
		if (PrevIndex == 1 && Index == 0)
		{
			UpSwich = true;
		}


	}
	if (IsValue == false && true == GameEngineInput::IsUp("UI_DOWN"))
	{
		PrevIndex = Index;
		if (Index == 4)
		{
			Index = 0;
			UpSwich = true;
		}
		else
		{
			Index++;
		}
		if (PrevIndex == 3 && Index == 4)
		{
			DownSwich = true;
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

