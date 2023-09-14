#pragma once
#include "ContentsEnum.h"

class ObjTransformData
{
public:
	float4 Pos = float4::ZERO;
	float4 Scale = float4::ONE;
	float4 Rot = float4::ZERO;

	void WriteObjTransformData(GameEngineSerializer& _Serializer);
	void ReadObjTransformData(GameEngineSerializer& _Serializer);
};

class FieldMapObjData
{
public:
	FieldMapObjType Type = FieldMapObjType::Null;
	ObjTransformData ObjTransform;
	
	void WriteObjData(GameEngineSerializer& _Serializer);
	void ReadObjData(GameEngineSerializer& _Serializer);
};

class FieldMapData
{
public:
	std::vector<std::string> PartsMeshFileNames;
	std::vector<ObjTransformData> CullingColTransform;
	std::vector<int> NodeIndex;
	std::vector<FieldMapObjData> MapObjData;

	void WriteFieldMapData(GameEngineSerializer& _Serializer);
	void ReadFieldMapData(GameEngineSerializer& _Serializer);
};

class StageData
{
public:
	std::string StageName = "";
	std::vector<FieldMapData> MapDatas;
	std::string SkyboxFileName = "";
	std::string GroundMeshFileName = "";
	std::string WallMeshFileName = "";
	ObjTransformData StartPosition;

	void WriteStageData(GameEngineSerializer& _Serializer);
	void ReadStageData(GameEngineSerializer& _Serializer);
	static void WriteAllStageData(GameEngineSerializer& _Serializer, std::vector<StageData>& _AllData);
	static void ReadAllStageData(GameEngineSerializer& _Serializer, std::vector<StageData>& _AllData);
};
