#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>

#include "PacketEnum.h"

class MessageChatPacket : public GameEnginePacket
{
public:
	static const PacketEnum Type = PacketEnum::MessageChatPacket;

public:
	MessageChatPacket()
	{
		SetPacketID(Type);
	}

	~MessageChatPacket() override
	{

	}

	std::string Message;

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);
		_Ser << Message;
	}

	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);
		_Ser >> Message;
	}

private:

};