#include "PrecompileHeader.h"
#include "StageDatas.h"

void FieldMapColData::WriteFieldMapColData(GameEngineSerializer& _Serializer)
{
	_Serializer << Pos;
	_Serializer << Scale;
	_Serializer << Rot;
	_Serializer.Write(&Type, sizeof(ColType));
	_Serializer.Write(&ColOrder, sizeof(CollisionOrder));
}

void FieldMapColData::ReadFieldMapColData(GameEngineSerializer& _Serializer)
{
	_Serializer >> Pos;
	_Serializer >> Scale;
	_Serializer >> Rot;
	_Serializer.Read(&Type, sizeof(ColType));
	_Serializer.Read(&ColOrder, sizeof(CollisionOrder));
}

void FieldMapData::WriteFieldMapData(GameEngineSerializer& _Serializer)
{
	_Serializer << MeshFileName;
	_Serializer << static_cast<int>(ColDatas.size());
	for (size_t i = 0; i < ColDatas.size(); i++)
	{
		ColDatas[i].WriteFieldMapColData(_Serializer);
	}
	_Serializer << FieldMapPosition;
}

void FieldMapData::ReadFieldMapData(GameEngineSerializer& _Serializer)
{
	_Serializer >> MeshFileName;
	int tempsize = 0;
	_Serializer >> tempsize;
	ColDatas.resize(tempsize);
	for (size_t i = 0; i < ColDatas.size(); i++)
	{
		ColDatas[i].ReadFieldMapColData(_Serializer);
	}
	_Serializer >> FieldMapPosition;
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
