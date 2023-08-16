#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>


class ObjectUpdatePacket : public GameEnginePacket
{
public:
	ObjectUpdatePacket()
	{
		SetPacketID(PacketEnum::ObjectUpdatePacket);
	}

	~ObjectUpdatePacket() override
	{

	}

protected:

private:
	float4 Pos;
	int Animation;
};

