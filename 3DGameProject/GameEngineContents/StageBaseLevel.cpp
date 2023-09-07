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

void StageBaseLevel::EraseStageFieldMap(int _index)
{
	if (_index == -1)
	{
		for (size_t i = 0; i < AcFieldMaps.size(); i++)
		{
			AcFieldMaps[i]->Death();
			AcFieldMaps[i] = nullptr;
		}
		AcFieldMaps.clear();
	}

	if (AcFieldMaps.size() <= _index)
	{
		return;
	}
	AcFieldMaps[_index]->Death();
	AcFieldMaps[_index] = nullptr;
	AcFieldMaps.erase(AcFieldMaps.begin() + _index);
}

void StageBaseLevel::CreateStageFieldMap(const std::vector<FieldMapData>& _MapDatas)
{
	AcFieldMaps.resize(_MapDatas.size());
	for (size_t i = 0; i < AcFieldMaps.size(); i++)
	{
		AcFieldMaps[i] = FieldMap::CreateFieldMap(this, _MapDatas[i].MeshFileName, _MapDatas[i].FieldMapPosition, _MapDatas[i].FieldMapScale, _MapDatas[i].FieldMapRotation);
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


