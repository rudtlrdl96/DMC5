#include "PrecompileHeader.h"
#include "Menu_NeroInfo.h"
#include "Nero_InvenToryUI.h"

#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>

Menu_NeroInfo::Menu_NeroInfo() 
{
}

Menu_NeroInfo::~Menu_NeroInfo() 
{
}

void Menu_NeroInfo::Start()
{
	NeroInfoPtr = GetLevel()->CreateActor<InfoUIRender>();
	NeroInfoPtr->GetRender()->SetTexture("NeroLogo.png");
	NeroInfoPtr->GetRender_Bar()->SetTexture("Menu_InfoBar.png");
	NeroInfoPtr->GetRender_Base()->SetTexture("Menu_difficultyBase.png");
	NeroInfoPtr->GetRender_Base()->ColorOptionValue.MulColor.a = 0.7f;
	NeroInfoPtr->SetUIText({ ._Text = "PLAYER",._SeletText = "NERO",._ExplaneText = "GAME MODE",._ExplaneText2 = "DEVIL HUNTER" });
	
	NeroInvenPtr = GetLevel()->CreateActor<InfoUIRender>();
	NeroInvenPtr->GetTransform()->SetLocalPosition({ 0.0f,-200.f,0.0f });
	NeroInvenPtr->GetRender()->SetTexture("NullTexture.png");
	NeroInvenPtr->GetRender_Bar()->SetTexture("Menu_InfoBar.png");
	NeroInvenPtr->GetRender_Base()->SetTexture("Menu_TabBase.png");
	NeroInvenPtr->GetRender_Base()->GetTransform()->SetLocalPosition({ 600.f, 60.f, 0.0f });
	NeroInvenPtr->GetRender_Base()->GetTransform()->SetLocalScale({ 337.f, 35.f, 1.0f});
	NeroInvenPtr->SetUIText({ ._Text = "EQUIPMENT",._SeletText = "",._ExplaneText = "",._ExplaneText2 = "" });

	Arrow1Ptr = GetLevel()->CreateActor<InvenUIButton>();
	Arrow1Ptr->GetRender()->SetScaleToTexture("Menu_Arrow.png");
	Arrow1Ptr->SetLerpValue(float4(530.f, -140.f, 0.0f), float4(520.f, -140.f, 0.0f));
	Arrow1Ptr->SetEvent([this]()
		{
			if (InvenMinusValue == false)
			{
				PrevIndex = Index;
				if (Index != 0)
				{
					Index--;
					InvenMinusValue = true;
				}
				else
				{
					Index = 2;
					InvenMinusValue = true;
				}
			}
		
		});
	Arrow2Ptr = GetLevel()->CreateActor<InvenUIButton>();
	Arrow2Ptr->GetRender()->SetTexture("Menu_Arrow.png");
	Arrow2Ptr->GetRender()->GetTransform()->SetLocalScale({ -13.0f,16.0f,0.0f });
	Arrow2Ptr->SetLerpValue(float4(685.f, -140.f, 0.0f), float4(695.f, -140.f, 0.0f));
	Arrow2Ptr->SetEvent([this]()
		{
			if (InvenPlusValue == false)
			{
				PrevIndex = Index;
				if (Index != 2)
				{
					Index++;
					InvenPlusValue = true;
				}
				else
				{
					Index = 0;
					InvenPlusValue = true;
				}
			}

		
		});
	FontRender = CreateComponent<GameEngineFontRenderer>(8);
	FontRender->SetFont("DMC5Font");
	FontRender->SetScale(24.0f);
	FontRender->SetFontFlag(FW1_CENTER);
	FontRender->SetColor(float4(0.305f, 0.96f, 0.94f, 1.0f));
	FontRender->GetTransform()->SetLocalPosition(float4(610.f, -125.f, 0.0f));

	Nero_InvenToryUI::CreateNeroInven(GetLevel());
	Nero_InvenToryUI::CreateNeroInven(GetLevel());
	Nero_InvenToryUI::CreateNeroInven(GetLevel());

}

void Menu_NeroInfo::Update(float _Delta)
{
	if (InvenMinusValue == true)
	{
		//������ prevIndex���� ���� >>���� ������ �� ������� ->������ Index ���� >>���� �����Ѵ�.
		//PrevIndex�� ���� 0 0 0 ���� �������� ���� index�� ���� -100 0 0 ���� ������Ų�� 
		//prevIndex ���� 100 0 0 ���� ������ �� alpha�� 0 �̵Ǹ� index�� 0 0 0 ���� ������Ų��.
		//PrevIndex�� ���� -100.0 0 0 ���� �������� ���� 
		//index�� ���� 0 0 0 ���� ������Ų�� 
		int asd = Index;
		InvenMinusValue = false;
		int a = 0;
		Test.push_back(1);
	}
	if (InvenPlusValue == true)
	{
		int asd = Index;
		InvenPlusValue = false;
		Test.push_back(1);
	}
	if (Index == 0)
	{
		FontRender->SetText("MAGAZINE");
	}
	else if (Index == 1)
	{
		FontRender->SetText("MAGAZINE01");
	}
	else if (Index == 2)
	{
		FontRender->SetText("MAGAZINE02");
	}

	if (InvenPlusValue == false && true == GameEngineInput::IsUp("UI_Tab"))
	{
		PrevIndex = Index;
		if (Index == 2)
		{
			InvenPlusValue = true;
			Index = 0;
		}
		else
		{
			InvenPlusValue = true;
			Index++;
		}
	}
}

