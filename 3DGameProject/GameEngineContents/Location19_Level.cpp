#include "PrecompileHeader.h"
#include "Location19_Level.h"

#include <GameEngineCore/GameEngineFBXMesh.h>

#include "NetworkManager.h"
#include "MapCollisionMesh.h"
#include "FreeCameraActor.h"
#include "PlayerActor_Nero.h"
#include "ColorEffect.h"
#include "JudgementCut.h"
#include "FXAA_Effect.h"
#include "SkyBox.h"

Location19_Level::Location19_Level()
{

}

Location19_Level::~Location19_Level()
{

}

void Location19_Level::Start()
{
	CreateActor<FreeCameraActor>();
	StageBaseLevel::Start();

	GetCamera(0)->GetCamTarget()->CreateEffect<ColorEffect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();
	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();

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

	std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
	Nero->GetPhysXComponent()->SetWorldPosition({ 0, 500, 0 });
	Nero->SetUserControllType();
	NetworkManager::LinkNetwork(Nero.get(), this);

	AcSkyBox.lock()->SetSkyBloom(5);
	GetDirectionalLight()->GetTransform()->SetWorldPosition({ 0.f,10000.f,0.f });
	GetDirectionalLight()->GetTransform()->SetWorldRotation({ 45.f,45.f,45.f });
	GetDirectionalLight()->SetLightColor({1.f,0.85f,0.85f});
}
