#include "PrecompileHeader.h"
#include "Menu_VirgilInfo.h"

#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>

Menu_VirgilInfo::Menu_VirgilInfo()
{
}

Menu_VirgilInfo::~Menu_VirgilInfo()
{
}

void Menu_VirgilInfo::Start()
{
	VergilInfoPtr = GetLevel()->CreateActor<InfoUIRender>();
	VergilInfoPtr->GetTransform()->SetParent(GetTransform());
	VergilInfoPtr->GetRender()->SetTexture("VirgilLogo.png");
	VergilInfoPtr->GetRender_Bar()->SetTexture("Menu_InfoBar.png");
	VergilInfoPtr->GetRender_Base()->SetTexture("Menu_difficultyBase.png");
	VergilInfoPtr->GetRender_Base()->ColorOptionValue.MulColor.a = 0.7f;
	VergilInfoPtr->SetUIText({ ._Text = "PLAYER",._SeletText = "VERGILE",._ExplaneText = "GAME MODE",._ExplaneText2 = "DEVIL HUNTER" });
}

void Menu_VirgilInfo::Update(float _Delta)
{

	
}

