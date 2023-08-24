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
	MainMenuBGRender->SetScaleToTexture("ui4005_im.texout.png");
	MainMenuBGRender->BSCControl(1.0f, 0.55f, 0.5f);
}


void MainMenuBG::Update(float _DeltaTime)
{

}

