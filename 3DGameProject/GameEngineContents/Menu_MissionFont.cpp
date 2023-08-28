#include "PrecompileHeader.h"
#include "Menu_MissionFont.h"

#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
Menu_MissionFont::Menu_MissionFont() 
{
}

Menu_MissionFont::~Menu_MissionFont() 
{
}

void Menu_MissionFont::Start()
{

	ExPlaneTex = CreateComponent<GameEngineUIRenderer>(0);
	ExPlaneTex->SetScaleToTexture("extext.png");
	ExPlaneTex->GetTransform()->SetLocalPosition({ 200.0f,-50.0f,0.0f });
	ExPlaneTex->BSCControl(0.5f, 0.5f, 0.5f);
}

void Menu_MissionFont::Update(float _Delta)
{
}

