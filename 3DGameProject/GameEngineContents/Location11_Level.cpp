#include "PrecompileHeader.h"
#include "Location11_Level.h"

#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineCaptureTexture.h>
#include "SkyBox.h"

#include "NetworkManager.h"
#include "MapCollisionMesh.h"
#include "FreeCameraActor.h"
#include "PlayerActor_Nero.h"
#include "ColorEffect.h"
#include "JudgementCut.h"
#include "FXAA_Effect.h"

#include "CavaliereAngelo.h"
Location11_Level::Location11_Level()
{

}

Location11_Level::~Location11_Level()
{

}

void Location11_Level::Start()
{
	CreateActor<FreeCameraActor>();
	StageBaseLevel::Start();

	GetCamera(0)->GetCamTarget()->CreateEffect<ColorEffect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();
	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();

	GetCamera(0)->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ -30000, 3200, -200 });
	GetCamera(100)->GetCamTarget()->DepthSettingOff();
}

void Location11_Level::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);
}

void Location11_Level::LevelChangeStart()
{
	StageBaseLevel::LevelChangeStart();
	SetCamera({ 0, 0, 0});
	CreateStage(Location11_StageDatas[0]);
	//AcWallCol.lock()->RenderOn();
	//AcGroundCol.lock()->RenderOn();

	std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
	Nero->GetPhysXComponent()->SetWorldPosition({ -31000, 1950, -360 });
	Nero->GetPhysXComponent()->SetWorldRotation({ 0.0f, -90.0f, 0.0f });
	Nero->SetUserControllType();
	NetworkManager::LinkNetwork(Nero.get(), this);

	if (nullptr == BossMonster)
	{
		BossMonster = CreateActor<CavaliereAngelo>();
		BossMonster->GetPhysXComponent()->SetWorldPosition({ -35500, 1950, -365 });
		BossMonster->GetPhysXComponent()->SetWorldRotation({ 0.0f, 90.0f, 0.0f });
		BossMonster->PushDeathCallback(std::bind(&Location11_Level::LevelChangeToResultLevel, this));
		Nero->SetBossCam(BossMonster->GetTransform());
	}

	AcSkyBox.lock()->SetSkyBloom(1);
	GetDirectionalLight()->GetTransform()->SetWorldPosition({0.f, 10000.f, 0.f});
	GetDirectionalLight()->GetTransform()->SetWorldRotation({45.f, 45.f, 45.f});
}

void Location11_Level::LevelChangeToResultLevel()
{
	TimeEvent.AddEvent(5.0f, [this](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* TimeEvent)
		{
			GameEngineCaptureTexture::CaptureTexture("Capture_Result", GameEngineWindow::GetScreenSize(), GetLevel()->GetMainCamera()->GetCamTarget());
			GameEngineCore::ChangeLevel("ResultLevel");
		});
}
