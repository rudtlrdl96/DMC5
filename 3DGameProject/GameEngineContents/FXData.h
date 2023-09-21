#pragma once
#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include "EffectRenderer.h"
class FXUnitData
{
public:
	std::string MeshName = "";
	std::string TextureName = "";
	AnimationParameter AnimData;
	int CutX = -1;
	int CutY = -1;
};

class FXKeyFrame
{
public:
	bool IsUpdate = false;
	float4 Position = float4::ZERONULL;
	float4 Rotation = float4::ZERONULL;
	float4 Scale = float4::ZERONULL;
	EffectData EffectOption;
};

// 설명 :
class FXData : public GameEngineResource<FXData>
{
public:
	// constrcuter destructer
	FXData();
	~FXData();

	// delete Function
	FXData(const FXData& _Other) = delete;
	FXData(FXData&& _Other) noexcept = delete;
	FXData& operator=(const FXData& _Other) = delete;
	FXData& operator=(FXData&& _Other) noexcept = delete;

	std::vector<FXUnitData>& GetUnitDatas()
	{
		return UnitDatas;
	}

	std::vector<std::vector<FXKeyFrame>>& GetFrameData()
	{
		return FrameData;
	}
	std::vector<FXKeyFrame>& GetFrameData(int _Frame)
	{
		return FrameData[_Frame];
	}
protected:

private:
	std::vector<FXUnitData> UnitDatas;
	//		프레임,		// 유닛,   // 키프레임
	std::vector<std::vector<FXKeyFrame>> FrameData;

};

