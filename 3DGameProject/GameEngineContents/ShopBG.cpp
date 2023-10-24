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
	ShopBGRender0->SetTexture("ShopBlackBG.png");
	ShopBGRender0->GetTransform()->SetLocalScale(Scale * GameEngineActor::ScreenRatio);
	//ShopBGRender1 = CreateComponent<GameEngineUIRenderer>(0);
	//ShopBGRender1->SetScaleToTexture("ShopBG.png");
	ShopBGRender2 = CreateComponent<GameEngineUIRenderer>(8);
	ShopBGRender2->SetScaleToTexture("ShopFrontBG.png");
	ShopBGRender2->GetTransform()->SetLocalScale(Scale * GameEngineActor::ScreenRatio);

}


void ShopBG::Update(float _DeltaTime)
{

}

