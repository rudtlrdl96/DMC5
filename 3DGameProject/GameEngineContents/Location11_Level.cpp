#include "PrecompileHeader.h"
#include "Location11_Level.h"
#include "FreeCameraActor.h"
#include <GameEngineCore/GameEngineFBXMesh.h>

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
				"Map", "Stage11"
			}
		);
		std::vector<GameEngineFile> FBXFiles = Dir.GetAllFile({ ".fbx" });
		for (GameEngineFile& File : FBXFiles)
		{
			GameEngineFBXMesh::Load(File.GetFullPath());
		}
	}
}
