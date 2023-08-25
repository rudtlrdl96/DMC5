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

	TempData.MapDatas.resize(1);
	TempData.StageID = 0;
	TempData.SkyboxFileName = "SkyBox.fbx";
	TempData.MapDatas[0].MeshFileName = "Location2.fbx";
	TempData.MapDatas[0].FieldMapPosition = float4::ZERO;

	for (size_t i = 0; i < TempData.MapDatas.size(); i++)
	{
		TempData.MapDatas[i].ColDatas.resize(3);
	}

	for (size_t i = 0; i < TempData.MapDatas.size(); i++)
	{
		for (size_t j = 0; j < TempData.MapDatas[i].ColDatas.size(); j++)
		{
			TempData.MapDatas[i].ColDatas[j].Pos = { 100 * static_cast<float>(j), 0, 0, 1 };
			TempData.MapDatas[i].ColDatas[j].Scale = { 100, 100, 100, 1 };
			TempData.MapDatas[i].ColDatas[j].Rot = { 20, 20, 20, 1 };
			TempData.MapDatas[i].ColDatas[j].Type = ColType::OBBBOX2D;
			TempData.MapDatas[i].ColDatas[j].ColOrder = 0;
		}
	}
}

void StartStageLevel::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);

	
}

void StartStageLevel::LevelChangeStart()
{
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


	SetCamera({ 0,0,-500 });
	CreateActor<FreeCameraActor>();

	CreateStage(TempData);
}


