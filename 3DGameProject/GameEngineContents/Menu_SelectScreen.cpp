#include "PrecompileHeader.h"
#include "Menu_SelectScreen.h"
#include "NetworkManager.h"
#include "ContentsEnum.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include "UIButton.h"
Menu_SelectScreen::Menu_SelectScreen()
{
}

Menu_SelectScreen::~Menu_SelectScreen()
{
}
void Menu_SelectScreen::Start()
{
	MissionButton = GetLevel()->CreateActor<UIButton>();
	MissionButton->GetTransform()->SetLocalPosition(float4{ -670.0f,150.0f,0.0f }*GameEngineActor::ScreenRatio);
	MissionButton->GetRender()->SetTexture("NullTexture.png");
	MissionButton->GetRender()->GetTransform()->SetLocalScale(float4{ 200.0f, 40.0f, 0.0f }*GameEngineActor::ScreenRatio);
	MissionButton->GetRender_Select()->SetTexture("NullTexture.png");
	MissionButton->GetRender_Select()->GetTransform()->SetLocalScale(SelectBaseScale * GameEngineActor::ScreenRatio);
	MissionButton->GetRender_Select()->GetTransform()->SetLocalPosition(SelectBasePos * GameEngineActor::ScreenRatio);
	MissionButton->GetRender_Select2()->SetTexture("Menu_SelectBase.png");
	MissionButton->GetRender_Select2()->GetTransform()->SetLocalScale(SelectBaseScale*GameEngineActor::ScreenRatio);
	MissionButton->GetRender_Select2()->GetTransform()->SetLocalPosition(float4{ 250.0f,0.0f,0.0f }*GameEngineActor::ScreenRatio);
	MissionButton->GetRender_Enter()->SetTexture("T_EnterBase.png");
	MissionButton->GetRender_Enter()->GetTransform()->SetLocalScale(T_EnterScale* GameEngineActor::ScreenRatio);
	MissionButton->GetRender_Enter()->GetTransform()->SetLocalPosition(SelectBasePos);
	MissionButton->GetRender_Enter2()->SetTexture("T_EnterBase.png");
	MissionButton->GetRender_Enter2()->GetTransform()->SetLocalScale(T_EnterScale* GameEngineActor::ScreenRatio);
	MissionButton->GetRender_Enter2()->GetTransform()->SetLocalPosition(float4{ 200.0f,0.0f,0.0f }*GameEngineActor::ScreenRatio);
	MissionButton->SetEvent([this]()
		{
			GameEngineCore::ChangeLevel("TestStageLevel");
		});
	CustomizeButton = GetLevel()->CreateActor<UIButton>();
	CustomizeButton->GetTransform()->SetLocalPosition(float4{ -670.0f,90.0f,0.0f }*GameEngineActor::ScreenRatio);
	CustomizeButton->GetRender()->SetTexture("NullTexture.png");
	CustomizeButton->GetRender()->GetTransform()->SetLocalScale(float4{ 200.0f,40.0f,0.0f }*GameEngineActor::ScreenRatio);
	CustomizeButton->GetRender_Select()->SetTexture("NullTexture.png");
	CustomizeButton->GetRender_Select()->GetTransform()->SetLocalScale(float4{ 800.0f,50.0f,0.0f }*GameEngineActor::ScreenRatio);
	CustomizeButton->GetRender_Select()->GetTransform()->SetLocalPosition(SelectBasePos * GameEngineActor::ScreenRatio);
	CustomizeButton->GetRender_Select2()->SetTexture("Menu_SelectBase.png");
	CustomizeButton->GetRender_Select2()->GetTransform()->SetLocalScale(float4{ 579.0f,50.0f,0.0f }*GameEngineActor::ScreenRatio);
	CustomizeButton->GetRender_Select2()->GetTransform()->SetLocalPosition(float4{ 250.0f,0.0f,0.0f }*GameEngineActor::ScreenRatio);
	CustomizeButton->SetEvent([this]()
		{
			GameEngineCore::ChangeLevel("ShopLevel");
		});
	PrevButton = GetLevel()->CreateActor<UIButton>();
	PrevButton->GetTransform()->SetLocalPosition(float4{ -670.0f,30.0f,0.0f }*GameEngineActor::ScreenRatio);
	PrevButton->GetRender()->GetTransform()->SetLocalScale(float4{ 200.0f,40.0f,0.0f }*GameEngineActor::ScreenRatio);
	PrevButton->GetRender()->SetTexture("NullTexture.png");
	PrevButton->GetRender_Select()->SetTexture("NullTexture.png");
	PrevButton->GetRender_Select()->GetTransform()->SetLocalScale(float4{ 800.0f,50.0f,0.0f }*GameEngineActor::ScreenRatio);
	PrevButton->GetRender_Select()->GetTransform()->SetLocalPosition(SelectBasePos * GameEngineActor::ScreenRatio);
	PrevButton->GetRender_Select2()->SetTexture("Menu_SelectBase.png");
	PrevButton->GetRender_Select2()->GetTransform()->SetLocalScale(float4{ 579.0f,50.0f,0.0f }*GameEngineActor::ScreenRatio);
	PrevButton->GetRender_Select2()->GetTransform()->SetLocalPosition(float4{ 250.0f,0.0f,0.0f }*GameEngineActor::ScreenRatio);
	SetFontText();

}

