#pragma once
#include "ContentsEnum.h"

class FieldMapColData
{
public:
	float4 Pos = float4::ZERO;
	float4 Scale = float4::ONE;
	float4 Rot = float4::ZERO;
	ColType Type = ColType::MAX;
	CollisionOrder ColOrder = CollisionOrder::Null;

	void WriteFieldMapColData(GameEngineSerializer& _Serializer);
	void ReadFieldMapColData(GameEngineSerializer& _Serializer);
};

class FieldMapData
{
public:
	std::string MeshFileName = "";
	float4 FieldMapPosition = float4::ZERO;
	float4 FieldMapScale = float4::ONE;
	float4 FieldMapRotation = float4::ZERO;

	void WriteFieldMapData(GameEngineSerializer& _Serializer);
	void ReadFieldMapData(GameEngineSerializer& _Serializer);
};

class StageData
{
public:
	std::string StageName = "";
	std::map<int, std::vector<FieldMapData>> MapDatas;
	std::string SkyboxFileName = "";
	std::string GroundMeshFileName = "";
	std::string WallMeshFileName = "";

	void WriteStageData(GameEngineSerializer& _Serializer);
	void ReadStageData(GameEngineSerializer& _Serializer);
	static void WriteAllStageData(GameEngineSerializer& _Serializer, std::vector<StageData>& _AllData);
	static void ReadAllStageData(GameEngineSerializer& _Serializer, std::vector<StageData>& _AllData);
};
