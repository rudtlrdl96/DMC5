#pragma once
#include <GameEngineCore/GameEngineTransform.h>

class FieldMapColData
{
public:
	float4 Pos = float4::ZERONULL;
	float4 Scale = float4::ZERONULL;
	float4 Rot = float4::ZERONULL;
	ColType Type = ColType::MAX;
	int ColOrder = -1;
};

class FieldMapData
{
public:
	std::string MeshFileName;
	std::vector<FieldMapColData> ColDatas;
	float4 FieldMapPosition = float4::ZERONULL;
};

class StageData
{
public:
	int StageID = -1;
	std::vector<FieldMapData> MapDatas;
};
