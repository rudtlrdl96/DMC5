#include "PrecompileHeader.h"
#include "StageEditLevel.h"

#include <GameEngineCore/GameEngineGUI.h>
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/EngineGrid.h>
#include <GameEngineCore/BloomEffect.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

#include "NetworkManager.h"
#include "StageEditGUI.h"
#include "FreeCameraActor.h"

#include "DistortionEffect.h"


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

	GetCamera(0)->GetDeferredLightTarget()->CreateEffect<BloomEffect>();

	{
		std::shared_ptr<DistortionEffect> Distortion = GetCamera(0)->GetCamTarget()->CreateEffect<DistortionEffect>();
		Distortion->SetMaskTexture(GetCamera(0)->GetCamAlphaTarget(), 1);
		Distortion->SetDistortionValue(10, 10);
		Distortion->SetMaxPixelValue(100, 100);
	}

	GetDirectionalLight()->GetTransform()->SetWorldPosition(float4(0, 5000, 0));
	GetDirectionalLight()->GetTransform()->SetWorldRotation({ 90.f,0.f,0.f });
	GetDirectionalLight()->SetLightPower(0.25f);
	GetDirectionalLight()->SetLightColor({ 0.85f,0.85f,1.f });
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

		//{
		//	IsResourceLoaded = true;
		//	GameEngineDirectory Dir = GameEnginePath::GetFileFullPath
		//	(
		//		"ContentResources",
		//		{
		//			"Map", "TestObj"
		//		}
		//	);
		//	std::vector<GameEngineFile> FBXFiles = Dir.GetAllFile({ ".fbx" });
		//	for (GameEngineFile& File : FBXFiles)
		//	{
		//		GameEngineFBXMesh::Load(File.GetFullPath());
		//	}
		//}

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

	GameEngineCoreWindow::Clear();
	GameEngineCoreWindow::AddDebugRenderTarget(0, "Forward Target", GetMainCamera()->GetCamForwardTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(1, "Deferred Target", GetMainCamera()->GetCamDeferrdTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(2, "Light Target", GetMainCamera()->GetDeferredLightTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(3, "Alpha Target", GetMainCamera()->GetCamAlphaTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(4, "Last Target", GetMainCamera()->GetCamTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(5, "Bake Shadow", GetDirectionalLight()->GetBakeTarget(0));
	GameEngineCoreWindow::AddDebugRenderTarget(6, "Last Shadow", GetDirectionalLight()->GetShadowTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(7, "Distortion Target", GetCamera(100)->GetCamAlphaTarget());
}

void StageEditLevel::LevelChangeEnd()
{
	Editor->Off();
}

