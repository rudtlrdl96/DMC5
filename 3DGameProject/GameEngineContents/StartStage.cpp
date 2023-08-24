#include "PrecompileHeader.h"
#include "StartStage.h"
#include "NetworkManager.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include "FreeCameraActor.h"

#include "StageBase.h"

StartStage::StartStage()
{
}

StartStage::~StartStage()
{
}

void StartStage::Start()
{
	StageBase::Start();
}

void StartStage::Update(float _DeltaTime)
{
	StageBase::Update(_DeltaTime);
	StartProcess(this);
}

void StartStage::LevelChangeStart()
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

