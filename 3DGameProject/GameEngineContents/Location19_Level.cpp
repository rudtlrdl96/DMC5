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

	// 플레이어 생성전 플레이어 벡터 초기화
	BasePlayerActor::LevelChangeClear(this);

	std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
	Nero->GetPhysXComponent()->SetWorldPosition({ 0, 500, 0 });
	Nero->SetUserControllType();
	NetworkManager::LinkNetwork(Nero.get(), this);

	AcSkyBox.lock()->SetSkyBloom(5);
	GetDirectionalLight()->GetTransform()->SetWorldPosition({ 0.f,10000.f,0.f });
	GetDirectionalLight()->GetTransform()->SetWorldRotation({ 45.f,45.f,45.f });
	GetDirectionalLight()->SetLightColor({1.f,0.85f,0.85f});
}
