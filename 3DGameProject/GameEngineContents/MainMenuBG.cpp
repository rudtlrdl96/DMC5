#include "PrecompileHeader.h"
#include "MainMenuBG.h"
#include "UIEnums.h"
MainMenuBG::MainMenuBG()
{
}

MainMenuBG::~MainMenuBG()
{
}

void MainMenuBG::Start()
{


	MainMenuBGRender = CreateComponent<GameEngineUIRenderer>(0);
	MainMenuBGRender->SetScaleToTexture("MainMenuBG2.png");
	MainMenuBGRender->BSCControl(0.5f, 0.5f, 0.5f);
}


void MainMenuBG::Update(float _DeltaTime)
{

}

