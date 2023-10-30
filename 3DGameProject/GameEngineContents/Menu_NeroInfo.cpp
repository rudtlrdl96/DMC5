#include "PrecompileHeader.h"
#include "Menu_NeroInfo.h"
#include "Nero_InvenToryUI.h"

#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>

#include "Nero_Inven.h"

Menu_NeroInfo::Menu_NeroInfo() 
{
}

Menu_NeroInfo::~Menu_NeroInfo() 
{
}

void Menu_NeroInfo::Start()
{
	WindowRatio = GameEngineActor::ScreenRatio;
	NeroInfoPtr = GetLevel()->CreateActor<InfoUIRender>();
	NeroInfoPtr->GetTransform()->SetParent(GetTransform());
	NeroInfoPtr->GetRender()->SetTexture("NeroLogo.png");
	NeroInfoPtr->GetRender_Bar()->SetTexture("Menu_InfoBar.png");
	NeroInfoPtr->GetRender_Base()->SetTexture("Menu_difficultyBase.png");
	NeroInfoPtr->GetRender_Base()->ColorOptionValue.MulColor.a = 0.7f;
	NeroInfoPtr->SetUIText({ ._Text = "PLAYER",._SeletText = "NERO",._ExplaneText = "GAME MODE",._ExplaneText2 = "DEVIL HUNTER" });
	
	NeroInvenPtr = GetLevel()->CreateActor<InfoUIRender>();
	NeroInvenPtr->GetTransform()->SetParent(GetTransform());

	NeroInvenPtr->GetTransform()->SetLocalPosition(WindowRatio* InvenPos);
	NeroInvenPtr->GetRender()->SetTexture("NullTexture.png");
	NeroInvenPtr->GetRender_Bar()->SetTexture("Menu_InfoBar.png");
	NeroInvenPtr->GetRender_Base()->SetTexture("Menu_TabBase.png");
	NeroInvenPtr->GetRender_Base()->GetTransform()->SetLocalPosition(BasePos* WindowRatio);
	NeroInvenPtr->GetRender_Base()->GetTransform()->SetLocalScale(WindowRatio * BaseScale);
	NeroInvenPtr->SetUIText({ ._Text = "EQUIPMENT",._SeletText = "",._ExplaneText = "",._ExplaneText2 = "" });

	Arrow1Ptr = GetLevel()->CreateActor<InvenUIButton>();
	Arrow1Ptr->GetTransform()->SetParent(GetTransform());

	Arrow1Ptr->GetRender()->SetTexture("Menu_Arrow.png");
	Arrow1Ptr->GetRender()->GetTransform()->SetLocalScale(ArrowScale);
	Arrow1Ptr->SetLerpValue(float4(530.f, -140.f, 0.0f)* WindowRatio, float4(520.f, -140.f, 0.0f)* WindowRatio);
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
	Arrow2Ptr->GetTransform()->SetParent(GetTransform());
	Arrow2Ptr->GetRender()->SetTexture("Menu_Arrow.png");
	Arrow2Ptr->GetRender()->GetTransform()->SetLocalScale(float4{ -13.0f,16.0f,0.0f }*WindowRatio);
	Arrow2Ptr->SetLerpValue(float4(685.f, -140.f, 0.0f) * WindowRatio, float4(695.f, -140.f, 0.0f) * WindowRatio);
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
	FontRender->SetScale(24.0f*WindowRatio.x);
	FontRender->SetFontFlag(FW1_CENTER);
	FontRender->SetColor(float4(0.305f, 0.96f, 0.94f, 1.0f));
	FontRender->GetTransform()->SetLocalPosition(float4(610.f, -125.f, 0.0f) * WindowRatio);
	AcNeroInven = GetLevel()->CreateActor<Nero_Inven>();
	AcNeroInven->GetTransform()->SetParent(GetTransform());
	AcNeroInven->GetInvenVec()[0]->GetTransform()->SetLocalPosition(CenterPos * WindowRatio);
	AcNeroInven->GetInvenVec()[0]->On();
	AcNeroInven->GetInvenVec()[1]->Off();
	AcNeroInven->GetInvenVec()[2]->Off();
}

