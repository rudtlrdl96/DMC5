#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>

#include "PacketEnum.h"

class ConnectIDPacket : public GameEnginePacket
{
public:
	static const PacketEnum Type = PacketEnum::ConnectIDPacket;

public:
	ConnectIDPacket()
	{
		SetPacketID(Type);
	}

	~ConnectIDPacket() override
	{

	}
	
protected:

private:

};

