#include "PrecompileHeader.h"
#include "Shop_ExplaneUI.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include "Nero_InvenToryUI.h"
Shop_ExplaneUI::Shop_ExplaneUI()
{
}

Shop_ExplaneUI::~Shop_ExplaneUI()
{
}

void Shop_ExplaneUI::SetExplaneText(const ExplaneOneParameter& _Param)
{
	E_NameRender->SetText(_Param._EName);
	K_NameRender->SetText(_Param._KName);
	ExplaneText0->SetText(_Param._Explane1);
	ExplaneText1->SetText(_Param._Explane2);
	ExplaneText2->SetText(_Param._Explane3);
	ExplaneText3->SetText(_Param._Explane4);
	ExplaneText4->SetText(_Param._Explane5);
	ExplaneText5->SetText(_Param._Explane6);
}

void Shop_ExplaneUI::SetSecoundText(const ExplaneOneParameter& _Param)
{
	SecoundExPlane->SetText(_Param._EName);
	S_ExplaneRender0->SetScaleToTexture(_Param._Explane1);
	S_ExplaneText0->SetText(_Param._Explane2);
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

	S_ExplaneRender0 = CreateComponent<GameEngineUIRenderer>(1);
	S_ExplaneRender0->GetTransform()->SetLocalPosition({ 711.f, -150.0f, 0.0f });

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
	
	ExplaneText3 = CreateComponent<GameEngineFontRenderer>(2);
	ExplaneText3->GetTransform()->SetParent(GetTransform());
	ExplaneText3->SetFont("³ª´®¹Ù¸¥°íµñ ¿¾ÇÑ±Û");
	ExplaneText3->SetFontFlag(FW1_LEFT);
	ExplaneText3->SetScale(20);
	ExplaneText3->SetColor(float4::WHITE);
	ExplaneText3->GetTransform()->SetLocalPosition({ 372.0f,50.f,0.0f });

	ExplaneText4 = CreateComponent<GameEngineFontRenderer>(2);
	ExplaneText4->GetTransform()->SetParent(GetTransform());
	ExplaneText4->SetFont("³ª´®¹Ù¸¥°íµñ ¿¾ÇÑ±Û");
	ExplaneText4->SetFontFlag(FW1_LEFT);
	ExplaneText4->SetScale(20);
	ExplaneText4->SetColor(float4::WHITE);
	ExplaneText4->GetTransform()->SetLocalPosition({ 372.0f,24.f,0.0f });

	ExplaneText5 = CreateComponent<GameEngineFontRenderer>(2);
	ExplaneText5->GetTransform()->SetParent(GetTransform());
	ExplaneText5->SetFont("³ª´®¹Ù¸¥°íµñ ¿¾ÇÑ±Û");
	ExplaneText5->SetFontFlag(FW1_LEFT);
	ExplaneText5->SetScale(20);
	ExplaneText5->SetColor(float4::WHITE);
	ExplaneText5->GetTransform()->SetLocalPosition({ 372.0f,-12.f,0.0f });

	SecoundExPlane = CreateComponent<GameEngineFontRenderer>(2);
	SecoundExPlane->GetTransform()->SetParent(GetTransform());
	SecoundExPlane->SetFont("DMC5Font");
	SecoundExPlane->SetFontFlag(FW1_LEFT);
	SecoundExPlane->SetScale(28);
	SecoundExPlane->SetColor({ 0.69f,0.792f,0.827f });
	SecoundExPlane->GetTransform()->SetLocalPosition({ 367.0f,-66.f,0.0f }); 
	
	S_ExplaneText0 = CreateComponent<GameEngineFontRenderer>(2);
	S_ExplaneText0->GetTransform()->SetParent(GetTransform());
	S_ExplaneText0->SetFont("DMC5Font");
	S_ExplaneText0->SetFontFlag(FW1_LEFT);
	S_ExplaneText0->SetScale(28);
	S_ExplaneText0->SetColor({ 0.305f,0.96f,0.94f,1.0f });
	S_ExplaneText0->GetTransform()->SetLocalPosition({ 493.0f,-133.f,0.0f });
}

