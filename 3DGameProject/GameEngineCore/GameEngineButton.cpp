#include "PrecompileHeader.h"
#include "GameEngineButton.h"
#include "GameEngineUIRenderer.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include "GameEngineLevel.h"
#include "GameEngineCamera.h"

GameEngineButton* GameEngineButton::SelectButton = nullptr;

GameEngineButton::GameEngineButton() 
{
}

GameEngineButton::~GameEngineButton() 
{
}

void GameEngineButton::Start() 
{
	Render = CreateComponent<GameEngineUIRenderer>();
	// Collision = CreateComponent<GameEngineCollision>();
}

void GameEngineButton::Update(float _Delta)
{
	float4 Scale = Render->GetTransform()->GetWorldScale();
	float4 Pos = Render->GetTransform()->GetWorldPosition();

	// 랜더러 => 크 자 이 공 부  월드 => 뷰 => 투영 => 뷰포트
	//                                               스크린
	//                                               마우스

	// 이카메라가 원근 투영이면
	GameEngineCamera* Camera = Render->GetCamera();

	// 랜더러 
	float4x4 ViewPort = Camera->GetViewPort();
	float4x4 Proj = Camera->GetProjection();
	float4x4 View = Camera->GetView();

	float4 Mouse = GameEngineInput::GetMousePosition();

	Mouse *= ViewPort.InverseReturn();
	Mouse *= Proj.InverseReturn();
	Mouse *= View.InverseReturn();


	CollisionData MouseData;
	MouseData.SPHERE.Center = Mouse.DirectFloat3;
	MouseData.SPHERE.Radius = 0.0f;

	if (true == GameEngineTransform::AABB2DToSpehre2D(Render->GetTransform()->GetCollisionData(), MouseData))
	{
		if(HoverImage != "")
		{
			Render->SetTexture(HoverImage);
		}

		if (true == GameEngineInput::IsPress("EngineMouseLeft"))
		{
			if(PressImage != "")
			{
				Render->SetTexture(PressImage);
			}
		}

		if (true == GameEngineInput::IsUp("EngineMouseLeft"))
		{
			if (nullptr != Click)
			{
				Click();
			}
		}
	}
	else
	{
		if (ReleaseImage != "")
		{
			Render->SetTexture(ReleaseImage);
		}
	}

	// / ;/  GameEngineTransform::sphe

	// Render->GetTransform()->Collision()

	// Render->GetTransform();
}


