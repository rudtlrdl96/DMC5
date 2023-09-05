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
	ShopBGRender0 = CreateComponent<GameEngineUIRenderer>(0);
	ShopBGRender0->SetScaleToTexture("ShopBlackBG.png");
	ShopBGRender1 = CreateComponent<GameEngineUIRenderer>(0);
	ShopBGRender1->SetScaleToTexture("ShopBG.png");
	ShopBGRender2 = CreateComponent<GameEngineUIRenderer>(8);
	ShopBGRender2->SetScaleToTexture("ShopFrontBG.png");

}


void ShopBG::Update(float _DeltaTime)
{

}

