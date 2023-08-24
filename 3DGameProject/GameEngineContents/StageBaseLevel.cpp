#include "PrecompileHeader.h"
#include "StageBaseLevel.h"
#include "NetworkManager.h"

using namespace std;

StageBaseLevel::StageBaseLevel()
{
}

StageBaseLevel::~StageBaseLevel()
{
}

void StageBaseLevel::Start()
{
}

void StageBaseLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);
}

void StageBaseLevel::CreateStage(const StageData& _Data)
{
	CreateStageFieldMap(_Data.MapDatas);
}

void StageBaseLevel::SetCamera(float4 _Position)
{
	if (GetMainCamera() != nullptr)
	{
		GetMainCamera()->SetProjectionType(CameraType::Perspective);
	}
	GetMainCamera()->GetTransform()->SetLocalPosition(_Position);
}

void StageBaseLevel::CreateStageFieldMap(const std::vector<FieldMapData>& _MapDatas)
{
	AcFieldMaps.resize(_MapDatas.size());
	for (size_t i = 0; i < AcFieldMaps.size(); i++)
	{
		AcFieldMaps[i] = FieldMap::CreateFieldMap(this, _MapDatas[i].MeshFileName, _MapDatas[i].ColDatas, _MapDatas[i].FieldMapPosition);
	}
}
