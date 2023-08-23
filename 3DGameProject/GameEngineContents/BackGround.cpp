#include "PrecompileHeader.h"
#include "BackGround.h"
#include <GameEngineCore/GameEngineSpriteRenderer.h>
BackGround::BackGround() 
{
}

BackGround::~BackGround() 
{
}

void BackGround::Start()
{
	BGRender = CreateComponent<GameEngineSpriteRenderer>();
	BGRender->GetTransform()->SetLocalPosition(Pos);
	BGRender->SetScaleToTexture("BG.png");
}

void BackGround::Update(float _DeltaTime)
{
}

