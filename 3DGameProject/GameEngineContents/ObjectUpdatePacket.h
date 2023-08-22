#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>

#include "PacketEnum.h"

class ObjectUpdatePacket : public GameEnginePacket
{
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

	unsigned int ActorType = -1;
	//unsigned int LevelType = -1;

	float4 Rotation;
	float4 Position;
	float TimeScale = 1.f;

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);
		
		_Ser << ActorType;
		//_Ser << LevelType;

		_Ser << Rotation;
		_Ser << Position;

		_Ser << TimeScale;
	}

	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);

		_Ser >> ActorType;
		//_Ser >> LevelType;

		_Ser >> Rotation;
		_Ser >> Position;

		_Ser >> TimeScale;
	}

private:
	
};

