#include "PrecompileHeader.h"
#include "Location11_Level.h"

#include <GameEngineCore/BloomEffect.h>
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineCaptureTexture.h>
#include "SkyBox.h"

#include "NetworkManager.h"
#include "MapCollisionMesh.h"
#include "FreeCameraActor.h"
#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"
#include "ColorEffect.h"
#include "JudgementCut.h"
#include "FXAA_Effect.h"

#include "CavaliereAngelo.h"
#include "FadeEffect.h"
#include "MotionBlurEffect.h"
#include "DistortionEffect.h"
#include "ZoomEffect.h"
#include "BWColorEffect.h"
#include "BGMPlayer.h"
#include "EventZone.h"
Location11_Level::Location11_Level()
{

}

Location11_Level::~Location11_Level()
{

}

void Location11_Level::Start()
{
	SetNetLevelType(Net_LevelType::Location11);
	CreateActor<FreeCameraActor>();
	StageBaseLevel::Start();

	GetCamera(0)->GetCamTarget()->CreateEffect<ColorEffect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();

	GetCamera(0)->GetDeferredLightTarget()->CreateEffect<BloomEffect>();

	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<BWColorEffect>();

	{
		std::shared_ptr<DistortionEffect> Distortion = GetCamera(0)->GetCamTarget()->CreateEffect<DistortionEffect>();
		Distortion->SetMaskTexture(GetCamera(0)->GetCamAlphaTarget(), 1);
		Distortion->SetDistortionValue(10, 10);
		Distortion->SetMaxPixelValue(100, 100);
	}

	{
		std::shared_ptr<DistortionEffect> Distortion = GetCamera(0)->GetCamTarget()->CreateEffect<DistortionEffect>();
		Distortion->SetMaskTexture(GetCamera(100)->GetCamAlphaTarget(), 1);
		Distortion->SetDistortionValue(10, 10);
		Distortion->SetMaxPixelValue(100, 100);
	}
	GetCamera(0)->GetCamTarget()->CreateEffect<ZoomEffect>();

	{
		std::shared_ptr<MotionBlurEffect> Blur = GetCamera(0)->GetCamTarget()->CreateEffect<MotionBlurEffect>();
		Blur->SetCamPosTarget(GetMainCamera()->GetCamPosTarget());
		Blur->SetCamMaskTarget(GetMainCamera()->GetCamMaskTarget());
		Blur->SetCam(GetMainCamera());
	}

	std::shared_ptr<FadeEffect> Fade = GetLastTarget()->CreateEffect<FadeEffect>();

	GetCamera(0)->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ -30000, 3200, -200 });
	GetCamera(100)->GetCamTarget()->DepthSettingOff();

	if (false == GameEngineInput::IsKey("BakeTestKey"))
	{
		GameEngineInput::CreateKey("BakeTestKey", VK_PRIOR);
	}
}

void Location11_Level::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);

	if (true == GameEngineInput::IsDown("BakeTestKey"))
	{
		const std::vector<std::shared_ptr<GameEngineLight>>& AllLightRef = GetAllLightRef();

		for (std::shared_ptr<GameEngineLight> Ref : AllLightRef)
		{
			Ref->BakeShadow(GetMainCamera());
		}
	}
}

