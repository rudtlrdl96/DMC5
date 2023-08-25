#include "PrecompileHeader.h"
#include "Menu_MissionFont.h"

#include <GameEngineCore/GameEngineFontRenderer.h>
Menu_MissionFont::Menu_MissionFont() 
{
}

Menu_MissionFont::~Menu_MissionFont() 
{
}

void Menu_MissionFont::Start()
{
	MFont = CreateComponent<GameEngineFontRenderer>(6);
	MFont->SetText("M");
	MFont->SetFont("DMC5Font");
	MFont->SetScale(160.0f);
	MFont->GetTransform()->SetLocalPosition({ 0.0f,65.f,0.0f });
	MFont->SetFontFlag(FW1_CENTER);
	MFont->SetColor(Color);
	OtherFont = CreateComponent<GameEngineFontRenderer>(6);
	OtherFont->GetTransform()->SetLocalPosition({ 150.f,0.0f,0.0f });
	OtherFont->SetFont("DMC5Font");
	OtherFont->SetFontFlag(FW1_CENTER);
	OtherFont->SetText("ISSION");
	OtherFont->SetColor(Color);
	OtherFont->SetScale(90.0f);
}

void Menu_MissionFont::Update(float _Delta)
{
}

