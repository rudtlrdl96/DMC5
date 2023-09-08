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
	_Serializer << FieldMapPosition;
	_Serializer << FieldMapScale;
	_Serializer << FieldMapRotation;
}

void FieldMapData::ReadFieldMapData(GameEngineSerializer& _Serializer)
{
	_Serializer >> MeshFileName;
	_Serializer >> FieldMapPosition;
	_Serializer >> FieldMapScale;
	_Serializer >> FieldMapRotation;
}

void StageData::WriteStageData(GameEngineSerializer& _Serializer)
{
	_Serializer << StageName;
	_Serializer << static_cast<int>(MapDatas.size());
	for (auto i : MapDatas)
	{
		_Serializer << i.first;
		_Serializer << static_cast<int>(i.second.size());
		for (size_t j = 0; j < i.second.size(); j++)
		{
			i.second[j].WriteFieldMapData(_Serializer);
		}
	}

	_Serializer << SkyboxFileName;
	_Serializer << GroundMeshFileName;
	_Serializer << WallMeshFileName;
}

void StageData::ReadStageData(GameEngineSerializer& _Serializer)
{
	_Serializer >> StageName;
	int temp_map_size = 0;
	_Serializer >> temp_map_size;
	for (int i = 0; i < temp_map_size; i++)
	{
		int temp_first = 0;
		int temp_second_size = 0;
		std::vector<FieldMapData> temp_second;
		_Serializer >> temp_first;
		_Serializer >> temp_second_size;
		temp_second.resize(temp_second_size);
		for (int j = 0; j < temp_second_size; j++)
		{
			temp_second[j].ReadFieldMapData(_Serializer);
		}

		MapDatas.insert(std::make_pair(temp_first, temp_second));
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