void Location11_Level::LevelChangeStart()
{
	GetDirectionalLight()->GetTransform()->SetWorldPosition(float4(-33000, 5000, 0));
	GetDirectionalLight()->GetTransform()->SetWorldRotation({ 90.f, 45.f, 0.f });
	GetDirectionalLight()->SetLightPower(1.0f);
	GetDirectionalLight()->SetLightColor({ 0.75f,0.75,1.f });

	StageBaseLevel::LevelChangeStart();
	SetCamera({ 0, 0, 0 });
	CreateStage(Location11_StageDatas[0]);
	//AcWallCol.lock()->RenderOn();
	//AcGroundCol.lock()->RenderOn();

	EventZone::ServerPlayerCount = BasePlayerActor::GetPlayers().size();
	// 플레이어 생성전 플레이어 벡터 초기화
	BasePlayerActor::LevelChangeClear(this);

	if (true == NetworkManager::IsClient())
	{
		MainPlayer = CreateActor<PlayerActor_Vergil>();
		MainPlayer->SetUserControllType();
		MainPlayer->SetWorldPosition({ -31000, 1950, -360 });
		MainPlayer->SetWorldRotation({ 0.0f, -90.0f, 0.0f });
		NetworkManager::LinkNetwork(MainPlayer.get(), this);
	}
	else
	{
		MainPlayer = CreateActor<PlayerActor_Nero>();
		MainPlayer->SetUserControllType();
		MainPlayer->SetWorldPosition({ -31000, 1950, -360 });
		MainPlayer->SetWorldRotation({ 0.0f, -90.0f, 0.0f });
		NetworkManager::LinkNetwork(MainPlayer.get(), this);
	}

	//Enemy_Qliphoth
	Poolable<CavaliereAngelo>::CreatePool(this, static_cast<int>(ActorOrder::Enemy), 1,
		[this](std::shared_ptr<CavaliereAngelo> _ActorPtr)
		{
			if (true == NetworkManager::IsClient())
			{
				_ActorPtr->SetControll(NetControllType::PassiveControll);
				_ActorPtr->PushDeathCallback(std::bind(&Location11_Level::LevelChangeToResultLevel, this));
			}
			else
			{
				NetworkManager::LinkNetwork(_ActorPtr.get(), this);
				_ActorPtr->PushDeathCallback(std::bind(&Location11_Level::LevelChangeToResultLevel, this));
			}
		});

	AcSkyBox.lock()->SetSkyBloom(1);

	BGMPlayer::SetBossBGM();

	if (false == NetworkManager::IsClient())
	{
		CreateEventZone();
	}
	NetworkManager::PushNetworkEvent(Net_EventType::Location11_Start, [=]
		{
			CutSceneStart();
		});
}

void Location11_Level::LevelChangeToResultLevel()
{
	BGMPlayer::SetBattleEnd();
	TimeEvent.AddEvent(5.0f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* TimeEvent)
		{
			GameEngineCaptureTexture::CaptureTexture("Capture_Result", GameEngineWindow::GetScreenSize(), GetLevel()->GetMainCamera()->GetCamTarget());
			GameEngineCore::ChangeLevel("ResultLevel");
		});
}

void Location11_Level::CreateEventZone()
{
	std::shared_ptr<EventZone> NewEvent = CreateActor<EventZone>();
	NewEvent->GetTransform()->SetWorldPosition({ -32700, 2389, -290 });
	NewEvent->GetTransform()->SetWorldScale({ 5000, 1110, 3000 });
	NewEvent->SetEvent([this]
		{
			std::shared_ptr<CavaliereAngelo> Cavaliere = Poolable<CavaliereAngelo>::PopFromPool(this, static_cast<int>(ActorOrder::Enemy));
			Cavaliere->GetPhysXComponent()->SetWorldPosition({ -35500, 1950, -365 });
			Cavaliere->GetPhysXComponent()->SetWorldRotation({ 0.0f, 90.0f, 0.0f });
			MainPlayer->SetBossCam(Cavaliere->GetTransform());
			NetworkManager::ExcuteNetworkEvent(Net_EventType::Location11_Start);
			CutSceneStart();
		});

}

void Location11_Level::CutSceneStart()
{
	BasePlayerActor::GetMainPlayer()->SetCutScene({ -33314, 3107, -1659 }, { -33767, 3100, -1517 }, { 21, 47, 0 }, { 27, 27, 0 }, 3.0f);
	TimeEvent.AddEvent(3.0f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* TimeEvent)
		{
			BasePlayerActor::GetMainPlayer()->SetCutScene({ -34184, 3671, -414 }, { -34244, 2300, -411 }, { -6, -90, 0 }, { 0, -90, 0 }, 5.0f);
		});
	TimeEvent.AddEvent(8.0f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* TimeEvent)
		{
			BasePlayerActor::GetMainPlayer()->SetCutScene({ -35000, 2000, -365 }, { -35000, 2000, -365 }, { 0, -90, 0 }, { 0, -90, 0 }, 5.0f);
		});
}
