#include "PrecompileHeader.h"
#include "ShopBG.h"
#include "UIEnums.h"
ShopBG::ShopBG()
{
}

ShopBG::~ShopBG()
{
}

void ShopBG::Start()
{
	ShopBGRender = CreateComponent<GameEngineUIRenderer>(0);
	ShopBGRender->GetTransform()->SetLocalScale(Scale);
	ShopBGRender->SetTexture("ShopTestBG.png");
}


void ShopBG::Update(float _DeltaTime)
{

}

