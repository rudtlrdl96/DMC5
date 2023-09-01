#include "PrecompileHeader.h"
#include "StageBaseLevel.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

#include "NetworkManager.h"
#include "SkyBox.h"
#include "NavMesh.h"
#include "ContentGrid.h"

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
	std::filesystem::path temp = Dir.GetPlusFileName("Test0").GetFullPath();

	GameEngineFile File(temp);
	File.LoadBin(LoadSerializer);

	StageData::ReadAllStageData(LoadSerializer, AllStageDatas);
}

void StageBaseLevel::Start()
{
	BaseLevel::Start();
	CreateScene(GetName());
	LoadAllStageData();
}

void StageBaseLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);
}

void StageBaseLevel::LevelChangeStart()
{
	//DebugGrid = CreateActor<ContentGrid>();
}

void StageBaseLevel::CreateStage(const StageData& _Data)
{
	StageName = _Data.StageName;
	CreateStageFieldMap(_Data.MapDatas);
	CreateSkyBox(_Data.SkyboxFileName);
	CreateNavMesh(_Data.NavMeshFileName);
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

void StageBaseLevel::ClearStage()
{
	StageName = "";
	EraseStageFieldMap();
	EraseSkyBox();
	EraseNavMesh();
}

void StageBaseLevel::CreateNavMesh(const std::string_view& _MeshFileName)
{
	if (_MeshFileName == "\0")
	{
		return;
	}

	AcNavMesh = NavMesh::CreateNavMesh(this, _MeshFileName);
}

void StageBaseLevel::EraseNavMesh()
{
	if (AcNavMesh == nullptr)
	{
		return;
	}
	AcNavMesh->Death();
	AcNavMesh = nullptr;
}


