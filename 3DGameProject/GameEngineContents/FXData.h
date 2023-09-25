#pragma once
#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include "EffectRenderer.h"
class FXUnitData : public GameEngineSerializObject
{
public:
	std::string MeshName = "";
	std::string TextureName = "";
	std::string AnimationName = "";
	int CutX = -1;
	int CutY = -1;

	void Write(GameEngineSerializer& _File) override
	{
		_File << MeshName;
		_File << TextureName;
		_File << AnimationName;
		_File << CutX;
		_File << CutY;
	}

	void Read(GameEngineSerializer& _File) override
	{
		_File >> MeshName;
		_File >> TextureName;
		_File >> AnimationName;
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

// 설명 :
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

	static std::shared_ptr<FXData> Load(const std::string_view& _Path)
	{
		return Load(_Path, GameEnginePath::GetFileName(_Path));
	}

	static std::shared_ptr<FXData> Load(const std::string_view& _Path, const std::string_view& _Name)
	{
		std::shared_ptr<FXData> NewFX = GameEngineResource::Create(_Name);
		//NewFX
		GameEngineFile File;
		GameEngineSerializer Ser;
		File.SetPath(_Path);
		File.LoadBin(Ser);

		NewFX->Read(Ser);

		return NewFX;
	}

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
		unsigned int Size = static_cast<unsigned int>(UnitDatas.size());
		_File << Size;
		for (size_t i = 0; i < UnitDatas.size(); i++)
		{
			UnitDatas[i].Write(_File);
		}
		Size = static_cast<unsigned int>(FrameData.size());
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
				continue;
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
		unsigned int Size = static_cast<unsigned int>(UnitDatas.size());
		_File >> Size;
		UnitDatas.resize(Size);
		for (size_t i = 0; i < UnitDatas.size(); i++)
		{
			 UnitDatas[i].Read(_File);
		}

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
				continue;
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
	//		프레임,		// 유닛,   // 키프레임
	std::vector<std::map<std::string, FXKeyFrame>> FrameData;

};

