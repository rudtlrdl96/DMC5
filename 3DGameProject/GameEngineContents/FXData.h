#pragma once
#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include "EffectRenderer.h"
class FXUnitData : public GameEngineSerializObject
{
public:
	std::string MeshName = "";
	std::string TextureName = "";
	AnimationParameter AnimData;
	int CutX = -1;
	int CutY = -1;

	void Write(GameEngineSerializer& _File) override
	{
		_File << MeshName;
		_File << TextureName;
		AnimData.Write(_File);
		_File << CutX;
		_File << CutY;
	}

	void Read(GameEngineSerializer& _File) override
	{
		_File >> MeshName;
		_File >> TextureName;
		AnimData.Read(_File);
		_File >> CutX;
		_File >> CutY;
	}
};

class FXKeyFrame : public GameEngineSerializObject
{
public:
	bool IsVaild = false;
	bool IsUpdate = false;
	float4 Position = float4::ZERONULL;
	float4 Rotation = float4::ZERONULL;
	float4 Scale = float4::ZERONULL;
	EffectData EffectOption;

	static FXKeyFrame Lerp(const FXKeyFrame& Start, const FXKeyFrame& End, float Ratio)
	{
		FXKeyFrame Result;
		Result.IsVaild = Start.IsVaild;
		Result.IsUpdate = Start.IsUpdate;
		Result.Position = float4::Lerp(Start.Position, End.Position, Ratio);
		Result.Rotation = float4::SLerpQuaternion(Start.Rotation, End.Rotation, Ratio);
		Result.Scale = float4::Lerp(Start.Scale, End.Scale, Ratio);
		Result.EffectOption = EffectData::Lerp(Start.EffectOption, End.EffectOption, Ratio);
		return Result;
	}

	void Write(GameEngineSerializer& _File) override
	{
		_File << IsVaild;
		_File << IsUpdate;
		_File << Position;
		_File << Rotation;
		_File << Scale;
		EffectOption.Write(_File);
	}

	void Read(GameEngineSerializer& _File) override
	{
		_File >> IsVaild;
		_File >> IsUpdate;
		_File >> Position;
		_File >> Rotation;
		_File >> Scale;
		EffectOption.Read(_File);
	}
};

// ���� :
class FXData : public GameEngineResource<FXData>, public GameEngineSerializObject
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

	std::vector<std::map<std::string, FXKeyFrame>>& GetFrameData()
	{
		return FrameData;
	}
	std::map<std::string, FXKeyFrame>& GetFrameData(int _Frame)
	{
		return FrameData[_Frame];
	}

	static std::shared_ptr<FXData> CreateData(std::vector<FXUnitData>& UnitDatas, std::map<int, std::map<int, FXKeyFrame>>& _Data);

	void Write(GameEngineSerializer& _File) override
	{
		_File << UnitDatas;
		unsigned int Size = static_cast<unsigned int>(FrameData.size());
		_File << Size;

		if (Size <= 0)
		{
			return;
		}

		for (size_t i = 0; i < FrameData.size(); i++)
		{
			unsigned int MapSize = static_cast<unsigned int>(FrameData[i].size());
			_File << MapSize;
			if (MapSize <= 0)
			{
				return;
			}
			for (std::pair <std::string, FXKeyFrame> Pair : FrameData[i])
			{

				_File << Pair.first;
				_File << Pair.second;
			}
		}

	}

	void Read(GameEngineSerializer& _File) override
	{
		_File >> UnitDatas;

		unsigned int Size = 0;
		_File >> Size;

		if (Size <= 0)
		{
			return;
		}

		FrameData.resize(Size);

		for (unsigned int i = 0; i < Size; i++)
		{
			unsigned int MapSize = 0;
			_File >> MapSize;

			if (MapSize <= 0)
			{
				return;
			}

			for (unsigned int j = 0; j < MapSize; ++j)
			{
				std::pair<std::string, FXKeyFrame> Pair;

				_File >> Pair.first;
				_File >> Pair.second;

				FrameData[i].insert(Pair);
			}
		}
	}

	void WriteFrameData(GameEngineSerializer& _File);

protected:

private:
	std::vector<FXUnitData> UnitDatas;
	//		������,		// ����,   // Ű������
	std::vector<std::map<std::string, FXKeyFrame>> FrameData;

};

