#include "PrecompileHeader.h"
#include "TitleBG.h"
#include "UIEnums.h"
#include "UIFontRender.h"
TitleBG::TitleBG() 
{
}

TitleBG::~TitleBG() 
{
}

void TitleBG::Start()
{


	TitleBGRender = CreateComponent<GameEngineUIRenderer>(UIOrder::NORMAL);
	TitleBGRender->GetTransform()->SetLocalScale(Scale);
	TitleBGRender->SetTexture("TitleBG.png");

}


void TitleBG::Update(float _DeltaTime)
{
	
}

