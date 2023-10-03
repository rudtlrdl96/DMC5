#include "PrecompileHeader.h"
#include "C_ChoiceBG.h"
#include "UIEnums.h"
C_ChoiceBG::C_ChoiceBG()
{
}

C_ChoiceBG::~C_ChoiceBG()
{
}

void C_ChoiceBG::Start()
{

	C_ChoiceBGRender = CreateComponent<GameEngineUIRenderer>(0);
	C_ChoiceBGRender->GetTransform()->SetLocalScale(Scale);
	C_ChoiceBGRender->SetTexture("CharChoise_Base.png");
}


void C_ChoiceBG::Update(float _DeltaTime)
{

}

