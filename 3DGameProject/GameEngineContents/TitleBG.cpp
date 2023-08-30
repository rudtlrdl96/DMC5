#include "PrecompileHeader.h"
#include "TitleBG.h"
#include "UIEnums.h"
TitleBG::TitleBG()
{
}

TitleBG::~TitleBG()
{
}

void TitleBG::Start()
{
	TitleBGRender = CreateComponent<GameEngineUIRenderer>(0);
	TitleBGRender->GetTransform()->SetLocalScale(Scale);
	TitleBGRender->SetTexture("TitleBG.png");
}


void TitleBG::Update(float _DeltaTime)
{

}

