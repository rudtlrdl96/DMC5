#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>

#include "PacketEnum.h"
#include "ContentsEnum.h"

class ObjectUpdatePacket : public GameEnginePacket
{
	friend class NetworkManager;

public:
	static const PacketEnum Type = PacketEnum::ObjectUpdatePacket;

public:
	ObjectUpdatePacket()
	{
		SetPacketID(Type);
	}

	~ObjectUpdatePacket() override
	{

	}

	inline const std::vector<int>& GetIntDatas() const
	{
		return IntDatas;
	}

	inline const std::vector<float>& GetFloatDatas() const
	{
		return FloatDatas;
	}

	inline const std::vector<bool>& GetBoolDatas() const
	{
		return BoolDatas;
	}

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);
		
		_Ser << NetID;
		_Ser << ActorType;

		_Ser << Rotation;
		_Ser << Position;

		_Ser << TimeScale;
		_Ser << IsDeath;
		_Ser << IsUpdate;

		unsigned int Level = static_cast<int>(LevelType);
		_Ser << Level;

		SerializeArr<int>(IntDatas, _Ser);
		SerializeArr<float>(FloatDatas, _Ser);
		SerializeArr<bool>(BoolDatas, _Ser);
	}

	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);

		_Ser >> NetID;
		_Ser >> ActorType;

		_Ser >> Rotation;
		_Ser >> Position;

		_Ser >> TimeScale;
		_Ser >> IsDeath;
		_Ser >> IsUpdate;

		unsigned int Level = 0;
		_Ser >> Level;
		LevelType = static_cast<Net_LevelType>(Level);

		DeSerializeArr<int>(IntDatas, _Ser);
		DeSerializeArr<float>(FloatDatas, _Ser);
		DeSerializeArr<bool>(BoolDatas, _Ser);
	}

private:
	unsigned int NetID = -1;
	unsigned int ActorType = -1;

	float4 Rotation;
	float4 Position;
	float TimeScale = 1.f;

	Net_LevelType LevelType = Net_LevelType::UNKNOWN;
	bool IsDeath = false;
	std::vector<bool> d;
	bool IsUpdate = true;

	std::vector<int> IntDatas;
	std::vector<float> FloatDatas;
	std::vector<bool> BoolDatas;


	template <typename DataType>
	void SerializeArr(const std::vector<DataType>& _Datas, GameEngineSerializer& _Ser)
	{
		unsigned int Size = static_cast<unsigned int>(_Datas.size());
		_Ser << (Size);

		if (Size <= 0)
			return;

		for (DataType Data : _Datas)
		{
			_Ser << Data;
		}
	}

	template <typename DataType>
	void DeSerializeArr(std::vector<DataType>& _Datas, GameEngineSerializer& _Ser)
	{
		unsigned int Size = 0;
		_Ser >> (Size);

		if (Size <= 0)
			return;

		_Datas.resize(Size);

		for (unsigned int i = 0; i < Size; i++)
		{
			DataType Data;
			_Ser >> Data;
			_Datas[i] = Data;
		}
	}

};

