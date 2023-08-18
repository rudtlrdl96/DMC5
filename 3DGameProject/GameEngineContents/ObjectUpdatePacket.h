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

	float4 Rotation;
	float4 Position;

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);
		_Ser << Rotation;
		_Ser << Position;
	}

	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);
		_Ser >> Rotation;
		_Ser >> Position;
	}

private:
	
};

