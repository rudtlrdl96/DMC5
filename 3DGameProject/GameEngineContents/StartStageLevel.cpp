#include "PrecompileHeader.h"
#include "StartStageLevel.h"
#include "NetworkManager.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include "FreeCameraActor.h"
#include "JudgementCut.h"

#include "StageBaseLevel.h"

StartStageLevel::StartStageLevel()
{
}

StartStageLevel::~StartStageLevel()
{
}

void StartStageLevel::Start()
{
	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();

	StageBaseLevel::Start();

	TempData.MapDatas.resize(1);
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
			TempData.MapDatas[i].ColDatas[j].ColOrder = CollisionOrder::Wall;
		}
	}
}

void StartStageLevel::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);

	if (GameEngineInput::IsDown("StageTestKey"))
	{
		TestLoad();
	}
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
	//AllStageDatas.push_back(TempData);
}

void StartStageLevel::TestSave()
{
	GameEngineSerializer SaveSerializer = GameEngineSerializer();
	StageData::WriteAllStageData(SaveSerializer, AllStageDatas);

	GameEnginePath filepath;
	filepath.SetPath("..//ContentData//StageData//TestData.txt");

	GameEngineFile file = GameEngineFile(std::string_view(filepath.GetFullPath()));
	file.SaveBin(SaveSerializer);
}

void StartStageLevel::TestLoad()
{
	GameEngineSerializer LoadSerializer = GameEngineSerializer();
	
	std::string_view Temp = "..//ContentData//StageData//TestData.txt";
	GameEngineFile File(Temp);
	File.LoadBin(LoadSerializer);

	StageData::ReadAllStageData(LoadSerializer, AllStageDatas);
}


