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

void StageBaseLevel::EraseStageFieldMap(int _mapbundleindex, int _mapindex)
{
	if (AcFieldMaps.size() <= _mapbundleindex)
	{
		return;
	}

	if (AcFieldMaps.size() <= _mapindex)
	{
		return;
	}

	if (_mapbundleindex == -1)
	{
		for (auto& i : AcFieldMaps)
		{
			for (size_t j = 0; j < i.second.size(); j++)
			{
				i.second[j]->Death();
				i.second[j] = nullptr;
			}
			i.second.clear();
		}
		AcFieldMaps.clear();
	}

	if (_mapindex == -1)
	{
		for (size_t j = 0; j < AcFieldMaps.find(_mapbundleindex)->second.size(); j++)
		{
			AcFieldMaps.find(_mapbundleindex)->second[j]->Death();
			AcFieldMaps.find(_mapbundleindex)->second[j] = nullptr;
		}
		AcFieldMaps.find(_mapbundleindex)->second.clear();
	}

	AcFieldMaps.find(_mapbundleindex)->second[_mapindex]->Death();
	AcFieldMaps.find(_mapbundleindex)->second[_mapindex] = nullptr;
	AcFieldMaps.find(_mapbundleindex)->second.erase(AcFieldMaps.find(_mapbundleindex)->second.begin() + _mapindex);
}

void StageBaseLevel::CreateStageFieldMap(const std::map<int, std::vector<FieldMapData>>& _MapDatas)
{
	//AcFieldMaps.resize(_MapDatas.size());
	for (auto& i : _MapDatas)
	{
		int temp_first = i.first;
		std::vector<std::shared_ptr<FieldMap>> temp_second;
		temp_second.resize(i.second.size());
		for (size_t j = 0; j < temp_second.size(); j++)
		{
			temp_second[j] = FieldMap::CreateFieldMap
			(
				this,
				i.second[j].MeshFileName,
				i.second[j].FieldMapPosition,
				i.second[j].FieldMapScale,
				i.second[j].FieldMapRotation
			);
		}
		AcFieldMaps.insert(std::make_pair(temp_first, temp_second));
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


