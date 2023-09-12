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
		:UnionDatas{0,}
	{
		SetPacketID(Type);
	}

	~ObjectUpdatePacket() override
	{

	}

	float4 Rotation;
	float4 Position;
	float TimeScale = 1.f;


	union 
	{
		unsigned int UnionDatas[NETWORK_UPDATEPACKET_UNION_DATA_SIZE];

		//플레이어의 경우
		struct 
		{
			unsigned int ArmState;
			unsigned int IsEvolve;		//0일때 변신X, 1일때 변신
		};

		//몬스터의 경우
		struct
		{
			unsigned int MONSTER_UNKNOWN1;
			unsigned int MONSTER_UNKNOWN2;
		};

		//TODO
	};

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

		unsigned int Level = static_cast<int>(LevelType);
		_Ser << Level;

		for (size_t i = 0; i < NETWORK_UPDATEPACKET_UNION_DATA_SIZE; ++i)
		{
			_Ser << UnionDatas[i];
		}
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

		unsigned int Level = 0;
		_Ser >> Level;
		LevelType = static_cast<Net_LevelType>(Level);

		for (size_t i = 0; i < NETWORK_UPDATEPACKET_UNION_DATA_SIZE; ++i)
		{
			_Ser >> UnionDatas[i];
		}
	}

private:
	unsigned int NetID = -1;
	unsigned int ActorType = -1;
	Net_LevelType LevelType = Net_LevelType::UNKNOWN;
	bool IsDeath = false;

	void CopyUnionData(const void* const _UnionArrPtr)
	{
		rsize_t Size = sizeof(unsigned int) * NETWORK_UPDATEPACKET_UNION_DATA_SIZE;
		memcpy_s(&UnionDatas[0], Size, _UnionArrPtr, Size);
	}
};

