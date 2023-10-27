#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>

#include "PacketEnum.h"
#include "ContentsEnum.h"

class NetEventPacket : public GameEnginePacket
{
	friend class NetworkManager;

public:
	static const PacketEnum Type = PacketEnum::NetEventPacket;

public:
	NetEventPacket()
	{
		SetPacketID(Type);
	}

	~NetEventPacket() override
	{

	}

	inline int GetEventType() const
	{
		return EventType;
	}

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);

		_Ser << EventType;
	}

	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);

		_Ser >> EventType;
	}

private:
	int EventType = -1;
};

