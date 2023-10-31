#include "PrecompileHeader.h"
#include "Shop_NeroArmUI.h"
#include "Shop_TitleButton.h"
#include "Shop_ItemButton.h"
#include "Shop_ExplaneUI.h"
#include "Nero_InvenToryUI.h"
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "Nero_Inven.h"
#include "NeroItemGlass.h"
#include "PlayerActor_Nero.h"
#include "RedOrbUI.h"
 std::function<void(DevilBreaker _Value)> Shop_NeroArmUI::CallBack_AddBreaker = nullptr;
 std::vector<DevilBreaker> Shop_NeroArmUI::DevilBreakers;

Shop_NeroArmUI::Shop_NeroArmUI()
{
}

Shop_NeroArmUI::~Shop_NeroArmUI()
{
}

void Shop_NeroArmUI::UpdateArm()
{
	AcNeroInven->UpdateInvenTory();
}

void Shop_NeroArmUI::Start()
{
	if (Shop_ItemButton::Items.size() != 0)
	{
		Shop_ItemButton::Items.clear();
	}
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_0 * GameEngineActor::ScreenRatio, { ._Name = "OVERTURE",._Price = "500",._png = "Nero_Overture.png",._BaseNone = "Shop_ArmBase.png", ._BaseSelect = "Shop_ArmSelect.png",.IsValue = false }, GetTransform(), float4(118.0f, 120.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_1 * GameEngineActor::ScreenRatio, { ._Name = "GERBERA",._Price = "1200",._png = "Nero_Gerbera.png",._BaseNone = "Shop_ArmBase.png", ._BaseSelect = "Shop_ArmSelect.png" ,.IsValue = false }, GetTransform(), float4(118.0f, 120.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_2 * GameEngineActor::ScreenRatio, { ._Name = "BUSTER ARM",._Price = "5000",._png = "Nero_BusterArm.png",._BaseNone = "Shop_ArmBase.png", ._BaseSelect = "Shop_ArmSelect.png",.IsValue = false }, GetTransform(), float4(118.0f, 120.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	ArmExplane = GetLevel()->CreateActor<Shop_ExplaneUI>();
	ArmExplane->GetTransform()->SetParent(GetTransform());
	ArmExplane->GetTransform()->SetWorldPosition({ 0.0f,0.0f,0.0f });
	AcNeroInven = GetLevel()->CreateActor<Nero_Inven>();
	AcNeroInven->GetTransform()->SetParent(GetTransform());
	ArmList = PlayerActor_Nero::GetBreakerListPointer();

	AcNeroInven->GetInvenVec()[1]->Off();
	AcNeroInven->GetInvenVec()[2]->Off();
	AcNeroInven->GetInvenVec()[0]->Off();
	Nero_InvenToryUI::IsAddInvenItem(true);
}

void Shop_NeroArmUI::Update(float _Delta)
{
	if (ArmExplane->IsUpdate() == false)
	{
		AcNeroInven->GetInvenVec()[1]->Off();
		AcNeroInven->GetInvenVec()[2]->Off();
		AcNeroInven->GetInvenVec()[0]->Off();

	}
	else
	{
		//���ڷ� ���θ޴����� ����ִ� �κ� �ε��� �־��ֱ�
		AcNeroInven->GetInvenVec()[0]->On();
		AcNeroInven->GetInvenVec()[0]->GetTransform()->SetLocalPosition(float4{ 665.0f,-233.0f,0.0f }*GameEngineActor::ScreenRatio);
	}


	SetIndex();
	Shop_ItemButton::Items[PrevIndex]->SetSelectValue(false);
	Shop_ItemButton::Items[Index]->SetSelectValue(true);
	SetExText();
	BuyBreaker();
}

void Shop_NeroArmUI::SetIndex()
{
	if (IsValue == false && true == GameEngineInput::IsUp("UI_UP"))
	{
		GameEngineSound::Play("ButtonSound.wav");

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
		GameEngineSound::Play("ButtonSound.wav");
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
	case 2:
		ArmExplane->SetExplaneText({ ._EName = "BUSTER ARM" ,._KName = "����Ʈ ��" ,._Explane1 = "ȭ���� �� �޺���",._Explane2 = "���� ���� ������ ����",._Explane3 = "������ ��ġ��ϴ�." });
		break;

	default:
		break;
	}
}

void Shop_NeroArmUI::BuyBreaker()
{
	if (Index == 0)
	{
		if (ArmList->size() <= 4 &&GameEngineInput::IsUp("UI_Enter") == true && IsSelect == true && RedOrbUI::GetRedOrb() >500)
		{
			RedOrbUI::SubRedOrb(500);
			if (nullptr != CallBack_AddBreaker)
			{
				GameEngineSound::Play("UIButtonSelectSound.wav");
				CallBack_AddBreaker(DevilBreaker::Overture);
				Nero_InvenToryUI::IsAddInvenItem(true);
			}
			else
			{
				GameEngineSound::Play("UIButtonSelectSound.wav");
				PlayerActor_Nero::GetBreakerListPointer()->push_back(DevilBreaker::Overture);
				DevilBreakers.insert(DevilBreakers.begin(), DevilBreaker::Overture);
				NeroItemGlass::AddItemUI(true);
				Nero_InvenToryUI::IsAddInvenItem(true);
			}

		}
	}
	else if (Index == 1)
	{

		if (ArmList->size() <= 4 && GameEngineInput::IsUp("UI_Enter") == true && IsSelect == true && RedOrbUI::GetRedOrb() > 1200)
		{
			RedOrbUI::SubRedOrb(1200);
			if (nullptr != CallBack_AddBreaker)
			{
				GameEngineSound::Play("UIButtonSelectSound.wav");
				CallBack_AddBreaker(DevilBreaker::Gerbera);
				Nero_InvenToryUI::IsAddInvenItem(true);
			}
			else
			{
				GameEngineSound::Play("UIButtonSelectSound.wav");
				PlayerActor_Nero::GetBreakerListPointer()->push_back(DevilBreaker::Gerbera);
				DevilBreakers.insert(DevilBreakers.begin(), DevilBreaker::Gerbera);
				NeroItemGlass::AddItemUI(true);
				Nero_InvenToryUI::IsAddInvenItem(true);
			}

		}
	}
	else if (Index == 2)
	{
		if (ArmList->size() <= 4 && GameEngineInput::IsUp("UI_Enter") == true && IsSelect == true && RedOrbUI::GetRedOrb() > 5000)
		{
			RedOrbUI::SubRedOrb(5000);
			if (nullptr != CallBack_AddBreaker)
			{
				GameEngineSound::Play("UIButtonSelectSound.wav");
				CallBack_AddBreaker(DevilBreaker::BusterArm);
				Nero_InvenToryUI::IsAddInvenItem(true);
			}
			else
			{
				GameEngineSound::Play("UIButtonSelectSound.wav");
				PlayerActor_Nero::GetBreakerListPointer()->push_back(DevilBreaker::BusterArm);
				DevilBreakers.insert(DevilBreakers.begin(), DevilBreaker::BusterArm);
				NeroItemGlass::AddItemUI(true);
				Nero_InvenToryUI::IsAddInvenItem(true);
			}

		}
	}
}

