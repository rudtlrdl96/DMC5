#include "PrecompileHeader.h"
#include "ResultActor.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCaptureTexture.h>
ResultActor::ResultActor()
{
}

ResultActor::~ResultActor()
{
}

void ResultActor::Start()
{
	GetTransform()->SetLocalPosition(float4::FORWARD * 450.0f * GameEngineActor::ScreenRatio.y);
	GetTransform()->SetLocalScale(float4::ONE * GameEngineActor::ScreenRatio.y);

	{
		SkyBox = CreateComponent<GameEngineFBXRenderer>();
		SkyBox->SetFBXMesh("l02_SkyBox.fbx", "FBX");
		SkyBox->SetTexture("DiffuseTexture", "resultfinal_bg_im.texout.tga");
	}

	{
		ScreenShotRenderer = CreateComponent<GameEngineFBXRenderer>();
		ScreenShotRenderer->GetTransform()->SetLocalScale({ 16, 9 });
		ScreenShotRenderer->SetFBXMesh("shape_plane.fbx", "FBX");
		std::shared_ptr<GameEngineRenderTarget> CaptureTarget = GameEngineCaptureTexture::FindTexture("Capture_Result");
		if (nullptr != CaptureTarget)
		{
			ScreenShotRenderer->SetTexture("DiffuseTexture", CaptureTarget->GetTexture(0));
		}
		else
		{
			ScreenShotRenderer->SetTexture("DiffuseTexture", "Result_ScreenShot.jpg");
		}
		ScreenShotRenderer->SetTexture("NormalTexture", "gun_center_nm.texout.tga");
	}

	{
		GlassRenderer = CreateComponent<GameEngineFBXRenderer>();
		GlassRenderer->GetTransform()->SetLocalScale({ 16, 9 });
		GlassRenderer->GetTransform()->SetLocalPosition({ 0, 0, -1 });
		GlassRenderer->SetFBXMesh("shape_plane.fbx", "FBX_Alpha");
		GlassRenderer->SetTexture("DiffuseTexture", "ui3230_03_albm.texout.tga");
		GlassRenderer->SetTexture("SpecularTexture", "ui3230_03_atos.texout.tga");
	}

	{
		GlassParts_0 = CreateComponent<GameEngineFBXRenderer>();
		GlassParts_0->SetFBXMesh("Result_Glass_0.fbx", "FBX_Alpha");
	}

	{
		GlassParts_1 = CreateComponent<GameEngineFBXRenderer>();
		GlassParts_1->SetFBXMesh("Result_Glass_0.fbx", "FBX_Alpha");
		GlassParts_1->GetTransform()->SetLocalPosition({ 0, 0, -800 });
		GlassParts_1->GetTransform()->SetLocalScale({ 2.5f, 2.5f, 2.5f });
		GlassParts_1->Off();
	}

	{
		RankBackgroundRenderer = CreateComponent<GameEngineFBXRenderer>();
		RankBackgroundRenderer->SetFBXMesh("shape_plane.fbx", "FBX");
		RankBackgroundRenderer->SetTexture("DiffuseTexture", "endglass_bm.texout.tga");
		RankBackgroundRenderer->SetTexture("NormalTexture", "endglass_nrmr.texout.tga");
		RankBackgroundRenderer->GetRenderBaseValueRef().IsNormal = 1;
		RankBackgroundRenderer->GetTransform()->SetLocalPosition({ 1500, 0, -1000 });
		RankBackgroundRenderer->GetTransform()->SetLocalRotation({ 0, 90, 0 });
		RankBackgroundRenderer->GetTransform()->SetLocalScale({ 16, 9 });
		RankBackgroundRenderer->Off();
	}

	{
		RankGrade = CreateComponent<GameEngineFBXRenderer>();
		RankGrade->SetFBXMesh("Result_S.fbx", "FBX");
		RankGrade->GetTransform()->SetLocalRotation({ 0, 98, 0 });
		RankGrade->Off();
	}


FSM.CreateState({ .StateValue = ResultState_Wait,
.Start = [this]
{
	GetLevel()->GetMainCamera()->GetTransform()->SetWorldPosition(float4::ZERO);
	GetLevel()->GetMainCamera()->GetTransform()->SetWorldRotation(float4::ZERO);
	ScreenShotRenderer->GetRenderBaseValueRef().IsNormal = 0;
	GlassRenderer->Off();
	GlassParts_0->Off();
	GlassParts_1->Off();
	RankGrade->Off();
	RankBackgroundRenderer->Off();
	GetLevel()->TimeEvent.AddEvent(1.0f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* TimeEvent)
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
					GameEngineSound::Play("Result_SFX_5.wav");
					FSM.ChangeState(ResultState_Shoot);
				});
			TimeEvent->AddEvent(1.34f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _TimeEvent)
				{
					CameraRotLock = false;
				});
			TimeEvent->AddEvent(1.6f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _TimeEvent)
				{
					GameEngineSound::Play("Result_SFX_6.wav");
					FSM.ChangeState(ResultState_Shoot2);
				});

			TimeEvent->AddEvent(3.0f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _TimeEvent)
				{
					GameEngineSound::Play("Result_SFX_5.wav");
					FSM.ChangeState(ResultState_Rank1);
				});

			TimeEvent->AddEvent(3.2f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _TimeEvent)
				{
					GameEngineSound::Play("Result_SFX_9.wav");
				});
		});
},
.Update = [=](float _DeltaTime)
{

},
.End = [=]
{

}
});

