#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>

#include "PacketEnum.h"

class LinkObjectPacket : public GameEnginePacket
{
	friend class NetworkManager;

public:
	static const PacketEnum Type = PacketEnum::LinkObjectPacket;

public:
	LinkObjectPacket()
	{
		SetPacketID(Type);
	}

	~LinkObjectPacket() override
	{

	}

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);

		_Ser << Ptr;
		_Ser << ActorType;
	}

	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);

		_Ser >> Ptr;
		_Ser >> ActorType;
	}

private:
	unsigned __int64 Ptr = 0;
	unsigned int ActorType = -1;
};

