#include "PrecompileHeader.h"
#include "Location19_Level.h"

#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/BloomEffect.h>

#include "NetworkManager.h"
#include "MapCollisionMesh.h"
#include "FreeCameraActor.h"
#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"
#include "ColorEffect.h"
#include "JudgementCut.h"
#include "FXAA_Effect.h"
#include "SkyBox.h"
#include "BWColorEffect.h"
#include "DistortionEffect.h"
#include "ZoomEffect.h"
#include "MotionBlurEffect.h"
#include "FadeEffect.h"
#include "Location19_EnemySpawner.h"
#include "Enemy_HellAntenora.h"
#include "Enemy_ScudoAngelo.h"
#include "Enemy_ProtoAngelo.h"
#include "Char_ChoiceUI.h"
#include "l19_Portal.h"

Location19_Level::Location19_Level()
{

}

Location19_Level::~Location19_Level()
{

}

void Location19_Level::Start()
{
	SetNetLevelType(Net_LevelType::Location19);
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

	GetCamera(0)->GetCamTarget()->CreateEffect<ZoomEffect>();
	std::shared_ptr<FadeEffect> Fade = GetLastTarget()->CreateEffect<FadeEffect>();

	GetCamera(0)->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, 0 });
	GetCamera(100)->GetCamTarget()->DepthSettingOff();

}

void Location19_Level::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);
}

void Location19_Level::LevelChangeStart()
{
	StageBaseLevel::LevelChangeStart();
	
	SetCamera({ 0, 0, 0 });
	CreateStage(Location19_StageDatas[0]);
	//AcWallCol.lock()->RenderOn();
	//AcGroundCol.lock()->RenderOn();

	// 플레이어 생성전 플레이어 벡터 초기화
	EventZone::ServerPlayerCount = static_cast<int>(BasePlayerActor::GetPlayers().size());
	BasePlayerActor::LevelChangeClear(this);

	switch (Char_ChoiceUI::GetPlayerType())
	{
	default:
	case ChoicePlayerType::NONE:
	case ChoicePlayerType::NERO:
	{
		MyPlayer = CreateActor<PlayerActor_Nero>();
		MyPlayer->SetUserControllType();
		MyPlayer->SetWorldPosition({ 0, 15.0f, 0 });
		MyPlayer->SetWorldRotation({ 0.0f, -90.0f, 0.0f });
		NetworkManager::LinkNetwork(MyPlayer.get(), this);
		break;
	}
	case ChoicePlayerType::VERGIL:
	{
		MyPlayer = CreateActor<PlayerActor_Vergil>();
		MyPlayer->SetUserControllType();
		MyPlayer->SetWorldPosition({ 0, 15.0f, 0 });
		MyPlayer->SetWorldRotation({ 0.0f, -90.0f, 0.0f });
		NetworkManager::LinkNetwork(MyPlayer.get(), this);
		break;
	}
	}

	if (true == NetworkManager::IsClient())
	{
		MyPlayer->SetWorldPosition({ 50, 15.0f, 50 });
	}
	else
	{
		MyPlayer->SetWorldPosition({ -50, 15.0f, -50 });
	}

	AcSkyBox.lock()->SetSkyBloom(3);
	GetDirectionalLight()->GetTransform()->SetWorldPosition({ 0.f,1000000.f,0.f });
	GetDirectionalLight()->GetTransform()->SetWorldRotation({ 45.f,45.f,45.f });
	GetDirectionalLight()->SetLightColor({1.f,1.f,1.f});
	
	InitPool();
	CreateEventZone();
	Portal = CreateActor<l19_Portal>();
	Portal->GetTransform()->SetWorldPosition({1000.f, 0.f, 1000.f});
	Portal->Off();
}

void Location19_Level::InitPool()
{
	//Enemy_Antenora
	Poolable<Enemy_HellAntenora>::CreatePool(this, static_cast<int>(ActorOrder::Enemy), 4,
		[this](std::shared_ptr<Enemy_HellAntenora> _ActorPtr)
		{
			if (true == NetworkManager::IsClient())
			{
				_ActorPtr->SetControll(NetControllType::PassiveControll);
			}
			else
			{
				NetworkManager::LinkNetwork(_ActorPtr.get(), this);
			}
		});

	//Enemy_Scudo
	Poolable<Enemy_ScudoAngelo>::CreatePool(this, static_cast<int>(ActorOrder::Enemy), 4,
		[this](std::shared_ptr<Enemy_ScudoAngelo> _ActorPtr)
		{
			if (true == NetworkManager::IsClient())
			{
				_ActorPtr->SetControll(NetControllType::PassiveControll);
			}
			else
			{
				NetworkManager::LinkNetwork(_ActorPtr.get(), this);
			}
		});

	//Enemy_Proto
	Poolable<Enemy_ProtoAngelo>::CreatePool(this, static_cast<int>(ActorOrder::Enemy), 1,
		[this](std::shared_ptr<Enemy_ProtoAngelo> _ActorPtr)
		{
			if (true == NetworkManager::IsClient())
			{
				_ActorPtr->SetControll(NetControllType::PassiveControll);
			}
			else
			{
				NetworkManager::LinkNetwork(_ActorPtr.get(), this);
			}
		});
}

void Location19_Level::CreateEventZone()
{
	std::shared_ptr<Location19_EnemySpawner> NewEvent = CreateActor<Location19_EnemySpawner>();
	NewEvent->GetTransform()->SetWorldPosition({ 0, 1000, 0});
	NewEvent->GetTransform()->SetWorldScale({ 8000, 3000, 8000 });
}

void Location19_Level::CreatePotal()
{
	// 모든 적 처치시 실행됨
	TimeEvent.AddEvent(3.0f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* TimeEvent)
		{
			BasePlayerActor::GetMainPlayer()->SetCutScene({ 912, 800, 2260 }, { 912, 600, 1966 }, { -15, 178, 0 }, { 10, 178, 0 }, 4.0f);
		});
	TimeEvent.AddEvent(3.5f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* TimeEvent)
		{
			Portal->On();
		});
}
