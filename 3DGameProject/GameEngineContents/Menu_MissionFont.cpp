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
	ExPlaneTex->SetTexture("extext.png");
	ExPlaneTex->GetTransform()->SetLocalScale(Scale * GameEngineActor::ScreenRatio);
	ExPlaneTex->GetTransform()->SetLocalPosition(Pos*GameEngineActor::ScreenRatio);
	ExPlaneTex->BSCControl(0.5f, 0.5f, 0.5f);
}

void Menu_MissionFont::Update(float _Delta)
{
}

