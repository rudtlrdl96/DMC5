#include "PrecompileHeader.h"
#include "StageBaseLevel.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

#include "NetworkManager.h"
#include "SkyBox.h"

std::vector<StageData> StageBaseLevel::AllStageDatas;

StageBaseLevel::StageBaseLevel()
{
}

StageBaseLevel::~StageBaseLevel()
{
}

void StageBaseLevel::Start()
{
	if (!GameEngineInput::IsKey("StageTestKey"))
	{
		GameEngineInput::CreateKey("StageTestKey", 'M');
	}
}

void StageBaseLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);
}

void StageBaseLevel::CreateStage(const StageData& _Data)
{
	StageName = _Data.StageName;
	CreateStageFieldMap(_Data.MapDatas);
	CreateSkyBox(_Data.SkyboxFileName);
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
		AcFieldMaps[i] = FieldMap::CreateFieldMap(this, _MapDatas[i].MeshFileName, _MapDatas[i].NavMeshFileName, _MapDatas[i].FieldMapPosition, _MapDatas[i].FieldMapScale, _MapDatas[i].FieldMapRotation);
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
	for (size_t i = 0; i < AcFieldMaps.size(); i++)
	{
		AcFieldMaps[i]->Death();
		AcFieldMaps[i] = nullptr;
	}
	AcFieldMaps.clear();
	
	if (AcSkyBox != nullptr)
	{
		AcSkyBox->Death();
		AcSkyBox = nullptr;
	}
	
}


