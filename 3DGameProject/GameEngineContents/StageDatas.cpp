#include "PrecompileHeader.h"
#include "StageDatas.h"

void ObjTransformData::WriteObjTransformData(GameEngineSerializer& _Serializer)
{
	_Serializer << Pos;
	_Serializer << Scale;
	_Serializer << Rot;
}

void ObjTransformData::ReadObjTransformData(GameEngineSerializer& _Serializer)
{
	_Serializer >> Pos;
	_Serializer >> Scale;
	_Serializer >> Rot;
}

void FieldMapData::WriteFieldMapData(GameEngineSerializer& _Serializer)
{
	_Serializer << static_cast<int>(PartsMeshFileNames.size());
	for (size_t i = 0; i < PartsMeshFileNames.size(); i++)
	{
		_Serializer << PartsMeshFileNames[i];
	}
	_Serializer << static_cast<int>(CullingColTransform.size());
	for (size_t i = 0; i < CullingColTransform.size(); i++)
	{
		CullingColTransform[i].WriteObjTransformData(_Serializer);
	}
	_Serializer << static_cast<int>(NodeIndex.size());
	for (size_t i = 0; i < NodeIndex.size(); i++)
	{
		_Serializer << NodeIndex[i];
	}
}

void FieldMapData::ReadFieldMapData(GameEngineSerializer& _Serializer)
{
	int tempsize = 0;
	_Serializer >> tempsize;
	PartsMeshFileNames.resize(tempsize);
	for (size_t i = 0; i < PartsMeshFileNames.size(); i++)
	{
		_Serializer >> PartsMeshFileNames[i];
	}
	_Serializer >> tempsize;
	CullingColTransform.resize(tempsize);
	for (size_t i = 0; i < CullingColTransform.size(); i++)
	{
		CullingColTransform[i].ReadObjTransformData(_Serializer);
	}
	_Serializer >> tempsize;
	NodeIndex.resize(tempsize);
	for (size_t i = 0; i < NodeIndex.size(); i++)
	{
		_Serializer >> NodeIndex[i];
	}
}

void StageData::WriteStageData(GameEngineSerializer& _Serializer)
{
	_Serializer << StageName;
	_Serializer << static_cast<int>(MapDatas.size());
	for (size_t i = 0; i < MapDatas.size(); i++)
	{
		MapDatas[i].WriteFieldMapData(_Serializer);
	}
	_Serializer << SkyboxFileName;
	_Serializer << GroundMeshFileName;
	_Serializer << WallMeshFileName;
}

void StageData::ReadStageData(GameEngineSerializer& _Serializer)
{
	_Serializer >> StageName;
	int tempsize = 0;
	_Serializer >> tempsize;
	MapDatas.resize(tempsize);
	for (size_t i = 0; i < MapDatas.size(); i++)
	{
		MapDatas[i].ReadFieldMapData(_Serializer);
	}
	_Serializer >> SkyboxFileName;
	_Serializer >> GroundMeshFileName;
	_Serializer >> WallMeshFileName;
}

void StageData::WriteAllStageData(GameEngineSerializer& _Serializer, std::vector<StageData>& _AllData)
{
	_Serializer << static_cast<int>(_AllData.size());
	for (size_t i = 0; i < _AllData.size(); i++)
	{
		_AllData[i].WriteStageData(_Serializer);
	}
}

void StageData::ReadAllStageData(GameEngineSerializer& _Serializer, std::vector<StageData>& _AllData)
{
	int tempsize = 0;
	_Serializer >> tempsize;
	_AllData.resize(tempsize);
	for (size_t i = 0; i < _AllData.size(); i++)
	{
		_AllData[i].ReadStageData(_Serializer);
	}
}


