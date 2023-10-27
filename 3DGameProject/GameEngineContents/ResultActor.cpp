#include "PrecompileHeader.h"
#include "ResultActor.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCaptureTexture.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
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

	{
		ResultText_0 = CreateComponent<GameEngineFontRenderer>();
		ResultText_0->SetName("Result");
		ResultText_0->SetFont("DMC5Font");
		ResultText_0->SetFontFlag(FW1_LEFT);
		ResultText_0->SetScale(128.f * GameEngineActor::ScreenRatio.y);
		ResultText_0->SetColor(float4(0.407f, 0.682f, 0.698f, 0.0f));
		ResultText_0->GetTransform()->SetLocalPosition({-700, 400, 0});
		ResultText_0->SetText("RESULTS");
		ResultText_0->Off();
	}

	{
		TextBackground_0 = CreateComponent<GameEngineUIRenderer>();
		TextBackground_0->SetTexture("Result_TextBack.png");
		TextBackground_0->GetTransform()->SetLocalPosition({ -140, 42, 0 });
		TextBackground_0->GetTransform()->SetLocalScale({ 770, 64, 1 });
		TextBackground_0->Off();
	}
	{
		TextBackground_1 = CreateComponent<GameEngineUIRenderer>();
		TextBackground_1->SetTexture("Result_TextBack.png");
		TextBackground_1->GetTransform()->SetLocalPosition({ -140, -58, 0 });
		TextBackground_1->GetTransform()->SetLocalScale({ 770, 64, 1 });
		TextBackground_1->Off();
	}
	{
		TextBackground_2 = CreateComponent<GameEngineUIRenderer>();
		TextBackground_2->SetTexture("Result_TextBack.png");
		TextBackground_2->GetTransform()->SetLocalPosition({ -140, -158, 0 });
		TextBackground_2->GetTransform()->SetLocalScale({ 770, 64, 1 });
		TextBackground_2->Off();
	}

	{
		TextBackground_3 = CreateComponent<GameEngineUIRenderer>();
		TextBackground_3->SetTexture("Result_TextBack.png");
		TextBackground_3->GetTransform()->SetLocalPosition({ -223, -270, 0 });
		TextBackground_3->GetTransform()->SetLocalScale({ 900, 88, 1 });
		TextBackground_3->Off();
	}

	{
		TextBackgroundBar_0 = CreateComponent<GameEngineUIRenderer>();
		TextBackgroundBar_0->SetTexture("Result_Bar.png");
		TextBackgroundBar_0->GetTransform()->SetLocalPosition({ -495, 250, 0 });
		TextBackgroundBar_0->GetTransform()->SetLocalScale({ -428, 5, 1 });
		TextBackgroundBar_0->Off();
	}

	{
		TextBackgroundBar_1 = CreateComponent<GameEngineUIRenderer>();
		TextBackgroundBar_1->SetTexture("Result_Bar.png");
		TextBackgroundBar_1->GetTransform()->SetLocalPosition({ -150, 8, 0 });
		TextBackgroundBar_1->GetTransform()->SetLocalScale({ 736, 5, 1 });
		TextBackgroundBar_1->Off();
	}

	{
		TextBackgroundBar_2 = CreateComponent<GameEngineUIRenderer>();
		TextBackgroundBar_2->SetTexture("Result_Bar.png");
		TextBackgroundBar_2->GetTransform()->SetLocalPosition({ -150, -92, 0 });
		TextBackgroundBar_2->GetTransform()->SetLocalScale({ 736, 5, 1 });
		TextBackgroundBar_2->Off();
	}

	{
		TextBackgroundBar_3 = CreateComponent<GameEngineUIRenderer>();
		TextBackgroundBar_3->SetTexture("Result_Bar.png");
		TextBackgroundBar_3->GetTransform()->SetLocalPosition({ -150, -192, 0 });
		TextBackgroundBar_3->GetTransform()->SetLocalScale({ 736, 5, 1 });
		TextBackgroundBar_3->Off();
	}

	{
		TextBackgroundBar_4 = CreateComponent<GameEngineUIRenderer>();
		TextBackgroundBar_4->SetTexture("Result_Bar.png");
		TextBackgroundBar_4->GetTransform()->SetLocalPosition({ -235, -312, 0 });
		TextBackgroundBar_4->GetTransform()->SetLocalScale({ 867, 5, 1 });
		TextBackgroundBar_4->Off();
	}

	{
		ResultText_1 = CreateComponent<GameEngineFontRenderer>();
		ResultText_1->SetName("Mission");
		ResultText_1->SetFont("DMC5Font");
		ResultText_1->SetFontFlag(FW1_LEFT);
		ResultText_1->SetScale(64.f * GameEngineActor::ScreenRatio.y);
		ResultText_1->SetColor(float4(0.407f, 0.682f, 0.698f, 0.0f));
		ResultText_1->GetTransform()->SetLocalPosition({ -695, 250, 0 });
		ResultText_1->SetText("MISSION 02");
		ResultText_1->Off();
	}

	{
		ResultText_2 = CreateComponent<GameEngineFontRenderer>();
		ResultText_2->SetName("QLIPHOTH");
		ResultText_2->SetFont("DMC5Font");
		ResultText_2->SetFontFlag(FW1_LEFT);
		ResultText_2->SetScale(30.0f * GameEngineActor::ScreenRatio.y);
		ResultText_2->SetColor(float4(0.407f, 0.682f, 0.698f, 0.0f));
		ResultText_2->GetTransform()->SetLocalPosition({ -695, 178, 0 });
		ResultText_2->SetText("QLIPHOTH");
		ResultText_2->Off();
	}

	{
		ResultText_3 = CreateComponent<GameEngineFontRenderer>();
		ResultText_3->SetName("Total");
		ResultText_3->SetFont("DMC5Font");
		ResultText_3->SetFontFlag(FW1_LEFT);
		ResultText_3->SetScale(48.0f * GameEngineActor::ScreenRatio.y);
		ResultText_3->SetColor(float4(0.74f, 0.7f, 0.48f, 0.0f));
		ResultText_3->GetTransform()->SetLocalPosition({ -470, 75, 0 });
		ResultText_3->SetText("Total Stylish Pts");
		ResultText_3->Off();
	}

	{
		ResultText_3_Num = CreateComponent<GameEngineFontRenderer>();
		ResultText_3_Num->SetName("Total Num");
		ResultText_3_Num->SetFont("DMC5Font");
		ResultText_3_Num->SetFontFlag(FW1_RIGHT);
		ResultText_3_Num->SetScale(48.0f * GameEngineActor::ScreenRatio.y);
		ResultText_3_Num->SetColor(float4(0.74f, 0.7f, 0.48f, 0.0f));
		ResultText_3_Num->GetTransform()->SetLocalPosition({ 175, 70, 0 });
		ResultText_3_Num->SetText("0000");
		ResultText_3_Num->Off();
	}

	{
		ResultText_4 = CreateComponent<GameEngineFontRenderer>();
		ResultText_4->SetName("Bonus");
		ResultText_4->SetFont("DMC5Font");
		ResultText_4->SetFontFlag(FW1_LEFT);
		ResultText_4->SetScale(48.0f * GameEngineActor::ScreenRatio.y);
		ResultText_4->SetColor(float4(0.407f, 0.682f, 0.698f, 0.0f));
		ResultText_4->GetTransform()->SetLocalPosition({ -470, -25, 0 });
		ResultText_4->SetText("Bonus Red Orbs");
		ResultText_4->Off();
	}

	{
		ResultText_4_Num = CreateComponent<GameEngineFontRenderer>();
		ResultText_4_Num->SetName("BonusNum");
		ResultText_4_Num->SetFont("DMC5Font");
		ResultText_4_Num->SetFontFlag(FW1_RIGHT);
		ResultText_4_Num->SetScale(48.0f * GameEngineActor::ScreenRatio.y);
		ResultText_4_Num->SetColor(float4(0.407f, 0.682f, 0.698f, 0.0f));
		ResultText_4_Num->GetTransform()->SetLocalPosition({ 175, -30, 0 });
		ResultText_4_Num->SetText("0000");
		ResultText_4_Num->Off();
	}

	{
		ResultText_5 = CreateComponent<GameEngineFontRenderer>();
		ResultText_5->SetName("Extra");
		ResultText_5->SetFont("DMC5Font");
		ResultText_5->SetFontFlag(FW1_LEFT);
		ResultText_5->SetScale(48.0f * GameEngineActor::ScreenRatio.y);
		ResultText_5->SetColor(float4(0.407f, 0.682f, 0.698f, 0.0f));
		ResultText_5->GetTransform()->SetLocalPosition({ -470, -125, 0 });
		ResultText_5->SetText("Extra Bonus");
		ResultText_5->Off();
	}

	{
		ResultText_5_Num = CreateComponent<GameEngineFontRenderer>();
		ResultText_5_Num->SetName("ExtraNum");
		ResultText_5_Num->SetFont("DMC5Font");
		ResultText_5_Num->SetFontFlag(FW1_RIGHT);
		ResultText_5_Num->SetScale(48.0f * GameEngineActor::ScreenRatio.y);
		ResultText_5_Num->SetColor(float4(0.407f, 0.682f, 0.698f, 0.0f));
		ResultText_5_Num->GetTransform()->SetLocalPosition({ 175, -130, 0 });
		ResultText_5_Num->SetText("0000");
		ResultText_5_Num->Off();
	}

	{
		ResultText_6 = CreateComponent<GameEngineFontRenderer>();
		ResultText_6->SetName("Total RedOrbs");
		ResultText_6->SetFont("DMC5Font");
		ResultText_6->SetFontFlag(FW1_LEFT);
		ResultText_6->SetScale(64.0f * GameEngineActor::ScreenRatio.y);
		ResultText_6->SetColor(float4(0.407f, 0.682f, 0.698f, 0.0f));
		ResultText_6->GetTransform()->SetLocalPosition({ -625, -230, 0 });
		ResultText_6->SetText("Total Red Orbs");
		ResultText_6->Off();
	}

	{
		ResultText_6_Num = CreateComponent<GameEngineFontRenderer>();
		ResultText_6_Num->SetName("Total RedOrbsNum");
		ResultText_6_Num->SetFont("DMC5Font");
		ResultText_6_Num->SetFontFlag(FW1_RIGHT);
		ResultText_6_Num->SetScale(64.0f * GameEngineActor::ScreenRatio.y);
		ResultText_6_Num->SetColor(float4(0.407f, 0.682f, 0.698f, 0.0f));
		ResultText_6_Num->GetTransform()->SetLocalPosition({ 170, -225, 0 });
		ResultText_6_Num->SetText("0000");
		ResultText_6_Num->Off();
	}

	//////////////// FSM //////////////////
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
	ResultText_0->Off();
	ResultText_1->Off();
	ResultText_2->Off();
	ResultText_3->Off();
	ResultText_3_Num->Off();
	ResultText_4->Off();
	ResultText_4_Num->Off();
	ResultText_5->Off();
	ResultText_5_Num->Off();
	ResultText_6->Off();
	ResultText_6_Num->Off();
	TextBackgroundBar_0->Off();
	TextBackground_0->Off();
	TextBackground_1->Off();
	TextBackground_2->Off();
	TextBackground_3->Off();
	TextBackgroundBar_1->Off();
	TextBackgroundBar_2->Off();
	TextBackgroundBar_3->Off();
	TextBackgroundBar_4->Off();

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
			TimeEvent->AddEvent(3.5f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _TimeEvent)
				{
					FSM.ChangeState(ResultState_Text1);
				});
			TimeEvent->AddEvent(3.9f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _TimeEvent)
				{
					FSM.ChangeState(ResultState_Text2);
				});
			TimeEvent->AddEvent(4.3f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _TimeEvent)
				{
					FSM.ChangeState(ResultState_Text3);
				});
			TimeEvent->AddEvent(4.7f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _TimeEvent)
				{
					FSM.ChangeState(ResultState_Text4);
				});
			TimeEvent->AddEvent(5.6f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _TimeEvent)
				{
					FSM.ChangeState(ResultState_Text5);
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

FSM.CreateState({ .StateValue = ResultState_Text1,
.Start = [this]
{
	GameEngineSound::Play("Result_SFX_8.wav");
	GameEngineSound::Play("Result_SFX_10.wav");
	ResultText_0->On();
	ResultText_0->SetAlpha(0);
	ResultText_1->On();
	ResultText_1->SetAlpha(0);
	ResultText_2->On();
	ResultText_2->SetAlpha(0);
	TextBackgroundBar_0->On();
	TextBackgroundBar_0->ColorOptionValue.MulColor.a = 0;
	Timer = 0;
},
.Update = [=](float _DeltaTime)
{
	Timer = std::min<float>(1.0f, Timer + _DeltaTime * 4);
	ResultText_0->SetAlpha(Timer);
	ResultText_1->SetAlpha(Timer);
	ResultText_2->SetAlpha(Timer);
	TextBackgroundBar_0->ColorOptionValue.MulColor.a = Timer;
},
.End = [=]
{

}
});

FSM.CreateState({ .StateValue = ResultState_Text2,
.Start = [this]
{
	GameEngineSound::Play("Result_SFX_8.wav");
	GameEngineSound::Play("Result_SFX_10.wav");

	ResultText_0->SetAlpha(1.0f);
	ResultText_1->SetAlpha(1.0f);
	ResultText_2->SetAlpha(1.0f);
	TextBackgroundBar_0->ColorOptionValue.MulColor.a = 1.0f;

	ResultText_3->On();
	ResultText_3->SetAlpha(0);
	ResultText_3_Num->On();
	ResultText_3_Num->SetAlpha(0);
	TextBackground_0->On();
	TextBackground_0->ColorOptionValue.MulColor.a = 0;
	TextBackgroundBar_1->On();
	TextBackgroundBar_1->ColorOptionValue.MulColor.a = 0;
	Timer = 0;
},
.Update = [=](float _DeltaTime)
{
	Timer = std::min<float>(1.0f, Timer + _DeltaTime * 4);
	ResultText_3->SetAlpha(Timer);
	ResultText_3_Num->SetAlpha(Timer);
	TextBackgroundBar_1->ColorOptionValue.MulColor.a = Timer;
	TextBackground_0->ColorOptionValue.MulColor.a = Timer * 0.5f;
	},
.End = [=]
{

}
});

FSM.CreateState({ .StateValue = ResultState_Text3,
.Start = [this]
{
	GameEngineSound::Play("Result_SFX_8.wav");
	GameEngineSound::Play("Result_SFX_10.wav");

	ResultText_3->SetAlpha(1.0f);
	ResultText_3_Num->SetAlpha(1.0f);
	TextBackgroundBar_1->ColorOptionValue.MulColor.a = 1.0f;
	TextBackground_0->ColorOptionValue.MulColor.a = 0.5f;

	ResultText_4->On();
	ResultText_4->SetAlpha(0);
	ResultText_4_Num->On();
	ResultText_4_Num->SetAlpha(0);

	TextBackground_1->On();
	TextBackground_1->ColorOptionValue.MulColor.a = 0;
	TextBackgroundBar_2->On();
	TextBackgroundBar_2->ColorOptionValue.MulColor.a = 0;
	Timer = 0;
},
.Update = [=](float _DeltaTime)
{
	Timer = std::min<float>(1.0f, Timer + _DeltaTime * 4);
	ResultText_4->SetAlpha(Timer);
	ResultText_4_Num->SetAlpha(Timer);
	TextBackgroundBar_2->ColorOptionValue.MulColor.a = Timer;
	TextBackground_1->ColorOptionValue.MulColor.a = Timer * 0.5f;
	},
.End = [=]
{

}
});

FSM.CreateState({ .StateValue = ResultState_Text4,
.Start = [this]
{
	GameEngineSound::Play("Result_SFX_8.wav");
	GameEngineSound::Play("Result_SFX_10.wav");

	ResultText_4->SetAlpha(1.0f);
	ResultText_4_Num->SetAlpha(1.0f);
	TextBackgroundBar_2->ColorOptionValue.MulColor.a = 1.0f;
	TextBackground_1->ColorOptionValue.MulColor.a = 0.5f;

	ResultText_5->On();
	ResultText_5->SetAlpha(0);
	ResultText_5_Num->On();
	ResultText_5_Num->SetAlpha(0);

	TextBackground_2->On();
	TextBackground_2->ColorOptionValue.MulColor.a = 0;
	TextBackgroundBar_3->On();
	TextBackgroundBar_3->ColorOptionValue.MulColor.a = 0;
	Timer = 0;
},
.Update = [=](float _DeltaTime)
{
	Timer = std::min<float>(1.0f, Timer + _DeltaTime * 4);
	ResultText_5->SetAlpha(Timer);
	ResultText_5_Num->SetAlpha(Timer);
	TextBackgroundBar_3->ColorOptionValue.MulColor.a = Timer;
	TextBackground_2->ColorOptionValue.MulColor.a = Timer * 0.5f;
	},
.End = [=]
{

}
});

FSM.CreateState({ .StateValue = ResultState_Text5,
.Start = [this]
{
	GameEngineSound::Play("Result_SFX_8.wav");
	GameEngineSound::Play("Result_SFX_10.wav");

	ResultText_5->SetAlpha(1.0f);
	ResultText_5_Num->SetAlpha(1.0f);
	TextBackgroundBar_3->ColorOptionValue.MulColor.a = 1.0f;
	TextBackground_2->ColorOptionValue.MulColor.a = 0.5f;

	ResultText_6->On();
	ResultText_6->SetAlpha(0);
	ResultText_6_Num->On();
	ResultText_6_Num->SetAlpha(0);

	TextBackground_3->On();
	TextBackground_3->ColorOptionValue.MulColor.a = 0;
	TextBackgroundBar_4->On();
	TextBackgroundBar_4->ColorOptionValue.MulColor.a = 0;
	Timer = 0;
},
.Update = [=](float _DeltaTime)
{
	Timer = std::min<float>(1.0f, Timer + _DeltaTime * 4);
	ResultText_6->SetAlpha(Timer);
	ResultText_6_Num->SetAlpha(Timer);
	TextBackgroundBar_4->ColorOptionValue.MulColor.a = Timer;
	TextBackground_3->ColorOptionValue.MulColor.a = Timer * 0.5f;
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
