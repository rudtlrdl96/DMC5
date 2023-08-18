#include "PrecompileHeader.h"
#include "MouseCusor.h"
#include "UIEnums.h"
#include <GameEngineCore/GameEngineSpriteRenderer.h>

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCollision.h>

MouseCusor::MouseCusor()
{
}

MouseCusor::~MouseCusor()
{
}


void MouseCusor::Start()
{

	MouseCusorRender = CreateComponent<GameEngineUIRenderer>(3);
	MouseCusorRender->SetTexture("NullTexture.png");
	MouseCusorRender->GetTransform()->SetLocalScale(MouseCusorScale);

	MouseCusorCollsion = CreateComponent<GameEngineCollision>(UICollisonOrder::Mouse);
	MouseCusorCollsion->GetTransform()->SetLocalScale({ 1.0f,1.0f,1.0f });
}


void MouseCusor::Update(float _DeltaTime)
{
	GameEngineCamera* Camera = MouseCusorRender->GetCamera();

	// ·£´õ·¯ 
	float4x4 ViewPort = Camera->GetViewPort();
	float4x4 Proj = Camera->GetProjection();
	float4x4 View = Camera->GetView();

	float4 Mouse = GameEngineInput::GetMousePosition();

	Mouse *= ViewPort.InverseReturn();
	Mouse *= Proj.InverseReturn();
	Mouse *= View.InverseReturn();

	MouseCusorRender->GetTransform()->SetLocalPosition({ Mouse.x,Mouse.y,0.0f});
	MouseCusorCollsion->GetTransform()->SetLocalPosition({ Mouse.x,Mouse.y,0.0f });
}

