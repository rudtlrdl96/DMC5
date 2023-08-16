#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>

#include "PacketEnum.h"

class ConnectIDPacket : public GameEnginePacket
{
public:
	ConnectIDPacket()
	{
		SetPacketID(PacketEnum::ConnectIDPacket);
	}

	~ConnectIDPacket() override
	{

	}
	
protected:

private:

};

