#include "PrecompileHeader.h"
#include "StageEditLevel.h"

#include <GameEngineCore/GameEngineGUI.h>
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/EngineGrid.h>

#include "NetworkManager.h"
#include "StageEditGUI.h"
#include "FreeCameraActor.h"


StageEditLevel::StageEditLevel()
{
}

StageEditLevel::~StageEditLevel()
{
}

void StageEditLevel::Start()
{
	StageBaseLevel::Start();
	if (GetMainCamera() != nullptr)
	{
		GetMainCamera()->SetProjectionType(CameraType::Perspective);
	}
	GetMainCamera()->GetTransform()->SetLocalPosition({0,0,-500});

	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Texture");
	NewDir.Move("StageEditorUI");

	std::vector<GameEngineFile> File = NewDir.GetAllFile({ ".Png", });

	for (size_t i = 0; i < File.size(); i++)
	{
		GameEngineTexture::Load(File[i].GetFullPath());
	}

	Grid = CreateActor<EngineGrid>();
	FreeCam = CreateActor<FreeCameraActor>();
	IsEditLevel = true;
}

void StageEditLevel::Update(float _DeltaTime)
{
	//StageBaseLevel::Update(_DeltaTime);
	//Test
	if (true == GameEngineInput::IsDown("SelectLevel_01"))
	{
		GameEngineCore::ChangeLevel("MainLevel");
		return;
	}
}

void StageEditLevel::LevelChangeStart()
{
	if (Editor == nullptr)
	{
		std::shared_ptr<GameEngineGUIWindow> EditGUI = GameEngineGUI::GUIWindowCreate<StageEditGUI>("StageEditGUI");
		Editor = EditGUI;
	}

	Editor->On();

	if (!IsResourceLoaded)
	{
		{
			IsResourceLoaded = true;
			GameEngineDirectory Dir = GameEnginePath::GetFileFullPath
			(
				"ContentResources",
				{
					"Map", "TestMap"
				}
			);
			std::vector<GameEngineFile> FBXFiles = Dir.GetAllFile({ ".fbx" });
			for (GameEngineFile& File : FBXFiles)
			{
				GameEngineFBXMesh::Load(File.GetFullPath());
			}
		}

		{
			IsResourceLoaded = true;
			GameEngineDirectory Dir = GameEnginePath::GetFileFullPath
			(
				"ContentResources",
				{
					"Map", "TestObj"
				}
			);
			std::vector<GameEngineFile> FBXFiles = Dir.GetAllFile({ ".fbx" });
			for (GameEngineFile& File : FBXFiles)
			{
				GameEngineFBXMesh::Load(File.GetFullPath());
			}
		}

		{
			IsResourceLoaded = true;
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
	}

	IsDebugSwitch();
}

void StageEditLevel::LevelChangeEnd()
{
	Editor->Off();
}

