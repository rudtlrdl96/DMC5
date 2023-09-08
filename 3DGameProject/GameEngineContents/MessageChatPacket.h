#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>

#include "PacketEnum.h"

class MessageChatPacket : public GameEnginePacket
{
	friend class NetworkManager;

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

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);
		_Ser << Message;
		_Ser << Color;
	}

	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);
		_Ser >> Message;
		_Ser >> Color;
	}

private:
	std::string Message;
	float4 Color = float4::WHITE;
};