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
	MainMenuBGRender->SetTexture("MainMenuBG2.png");
	MainMenuBGRender->GetTransform()->SetLocalScale(Scale * GameEngineActor::ScreenRatio);
	MainMenuBGRender->BSCControl(0.5f, 0.5f, 0.5f);
}


void MainMenuBG::Update(float _DeltaTime)
{

}

