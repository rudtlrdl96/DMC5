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

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);

		unsigned short EvtType = static_cast<unsigned int>(EventType);
		_Ser << EvtType;
	}

	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);

		unsigned short EvtType;
		_Ser >> EvtType;
		EventType = static_cast<Net_EventType>(EvtType);
	}

private:
	Net_EventType EventType = Net_EventType::UNKNOWN;
};

