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

	inline std::vector<int> GetAllTarget() const
	{
		return AllTargetID;
	}

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);

		_Ser << EventType;
		_Ser << AllTargetID;
	}

	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);

		_Ser >> EventType;
		_Ser >> AllTargetID;
	}

private:
	int EventType = -1;
	std::vector<int> AllTargetID;
};