static GameEngineTransform* CamTransform = nullptr;
static float Timer = 0;
static float4 StartPos;
static float4 StartRot;
FSM.CreateState({ .StateValue = ResultState_Shoot,
.Start = [this]
{
	CamTransform = GetLevel()->GetMainCamera()->GetTransform();
	Timer = 0;
	CameraRotLock = true;
	StartPos = CamTransform->GetWorldPosition();
	StartRot = CamTransform->GetWorldRotation();
	GlassParts_0->GetTransform()->SetLocalPosition({0, 0, 150});
	GlassParts_0->GetTransform()->SetLocalRotation({90, 0, 0});
	GlassParts_0->On();
},
.Update = [=](float _DeltaTime)
{
	if (false == CameraRotLock)
	{
		Timer += _DeltaTime;
		float Ratio = std::sinf(Timer * GameEngineMath::PIE * 0.5f);
		CamTransform->SetWorldPosition(float4::Lerp(StartPos, { -100, 0, 40 }, Ratio));
		CamTransform->SetWorldRotation(float4::Lerp(StartRot, { 0, -179, 0 }, Ratio));
	}

	GlassParts_0->GetTransform()->AddLocalPosition({0, 0, -4000 * _DeltaTime});
	GlassParts_0->GetTransform()->AddLocalScale(float4::ONE * 4 * _DeltaTime);
},
.End = [=]
{

}
});

FSM.CreateState({ .StateValue = ResultState_Shoot2,
.Start = [this]
{
	GlassParts_0->Off();
	GlassParts_1->On();
	GlassParts_1->GetTransform()->SetLocalPosition({ 0, 0, -800 });
	},
.Update = [=](float _DeltaTime)
{
	Timer += _DeltaTime * 0.714f;
	float Ratio = std::sinf(std::min<float>(1.0f, Timer) * GameEngineMath::PIE * 0.5f);
	CamTransform->SetWorldPosition(float4::Lerp(StartPos, { -100, 0, 40 }, Ratio));
	CamTransform->SetWorldRotation(float4::Lerp(StartRot, { 0, -179, 0 }, Ratio));

	GlassParts_1->GetTransform()->AddLocalPosition({-50 * _DeltaTime, 0, 0 });
},
.End = [=]
{

}
});


EndPos = RankBackgroundRenderer->GetTransform()->GetWorldPosition() + (float4::LEFT * 350.0f * GameEngineActor::ScreenRatio.y);

FSM.CreateState({ .StateValue = ResultState_Rank1,
.Start = [this]
{
	RankBackgroundRenderer->On();
	RankGrade->GetTransform()->SetLocalPosition(float4::ZERO);
	RankGrade->On();

	CamTransform->SetWorldPosition({ -100, 0, 40 });
	CamTransform->SetWorldRotation({ 0, 179, 0 });

	StartPos = CamTransform->GetWorldPosition();
	StartRot = CamTransform->GetWorldRotation();
	Timer = 0;
},
.Update = [=](float _DeltaTime)
{
	Timer += _DeltaTime * 2.5f;
	float Ratio = std::sinf(std::min<float>(1.0f, Timer) * GameEngineMath::PIE * 0.5f);
	
	CamTransform->SetWorldPosition(float4::Lerp(StartPos, EndPos, Ratio));
	CamTransform->SetWorldRotation(float4::Lerp(StartRot, { 0, 90, 0 }, Ratio));
	GlassParts_1->GetTransform()->AddLocalPosition({ -50  * Ratio, 0, 0 });

	RankGrade->GetTransform()->SetLocalPosition(float4::LerpClamp(float4::ZERO, { 1480, 0, -1372 }, Ratio));
},
.End = [=]
{

}
	});

	FSM.ChangeState(ResultState_Wait);

}

void ResultActor::Update(float _DeltaTime)
{
	static bool IsStop = false;
	if (true == IsStop)
	{
		if (GameEngineInput::IsDown("Player_Exceed"))
		{
			IsStop = false;
		}
		return; 
	}
	FSM.Update(_DeltaTime);

	if (GameEngineInput::IsDown("Player_GT_Bomb"))
	{
		FSM.ChangeState(ResultState_Wait);
	}
	if (GameEngineInput::IsDown("Player_Exceed"))
	{
		IsStop = true;
	}
}
