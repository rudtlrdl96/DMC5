#pragma once

class FieldMapColData
{
public:
	float4 Pos = float4::ZERONULL;
	float4 Scale = float4::ZERONULL;
	float4 Rot = float4::ZERONULL;
	int ColType = -1;
};

class FieldMapData
{
public:
	std::string MeshFileName;
	std::vector<FieldMapColData> ColData;
};

class StageData
{
public:
	int StageID = -1;
	std::vector<FieldMapData> MapDatas;
};
