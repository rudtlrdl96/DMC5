#include "PrecompileHeader.h"
#include "StartStageLevel.h"
#include <GameEngineCore/GameEngineFBXMesh.h>

#include "NetworkManager.h"
#include "FreeCameraActor.h"
#include "Plane.h"
#include "TestObject.h"
#include "PlayerActor_Nero.h"

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
	StageBaseLevel::Start();
}

void StartStageLevel::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);
}

void StartStageLevel::LevelChangeStart()
{
	StageBaseLevel::LevelChangeStart();
	/*{
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
	}*/

	SetCamera({ 0,0,-500 });

	CreateStage(AllStageDatas[0]);
	//std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
	//Nero->SetWorldPosition({ 0, 1000, 0 });
}






