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
	NeroInfoPtr = GetLevel()->CreateActor<UIInfoBase>();
	NeroInfoPtr->GetRender()->SetTexture("NeroLogo.png");
	NeroInfoPtr->GetRender()->GetTransform()->SetLocalScale(float4(170.f, 200.f, 1.0f));
	NeroInfoPtr->GetRender()->GetTransform()->SetWorldPosition(float4(700.f, 50.f, 0.0f));
	NeroInfoPtr->GetRender()->BSCControl(0.3f, 0.4f, 0.5f);
	NeroInfoPtr->GetRender_Bar()->SetTexture("Menu_InfoBar.png");
	NeroInfoPtr->GetRender_Bar()->GetTransform()->SetLocalScale(float4(546.f, 5.f, 1.0f));
	NeroInfoPtr->GetRender_Bar()->GetTransform()->SetWorldPosition(float4(700.f, 90.f, 0.0f));
	NeroInfoPtr->GetRender_Base()->BSCControl(0.3f, 0.4f, 0.5f);
	NeroInfoPtr->GetRender_Base()->SetTexture("Menu_difficultyBase.png");
	NeroInfoPtr->GetRender_Base()->GetTransform()->SetLocalScale(float4(546.f, 40.f, 1.0f));
	NeroInfoPtr->GetRender_Base()->GetTransform()->SetWorldPosition(float4(700.f, 30.f, 0.0f));
	NeroInfoPtr->GetRender_Base()->ColorOptionValue.MulColor.a = 0.7f;
	NeroInfoPtr->GetRender_Base()->BSCControl(0.3f, 0.4f, 0.5f);
}

void Menu_NeroInfo::Update(float _Delta)
{
}

