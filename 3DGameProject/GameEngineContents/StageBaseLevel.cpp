#include "PrecompileHeader.h"
#include "StageBaseLevel.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

#include "NetworkManager.h"
#include "SkyBox.h"
#include "MapCollisionMesh.h"

std::vector<StageData> StageBaseLevel::AllStageDatas;

StageBaseLevel::StageBaseLevel()
{
}

StageBaseLevel::~StageBaseLevel()
{
}

void StageBaseLevel::LoadAllStageData()
{
	GameEngineSerializer LoadSerializer = GameEngineSerializer();

	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("ContentData");
	Dir.Move("ContentData");
	Dir.Move("StageData");
	std::filesystem::path temp = Dir.GetPlusFileName("TestStage").GetFullPath();

	GameEngineFile File(temp);
	File.LoadBin(LoadSerializer);

	StageData::ReadAllStageData(LoadSerializer, AllStageDatas);
}

void StageBaseLevel::Start()
{
	BaseLevel::Start();
	CreateScene(GetName());
	if (AllStageDatas.empty())
	{
		LoadAllStageData();
	}
}

void StageBaseLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);
}

void StageBaseLevel::LevelChangeStart()
{
}

void StageBaseLevel::CreateStage(const StageData& _Data)
{
	StageName = _Data.StageName;
	CreateStageFieldMap(_Data.MapDatas);
	CreateSkyBox(_Data.SkyboxFileName);
	CreateGroundCol(_Data.GroundMeshFileName);
	CreateWallCol(_Data.WallMeshFileName);
}

void StageBaseLevel::SetCamera(float4 _Position)
{
	if (GetMainCamera() != nullptr)
	{
		GetMainCamera()->SetProjectionType(CameraType::Perspective);
	}
	GetMainCamera()->GetTransform()->SetLocalPosition(_Position);
}

//void StageBaseLevel::FieldMapOn(int _Index)
//{
//	if (_Index == -1)
//	{
//		for (auto& i : AcFieldMaps)
//		{
//			for (auto& j : i.second)
//			{
//				j->On();
//			}
//		}
//
//	}
//	else
//	{
//		if (AcFieldMaps.find(_Index) == AcFieldMaps.end())
//		{
//			return;
//		}
//		std::vector<std::shared_ptr<FieldMap>>& tempFieldMap = AcFieldMaps.find(_Index)->second;
//		for (size_t i = 0; i < tempFieldMap.size(); i++)
//		{
//			tempFieldMap[i]->On();
//		}
//	}
//}
//
//void StageBaseLevel::FieldMapOff(int _Index)
//{
//	if (_Index == -1)
//	{
//		for (auto& i : AcFieldMaps)
//		{
//			for (auto& j : i.second)
//			{
//				j->Off();
//			}
//		}
//	}
//	else
//	{
//		if (AcFieldMaps.find(_Index) == AcFieldMaps.end())
//		{
//			return;
//		}
//		std::vector<std::shared_ptr<FieldMap>>& tempFieldMap = AcFieldMaps.find(_Index)->second;
//		for (size_t i = 0; i < tempFieldMap.size(); i++)
//		{
//			tempFieldMap[i]->Off();
//		}
//	}
//}

void StageBaseLevel::EraseStageFieldMap()
{
	if (AcFieldMaps.empty())
	{
		return;
	}

	for (size_t i = 0; i < AcFieldMaps.size(); i++)
	{
		AcFieldMaps[i]->EraseFieldMap();
		AcFieldMaps[i] = nullptr;
	}
	AcFieldMaps.clear();
}

void StageBaseLevel::CreateStageFieldMap(const std::vector<FieldMapData>& _MapDatas)
{
	if (_MapDatas.empty())
	{
		return;
	}

	AcFieldMaps.resize(_MapDatas.size());
	for (size_t i = 0; i < AcFieldMaps.size(); i++)
	{
		AcFieldMaps[i] = FieldMap::CreateFieldMap(this, _MapDatas[i].PartsMeshFileNames, _MapDatas[i].CullingColTransform);
	}
}

void StageBaseLevel::CreateSkyBox(const std::string_view& _MeshFileName)
{
	if (_MeshFileName == "\0")
	{
		return;
	}

	AcSkyBox = SkyBox::CreateSkyBox(this, _MeshFileName);
}

void StageBaseLevel::EraseSkyBox()
{
	if (AcSkyBox == nullptr)
	{
		return;
	}
	AcSkyBox->Death();
	AcSkyBox = nullptr;
}

void StageBaseLevel::CreateGroundCol(const std::string_view& _MeshFileName)
{
	if (_MeshFileName == "\0")
	{
		return;
	}
	AcGroundCol = MapCollisionMesh::CreateGroundCollisionMesh(this, _MeshFileName);
}

void StageBaseLevel::EraseGroundCol()
{
	if (AcGroundCol == nullptr)
	{
		return;
	}
	AcGroundCol->Death();
	AcGroundCol = nullptr;
}

void StageBaseLevel::ClearStage()
{
	if (StageName == "")
	{
		return;
	}
	StageName = "";
	EraseStageFieldMap();
	EraseSkyBox();
	EraseGroundCol();
	EraseWallCol();
}

void StageBaseLevel::CreateWallCol(const std::string_view& _MeshFileName)
{
	if (_MeshFileName == "\0")
	{
		return;
	}
	AcWallCol = MapCollisionMesh::CreateWallCollisionMesh(this, _MeshFileName);
}

void StageBaseLevel::EraseWallCol()
{
	if (AcWallCol == nullptr)
	{
		return;
	}
	AcWallCol->Death();
	AcWallCol = nullptr;
}


