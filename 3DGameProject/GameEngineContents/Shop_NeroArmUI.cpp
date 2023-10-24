#include "PrecompileHeader.h"
#include "Shop_NeroArmUI.h"
#include "Shop_TitleButton.h"
#include "Shop_ItemButton.h"
#include "Shop_ExplaneUI.h"
#include "Nero_InvenToryUI.h"
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "Nero_Inven.h"
#include "PlayerActor_Nero.h"
 std::function<void(DevilBreaker _Value)> Shop_NeroArmUI::CallBack_AddBreaker = nullptr;

Shop_NeroArmUI::Shop_NeroArmUI()
{
}

Shop_NeroArmUI::~Shop_NeroArmUI()
{
}

void Shop_NeroArmUI::Start()
{
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_0 * GameEngineActor::ScreenRatio, { ._Name = "OVERTURE",._Price = "500",._png = "Nero_Overture.png",._BaseNone = "Shop_ArmBase.png", ._BaseSelect = "Shop_ArmSelect.png",.IsValue = false }, GetTransform(), float4(118.0f, 120.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_1 * GameEngineActor::ScreenRatio, { ._Name = "GERBERA",._Price = "1200",._png = "Nero_Gerbera.png",._BaseNone = "Shop_ArmBase.png", ._BaseSelect = "Shop_ArmSelect.png" ,.IsValue = false }, GetTransform(), float4(118.0f, 120.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_2 * GameEngineActor::ScreenRatio, { ._Name = "BUSTER ARM",._Price = "5000",._png = "Nero_BusterArm.png",._BaseNone = "Shop_ArmBase.png", ._BaseSelect = "Shop_ArmSelect.png",.IsValue = false }, GetTransform(), float4(118.0f, 120.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	ArmExplane = GetLevel()->CreateActor<Shop_ExplaneUI>();
	ArmExplane->GetTransform()->SetParent(GetTransform());
	ArmExplane->GetTransform()->SetWorldPosition({ 0.0f,0.0f,0.0f });
	AcNeroInven = GetLevel()->CreateActor<Nero_Inven>();
	AcNeroInven->GetTransform()->SetParent(GetTransform());
	ArmList = PlayerActor_Nero::GetBreakerListPointer();

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
		//인자로 메인메뉴에서 들고있던 인벤 인덱스 넣어주기
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
		ArmExplane->SetExplaneText({ ._EName = "OVERTURE" ,._KName = "오버츄어" 
			,._Explane1 = "다용도로 사용가능한 데빌 브레이커.",
			._Explane2 = "전기 충격파로 적을 날리거나,"
			,._Explane3 = "적에게 찔러 넣고 폭발시킬 수 있다." });
		break;
	case 1:
		ArmExplane->SetExplaneText({ ._EName = "GERBERA" ,._KName = "거베라" ,
			._Explane1 = "이동력을 가진 데빌 브레이커."
			,._Explane2 = "큰 범위의 충격파로 공격을 무력화하거나"
			,._Explane3 = "강한 힘을 가진 광선을 발사한다." });
		break;
	case 2:
		ArmExplane->SetExplaneText({ ._EName = "BUSTER ARM" ,._KName = "버스트 암" ,._Explane1 = "화려한 검 콤보로",._Explane2 = "보다 넓은 범위에 걸쳐",._Explane3 = "적들을 해치웁니다." });
		break;

	default:
		break;
	}
}

void Shop_NeroArmUI::BuyBreaker()
{
	if (Index == 0)
	{
		if (ArmList->size() <= 4 &&GameEngineInput::IsUp("UI_Enter") == true)
		{
			CallBack_AddBreaker(DevilBreaker::Overture);
		}
	}
	else if (Index == 1)
	{
		if (ArmList->size() <= 4 && GameEngineInput::IsUp("UI_Enter") == true)
		{
			CallBack_AddBreaker(DevilBreaker::Gerbera);

		}
	}
	else if (Index == 2)
	{
		if (ArmList->size() <= 4 && GameEngineInput::IsUp("UI_Enter") == true)
		{
			CallBack_AddBreaker(DevilBreaker::BusterArm);

		}
	}
}

