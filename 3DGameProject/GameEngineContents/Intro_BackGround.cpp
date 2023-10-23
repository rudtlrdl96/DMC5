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
	BGRender->SetTexture("IntroBG.png");
	BGRender->GetTransform()->SetLocalScale(Scale * GameEngineActor::ScreenRatio);
}

void Intro_BackGround::Update(float _DeltaTime)
{
}

