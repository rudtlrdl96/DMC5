#include "PrecompileHeader.h"
#include "Shop_NeroArmUI.h"
#include "Shop_TitleButton.h"
#include "Shop_ItemButton.h"
#include "Shop_ExplaneUI.h"
#include "Nero_InvenToryUI.h"
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>

Shop_NeroArmUI::Shop_NeroArmUI()
{
}

Shop_NeroArmUI::~Shop_NeroArmUI()
{
}

void Shop_NeroArmUI::Start()
{

	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_0, { ._Name = "OVERTURE",._Price = "500",._png = "Nero_Overture.png",._BaseNone = "Shop_ArmBase.png", ._BaseSelect = "Shop_ArmSelect.png",.IsValue = false }, GetTransform(), float4(118.0f, 120.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_1, { ._Name = "GERBERA",._Price = "1200",._png = "Nero_Gerbera.png",._BaseNone = "Shop_ArmBase.png", ._BaseSelect = "Shop_ArmSelect.png" ,.IsValue = false }, GetTransform(), float4(118.0f, 120.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_2, { ._Name = "BUSTER ARM",._Price = "5000",._png = "Nero_BusterArm.png",._BaseNone = "Shop_ArmBase.png", ._BaseSelect = "Shop_ArmSelect.png",.IsValue = false }, GetTransform(), float4(118.0f, 120.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	ArmExplane = GetLevel()->CreateActor<Shop_ExplaneUI>();
	ArmExplane->GetTransform()->SetParent(GetTransform());
	ArmExplane->GetTransform()->SetWorldPosition({ 0.0f,0.0f,0.0f });
	
}

void Shop_NeroArmUI::Update(float _Delta)
{
	Nero_InvenToryUI::Invens[0]->GetTransform()->SetParent(GetTransform());
	if (ArmExplane->IsUpdate() == false)
	{
	}
	else
	{

	}
	SetIndex();
	Shop_ItemButton::Items[PrevIndex]->SetSelectValue(false);
	Shop_ItemButton::Items[Index]->SetSelectValue(true);
	SetExText();

}

void Shop_NeroArmUI::SetIndex()
{
	if (IsValue == false && true == GameEngineInput::IsUp("UI_UP"))
	{
		PrevIndex = Index;
		if (Index == 0)
		{
			Index = 2;
			DownSwich = true;
		}
		else
		{
			Index--;
		}
	}
	if (IsValue == false && true == GameEngineInput::IsUp("UI_DOWN"))
	{
		PrevIndex = Index;
		if (Index == 2)
		{
			Index = 0;
			UpSwich = true;
		}
		else
		{
			Index++;
		}
	}
}

void Shop_NeroArmUI::SetExText()
{
	ArmExplane->SetSecoundText({ ._EName = "EQUIPMENT" ,._Explane1 = "Menu_difficultyBase.png",
	._Explane2 = "MAGAZINE" });
	switch (Index)
	{
	case 0:
		ArmExplane->SetExplaneText({ ._EName = "OVERTURE" ,._KName = "�������" 
			,._Explane1 = "�ٿ뵵�� ��밡���� ���� �극��Ŀ.",
			._Explane2 = "���� ����ķ� ���� �����ų�,"
			,._Explane3 = "������ �� �ְ� ���߽�ų �� �ִ�." });

			break;
	case 1:
		ArmExplane->SetExplaneText({ ._EName = "GERBERA" ,._KName = "�ź���" ,
			._Explane1 = "�̵����� ���� ���� �극��Ŀ."
			,._Explane2 = "ū ������ ����ķ� ������ ����ȭ�ϰų�"
			,._Explane3 = "���� ���� ���� ������ �߻��Ѵ�." });
		break;
		break;
	case 2:
		ArmExplane->SetExplaneText({ ._EName = "BUSTER ARM" ,._KName = "����Ʈ ��" ,._Explane1 = "ȭ���� �� �޺���",._Explane2 = "���� ���� ������ ����",._Explane3 = "������ ��ġ��ϴ�." });

		break;
		break;

	default:
		break;
	}
}

