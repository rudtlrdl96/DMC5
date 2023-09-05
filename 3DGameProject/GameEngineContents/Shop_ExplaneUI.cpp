#include "PrecompileHeader.h"
#include "Shop_ExplaneUI.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
Shop_ExplaneUI::Shop_ExplaneUI()
{
}

Shop_ExplaneUI::~Shop_ExplaneUI()
{
}

void Shop_ExplaneUI::SetExplaneText(const ExplaneParameter& _Param)
{
	E_NameRender->SetText(_Param._EName);
	K_NameRender->SetText(_Param._KName);
	ExplaneText0->SetText(_Param._Explane1);
	ExplaneText1->SetText(_Param._Explane2);
	ExplaneText2->SetText(_Param._Explane3);
}

void Shop_ExplaneUI::Start()
{
	ExplaneBar_0 = CreateComponent<GameEngineUIRenderer>(1);
	ExplaneBar_0->SetTexture("Menu_InfoBar.png");
	ExplaneBar_0->GetTransform()->SetLocalScale({ 736.0f,5.0f,0.0f });
	ExplaneBar_0->GetTransform()->SetLocalPosition({ 711.f, 223.0f, 0.0f });

	ExplaneBar_1 = CreateComponent<GameEngineUIRenderer>(1);
	ExplaneBar_1->SetTexture("Menu_InfoBar.png");
	ExplaneBar_1->GetTransform()->SetLocalScale({ 736.0f,5.0f,0.0f });
	ExplaneBar_1->GetTransform()->SetLocalPosition({ 711.f, -102.0f, 0.0f });
	FontCreate();
}

void Shop_ExplaneUI::Update(float _Delta)
{

}

void Shop_ExplaneUI::FontCreate()
{
	E_NameRender = CreateComponent<GameEngineFontRenderer>(2);
	E_NameRender->GetTransform()->SetParent(GetTransform());
	E_NameRender->SetFont("DMC5Font");
	E_NameRender->SetFontFlag(FW1_LEFT);
	E_NameRender->SetScale(28);
	E_NameRender->SetColor({ 0.69f,0.792f,0.827f });
	E_NameRender->GetTransform()->SetLocalPosition({ 367.0f,259.f,0.0f });

	K_NameRender = CreateComponent<GameEngineFontRenderer>(2);
	K_NameRender->GetTransform()->SetParent(GetTransform());
	K_NameRender->SetFont("³ª´®¹Ù¸¥°íµñ ¿¾ÇÑ±Û");
	K_NameRender->SetFontFlag(FW1_LEFT);
	K_NameRender->SetScale(20);
	K_NameRender->SetColor(float4::WHITE);
	K_NameRender->GetTransform()->SetLocalPosition({ 372.0f,215.f,0.0f });

	ExplaneText0 = CreateComponent<GameEngineFontRenderer>(2);
	ExplaneText0->GetTransform()->SetParent(GetTransform());
	ExplaneText0->SetFont("³ª´®¹Ù¸¥°íµñ ¿¾ÇÑ±Û");
	ExplaneText0->SetFontFlag(FW1_LEFT);
	ExplaneText0->SetScale(20);
	ExplaneText0->SetColor(float4::WHITE);
	ExplaneText0->GetTransform()->SetLocalPosition({ 372.0f,158.f,0.0f });

	ExplaneText1 = CreateComponent<GameEngineFontRenderer>(2);
	ExplaneText1->GetTransform()->SetParent(GetTransform());
	ExplaneText1->SetFont("³ª´®¹Ù¸¥°íµñ ¿¾ÇÑ±Û");
	ExplaneText1->SetFontFlag(FW1_LEFT);
	ExplaneText1->SetScale(20);
	ExplaneText1->SetColor(float4::WHITE);
	ExplaneText1->GetTransform()->SetLocalPosition({ 372.0f,122.f,0.0f });

	ExplaneText2 = CreateComponent<GameEngineFontRenderer>(2);
	ExplaneText2->GetTransform()->SetParent(GetTransform());
	ExplaneText2->SetFont("³ª´®¹Ù¸¥°íµñ ¿¾ÇÑ±Û");
	ExplaneText2->SetFontFlag(FW1_LEFT);
	ExplaneText2->SetScale(20);
	ExplaneText2->SetColor(float4::WHITE);
	ExplaneText2->GetTransform()->SetLocalPosition({ 372.0f,86.f,0.0f });
}

