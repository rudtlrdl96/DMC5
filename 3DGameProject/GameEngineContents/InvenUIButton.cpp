#include "PrecompileHeader.h"
#include "InvenUIButton.h"
#include "UIEnums.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

InvenUIButton::InvenUIButton()
{
}

InvenUIButton::~InvenUIButton()
{
}

void InvenUIButton::Start()
{
	Render = CreateComponent<GameEngineUIRenderer>(6);
}

void InvenUIButton::Update(float _Delta)
{
	GameEngineCamera* Camera = Render->GetCamera();

	// ·£´õ·¯ 
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
		if (true == GameEngineInput::IsUp("CLICK"))
		{
			if (nullptr != Click)
			{
				Click();
			}
		}
	}
	else
	{

	}

	LerpTexture(StartPos, EndPos, Ratio, _Delta);
}

void InvenUIButton::LerpTexture(float4 _StartPos, float4 _EndPos, float _Ratio, float _Delta)
{
	if (SwichValue == false)
	{
		Speed += _Delta;
		float4 M0 = float4::LerpClamp(_StartPos, _EndPos, _Ratio* Speed);
		Render->GetTransform()->SetLocalPosition(M0);
	}
	if (Render->GetTransform()->GetLocalPosition() == _EndPos)
	{
		Speed = 0.0f;
		SwichValue = true;
	}
	if (SwichValue == true)
	{
		Speed += _Delta;
		float4 M1 = float4::LerpClamp(_EndPos, _StartPos, _Ratio* Speed);
		Render->GetTransform()->SetLocalPosition(M1);
		if (Render->GetTransform()->GetLocalPosition() == _StartPos)
		{
			Time += _Delta;
			if (Time > 0.5f)
			{
				SwichValue = false;
				Speed = 0.0f;
				Time = 0.0f;
			}
		}

	}
}


