#include "PrecompileHeader.h"
#include "ResultActor.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
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
	{
		ScreenShotRenderer = CreateComponent<GameEngineFBXRenderer>();
		ScreenShotRenderer->GetTransform()->SetLocalScale({ 16, 9 });
		ScreenShotRenderer->SetFBXMesh("shape_plane.fbx", "FBX");
		ScreenShotRenderer->SetTexture("DiffuseTexture", "Result_ScreenShot.jpg");
		ScreenShotRenderer->SetTexture("NormalTexture", "gun_center_nm.texout.tga");
	}

	{
		GlassRenderer = CreateComponent<GameEngineFBXRenderer>();
		GlassRenderer->GetTransform()->SetLocalScale({ 9, 9 });
		GlassRenderer->GetTransform()->SetLocalPosition({ 0, 0, -1 });
		GlassRenderer->SetFBXMesh("shape_plane.fbx", "FBX_Alpha");
		GlassRenderer->SetTexture("DiffuseTexture", "ui3230_03_albm.texout.tga");
		GlassRenderer->SetTexture("SpecularTexture", "ui3230_03_atos.texout.tga");
	}

	{
		GlassParts_0 = CreateComponent<GameEngineFBXRenderer>();
		GlassParts_0->SetFBXMesh("Result_Glass_0.fbx", "FBX_Alpha");
	}


	GlassRenderer->Off();

	GameEngineTimeEvent& TimeEvent = GetLevel()->TimeEvent;

	TimeEvent.AddEvent(1.0f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* TimeEvent)
		{
			TimeEvent->AddEvent(0.2f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _TimeEvent)
				{
					GameEngineSound::Play("Result_SFX_0.wav");
					GameEngineSound::Play("Result_SFX_1.wav");
					ScreenShotRenderer->GetRenderBaseValueRef().IsNormal = 1;
					GlassRenderer->On();
				});

			TimeEvent->AddEvent(1.2f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _TimeEvent)
				{
					GameEngineSound::Play("Result_SFX_2.wav");
					GameEngineSound::Play("Result_SFX_3.wav");
				});

		});
}

void ResultActor::Update(float _DeltaTime)
{
}
