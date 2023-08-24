#include "PrecompileHeader.h"
#include "StartStageLevel.h"
#include "NetworkManager.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include "FreeCameraActor.h"

#include "StageBaseLevel.h"

StartStageLevel::StartStageLevel()
{
}

StartStageLevel::~StartStageLevel()
{
}

void StartStageLevel::Start()
{
	StageBaseLevel::Start();
}

void StartStageLevel::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);
	StartProcess(this);
}

void StartStageLevel::LevelChangeStart()
{
	std::string Path = GameEnginePath::GetFileFullPath
	(
		"ContentResources",
		{
			"Map", "TestMap"
		},
		"Location2ColGuide.FBX"
	);

	GameEngineFBXMesh::Load(Path);
	SetCamera({ 0,0,-500 });
	CreateActor<FreeCameraActor>();

	AcFieldMap = FieldMap::CreateFieldMap(this, "Location2ColGuide.fbx");
}

