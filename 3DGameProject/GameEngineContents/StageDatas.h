#pragma once
#include "ContentsEnum.h"

class FieldMapColData
{
public:
	float4 Pos = float4::ZERONULL;
	float4 Scale = float4::ZERONULL;
	float4 Rot = float4::ZERONULL;
	ColType Type = ColType::MAX;
	CollisionOrder ColOrder = CollisionOrder::Null;

	void WriteFieldMapColData(GameEngineSerializer& _Serializer);
	void ReadFieldMapColData(GameEngineSerializer& _Serializer);
};

class FieldMapData
{
public:
	std::string MeshFileName = "";
	std::vector<FieldMapColData> ColDatas;
	float4 FieldMapPosition = float4::ZERONULL;

	void WriteFieldMapData(GameEngineSerializer& _Serializer);
	void ReadFieldMapData(GameEngineSerializer& _Serializer);
};

class StageData
{
public:
	std::string StageName = "";
	std::vector<FieldMapData> MapDatas;
	std::string SkyboxFileName = "";

	void WriteStageData(GameEngineSerializer& _Serializer);
	void ReadStageData(GameEngineSerializer& _Serializer);
	static void WriteAllStageData(GameEngineSerializer& _Serializer, std::vector<StageData>& _AllData);
	static void ReadAllStageData(GameEngineSerializer& _Serializer, std::vector<StageData>& _AllData);
};
