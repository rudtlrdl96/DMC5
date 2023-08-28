#include "PrecompileHeader.h"
#include "Menu_NeroInfo.h"

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

}

void Menu_NeroInfo::Update(float _Delta)
{
}

