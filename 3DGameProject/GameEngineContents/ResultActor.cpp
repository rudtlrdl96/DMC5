#include "PrecompileHeader.h"
#include "ResultActor.h"
#include <GameEngineCore/GameEngineSpriteRenderer.h>
ResultActor::ResultActor()
{
}

ResultActor::~ResultActor()
{
}

void ResultActor::Start()
{
	GetTransform()->SetLocalPosition(float4::FORWARD * 450.0f * GameEngineActor::ScreenRatio.y);
	GetTransform()->SetLocalScale(GameEngineActor::ScreenRatio);
	Renderer = CreateComponent<GameEngineSpriteRenderer>();
	Renderer->GetTransform()->SetLocalScale({ 1600, 900 });
	Renderer->SetTexture("Result_ScreenShot.jpg");
}

void ResultActor::Update(float _DeltaTime)
{
}
