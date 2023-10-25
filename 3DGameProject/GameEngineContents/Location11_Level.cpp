#include "PrecompileHeader.h"
#include "Location11_Level.h"

#include <GameEngineCore/GameEngineFBXMesh.h>

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
	{
		GameEngineDirectory Dir = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Map", "Location2"
			}
		);
		std::vector<GameEngineFile> FBXFiles = Dir.GetAllFile({ ".fbx" });
		for (GameEngineFile& File : FBXFiles)
		{
			GameEngineFBXMesh::Load(File.GetFullPath());
		}
	}
	SetCamera({ 0, 0, 0});
	CreateStage(AllStageDatas[1]);
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
		Nero->SetBossCam(BossMonster->GetTransform());
	}
}
