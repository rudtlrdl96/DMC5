#include "PrecompileHeader.h"
#include "StartStageLevel.h"
#include "NetworkManager.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include "FreeCameraActor.h"

#include "JudgementCut.h"
#include "ColorEffect.h"
#include "FXAA_Effect.h"
#include "ZoomEffect.h"

#include "StageBaseLevel.h"

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

	TempData.MapDatas.resize(1);
	TempData.SkyboxFileName = "SkyBox.fbx";
	TempData.MapDatas[0].MeshFileName = "Location2.fbx";
	TempData.MapDatas[0].FieldMapPosition = float4::ZERO;

	//for (size_t i = 0; i < TempData.MapDatas.size(); i++)
	//{
	//	TempData.MapDatas[i].ColDatas.resize(3);
	//}

	//for (size_t i = 0; i < TempData.MapDatas.size(); i++)
	//{
	//	for (size_t j = 0; j < TempData.MapDatas[i].ColDatas.size(); j++)
	//	{
	//		TempData.MapDatas[i].ColDatas[j].Pos = { 100 * static_cast<float>(j), 0, 0, 1 };
	//		TempData.MapDatas[i].ColDatas[j].Scale = { 100, 100, 100, 1 };
	//		TempData.MapDatas[i].ColDatas[j].Rot = { 20, 20, 20, 1 };
	//		TempData.MapDatas[i].ColDatas[j].Type = ColType::OBBBOX2D;
	//		TempData.MapDatas[i].ColDatas[j].ColOrder = CollisionOrder::Wall;
	//	}
	//}
}

void StartStageLevel::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);

	/*if (GameEngineInput::IsDown("StageTestKey"))
	{
		TestLoad();
	}*/
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






