#include "PrecompileHeader.h"
#include "StartStageLevel.h"
#include "NetworkManager.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include "FreeCameraActor.h"

#include "JudgementCut.h"
#include "ColorEffect.h"
#include "FXAA_Effect.h"
#include "ZoomEffect.h"

#include "Plane.h"
#include "TestObject.h"

StartStageLevel* StartStageLevel::Inst = nullptr;

StartStageLevel::StartStageLevel()
{
	Inst = this;
}

StartStageLevel::~StartStageLevel()
{
	Inst = nullptr;
}

void StartStageLevel::Start()
{
	GetCamera(0)->ReflectionOn();
	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();
	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<ZoomEffect>();

	StageBaseLevel::Start();
}

void StartStageLevel::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);
}

void StartStageLevel::LevelChangeStart()
{
	StageBaseLevel::LevelChangeStart();
	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Map", "TestMap"
			},
			"Location2.FBX"
		);

		GameEngineFBXMesh::Load(Path);
	}

	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Map", "TestMap"
			},
			"SkyBox.fbx"
		);

		GameEngineFBXMesh::Load(Path);
	}

	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Map", "TestMap"
			},
			"Location2ColGuide.fbx"
		);

		GameEngineFBXMesh::Load(Path);
	}

	SetCamera({ 0,0,-500 });

	CreateStage(AllStageDatas[1]);
	//CreateActor<TestObject>();
}






