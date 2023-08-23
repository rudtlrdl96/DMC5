#include "PrecompileHeader.h"
#include "Intro_BackGround.h"
#include <GameEngineCore/GameEngineSpriteRenderer.h>
Intro_BackGround::Intro_BackGround()
{
}

Intro_BackGround::~Intro_BackGround()
{
}

void Intro_BackGround::Start()
{
	BGRender = CreateComponent<GameEngineSpriteRenderer>();
	BGRender->GetTransform()->SetLocalPosition(Pos);
	BGRender->SetScaleToTexture("IntroBG.png");
}

void Intro_BackGround::Update(float _DeltaTime)
{
}