void Menu_NeroInfo::Update(float _Delta)
{
	AcNeroInven->GetInvenVec()[Index]->IsAddInvenItem(true);
	MoveInven(_Delta);
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
	if (InvenPlusValue == false && true == GameEngineInput::IsUp("UI_Tab") && InvenMinusValue == false)
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

void Menu_NeroInfo::MoveInven(float _Delta)
{
	if (InvenMinusValue == true)
	{
		Speed += _Delta;
		// 0번 러프후 2번 러프
		if (SetPosValue == false)
		{
			AcNeroInven->GetInvenVec()[PrevIndex]->GetTransform()->SetLocalPosition(CenterPos * WindowRatio);
			AcNeroInven->GetInvenVec()[Index]->GetTransform()->SetLocalPosition(LeftPos * WindowRatio);
			AcNeroInven->GetInvenVec()[PrevIndex]->GetRender()->ColorOptionValue.MulColor.a = 1.0f;
			AcNeroInven->GetInvenVec()[PrevIndex]->On();
			SetPosValue = true;
		}
		if (ScendMove == true)
		{
			AcNeroInven->GetInvenVec()[Index]->GetRender()->ColorOptionValue.MulColor.a = 0.5f;
			AcNeroInven->GetInvenVec()[Index]->On();
			float4 M1 = float4::LerpClamp(LeftPos * WindowRatio, CenterPos * WindowRatio, Ratio * Speed);
			AcNeroInven->GetInvenVec()[Index]->GetTransform()->SetLocalPosition(M1);
			if (AcNeroInven->GetInvenVec()[Index]->GetRender()->ColorOptionValue.MulColor.a <= 1.0f)
			{
				AcNeroInven->GetInvenVec()[Index]->GetRender()->ColorOptionValue.MulColor.a += Speed * 0.5f;
			}
			if (AcNeroInven->GetInvenVec()[Index]->GetTransform()->GetLocalPosition() == CenterPos * WindowRatio)
			{
				InvenMinusValue = false;
				SetPosValue = false;
				ScendMove = false;
				AcNeroInven->GetInvenVec()[Index]->GetRender()->ColorOptionValue.MulColor.a = 1.0f;
				Speed = 0.0f;
			}

		}
		else if (ScendMove == false)
		{
			float4 M0 = float4::LerpClamp(CenterPos * WindowRatio, RightPos * WindowRatio, Ratio * Speed);
			AcNeroInven->GetInvenVec()[PrevIndex]->GetTransform()->SetLocalPosition(M0);
			AcNeroInven->GetInvenVec()[PrevIndex]->GetRender()->ColorOptionValue.MulColor.a -= Speed * 0.5f;
			if (AcNeroInven->GetInvenVec()[PrevIndex]->GetRender()->ColorOptionValue.MulColor.a <= 0.5f)
			{
				AcNeroInven->GetInvenVec()[PrevIndex]->Off();
				ScendMove = true;

			}
		}
	}
	if (InvenPlusValue == true)
	{
		Speed += _Delta;
		// 0번 러프후 2번 러프
		if (SetPosValue == false)
		{
			AcNeroInven->GetInvenVec()[PrevIndex]->GetTransform()->SetLocalPosition(CenterPos * WindowRatio);
			AcNeroInven->GetInvenVec()[Index]->GetTransform()->SetLocalPosition(RightPos * WindowRatio);
			AcNeroInven->GetInvenVec()[PrevIndex]->GetRender()->ColorOptionValue.MulColor.a = 1.0f;
			AcNeroInven->GetInvenVec()[PrevIndex]->On();
			SetPosValue = true;
		}
		if (ScendMove == true)
		{
			AcNeroInven->GetInvenVec()[Index]->GetRender()->ColorOptionValue.MulColor.a = 0.5f;
			AcNeroInven->GetInvenVec()[Index]->On();
			float4 M1 = float4::LerpClamp(RightPos * WindowRatio, CenterPos * WindowRatio, Ratio * Speed);
			AcNeroInven->GetInvenVec()[Index]->GetTransform()->SetLocalPosition(M1);
			if (AcNeroInven->GetInvenVec()[Index]->GetRender()->ColorOptionValue.MulColor.a <= 1.0f)
			{
				AcNeroInven->GetInvenVec()[Index]->GetRender()->ColorOptionValue.MulColor.a += Speed * 0.5f;
			}
			if (AcNeroInven->GetInvenVec()[Index]->GetTransform()->GetLocalPosition() == CenterPos * WindowRatio)
			{
				InvenPlusValue = false;
				SetPosValue = false;
				ScendMove = false;
				AcNeroInven->GetInvenVec()[Index]->GetRender()->ColorOptionValue.MulColor.a = 1.0f;
				Speed = 0.0f;
			}

		}
		else if (ScendMove == false)
		{
			float4 M0 = float4::LerpClamp(CenterPos * WindowRatio, LeftPos * WindowRatio, Ratio * Speed);
			AcNeroInven->GetInvenVec()[PrevIndex]->GetTransform()->SetLocalPosition(M0);
			AcNeroInven->GetInvenVec()[PrevIndex]->GetRender()->ColorOptionValue.MulColor.a -= Speed * 0.5f;
			if (AcNeroInven->GetInvenVec()[PrevIndex]->GetRender()->ColorOptionValue.MulColor.a <= 0.5f)
			{
				AcNeroInven->GetInvenVec()[PrevIndex]->Off();
				ScendMove = true;

			}
		}
	}
}