void Menu_SelectScreen::Update(float _DeltaTime)
{
	SetButtonIndex();
	if (GameEngineInput::IsDown("UI_UP"))
	{
		if (SelectValue == 0)
		{
			SelectValue = 2;
		}
		else
		{
			SelectValue--;
		}
	}
	if (GameEngineInput::IsDown("UI_Down"))
	{
		if (SelectValue == 2)
		{
			SelectValue = 0;
		}
		else
		{
			SelectValue++;
		}
	}


}

void Menu_SelectScreen::SetFontText()
{
	MissionButton->GetText()->SetFontFlag(FW1_LEFT);
	MissionButton->GetText()->GetTransform()->SetLocalPosition(FontPos * GameEngineActor::ScreenRatio);
	MissionButton->GetSeletText()->GetTransform()->SetLocalPosition(FontScale * GameEngineActor::ScreenRatio);
	MissionButton->SetExplanePos(ExplanePos * GameEngineActor::ScreenRatio);
	MissionButton->SetExFontFlag(FW1_LEFT);
	MissionButton->GetSeletText()->SetFontFlag(FW1_LEFT);
	MissionButton->GetText()->SetScale(36.0f * GameEngineActor::ScreenRatio.x);
	MissionButton->GetSeletText()->SetScale(40.0f * GameEngineActor::ScreenRatio.x);
	MissionButton->SetUIText({ ._Text = "MISSION START",._SeletText = "MISSION START",
	._ExplaneText = "미션을 시작합니다." });

	CustomizeButton->GetText()->SetFontFlag(FW1_LEFT);
	CustomizeButton->GetText()->GetTransform()->SetLocalPosition(FontPos * GameEngineActor::ScreenRatio);
	CustomizeButton->GetSeletText()->GetTransform()->SetLocalPosition(FontScale * GameEngineActor::ScreenRatio);
	CustomizeButton->SetExplanePos(ExplanePos * GameEngineActor::ScreenRatio);
	CustomizeButton->GetSeletText()->SetFontFlag(FW1_LEFT);
	CustomizeButton->SetExFontFlag(FW1_LEFT);
	CustomizeButton->GetText()->SetScale(36.0f * GameEngineActor::ScreenRatio.x);
	CustomizeButton->GetSeletText()->SetScale(40.0f * GameEngineActor::ScreenRatio.x);
	CustomizeButton->SetUIText({ ._Text = "CUSTOMIZE",._SeletText = "CUSTOMIZE",._ExplaneText = "캐릭터를 커스터마이징합니다." });
	
	PrevButton->GetText()->SetFontFlag(FW1_LEFT);
	PrevButton->GetText()->GetTransform()->SetLocalPosition(FontPos * GameEngineActor::ScreenRatio);
	PrevButton->GetSeletText()->GetTransform()->SetLocalPosition(FontScale * GameEngineActor::ScreenRatio);
	PrevButton->SetExplanePos(ExplanePos * GameEngineActor::ScreenRatio);
	PrevButton->GetSeletText()->SetFontFlag(FW1_LEFT);
	PrevButton->SetExFontFlag(FW1_LEFT);
	PrevButton->GetText()->SetScale(36.0f * GameEngineActor::ScreenRatio.x);
	PrevButton->GetSeletText()->SetScale(40.0f * GameEngineActor::ScreenRatio.x);
	PrevButton->SetUIText({ ._Text = "PREV MENU",._SeletText = "PREV MENU",._ExplaneText = "이전 메뉴로 돌아갑니다." });
}

void Menu_SelectScreen::SetButtonIndex()
{
	if (true == MissionButton->GetIsValue())
	{
		SelectValue = 0;
		MissionButton->SetIsValue(false);
	}
	if (true == CustomizeButton->GetIsValue())
	{
		SelectValue = 1;
		CustomizeButton->SetIsValue(false);
	}
	if (true == PrevButton->GetIsValue())
	{
		SelectValue = 2;
		PrevButton->SetIsValue(false);
	}
	if (SelectValue == 0)
	{
		MissionButton->SetSelectValue(true);
	}
	else
	{
		MissionButton->SetSelectValue(false);
	}
	if (SelectValue == 1)
	{
		CustomizeButton->SetSelectValue(true);
	}
	else
	{
		CustomizeButton->SetSelectValue(false);
	}
	if (SelectValue == 2)
	{
		PrevButton->SetSelectValue(true);
	}
	else
	{
		PrevButton->SetSelectValue(false);
	}
}

