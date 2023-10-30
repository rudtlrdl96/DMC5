#include "PrecompileHeader.h"
#include "Location19_Level.h"

#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/BloomEffect.h>

#include "NetworkManager.h"
#include "MapCollisionMesh.h"
#include "FreeCameraActor.h"
#include "PlayerActor_Nero.h"
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

	// �÷��̾� ������ �÷��̾� ���� �ʱ�ȭ
	BasePlayerActor::LevelChangeClear(this);

	std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
	Nero->GetPhysXComponent()->SetWorldPosition({ 0, 15.0f, 0 });
	Nero->SetUserControllType();
	NetworkManager::LinkNetwork(Nero.get(), this);

	AcSkyBox.lock()->SetSkyBloom(5);
	GetDirectionalLight()->GetTransform()->SetWorldPosition({ 0.f,10000.f,0.f });
	GetDirectionalLight()->GetTransform()->SetWorldRotation({ 45.f,45.f,45.f });
	GetDirectionalLight()->SetLightColor({1.f,0.85f,0.85f});
	
	InitPool();
	CreateEventZone();
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
	// ��� �� óġ�� �����
	int a = 0;
}
