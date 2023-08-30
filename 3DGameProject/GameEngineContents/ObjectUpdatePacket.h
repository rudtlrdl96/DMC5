#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>

#include "PacketEnum.h"

class ObjectUpdatePacket : public GameEnginePacket
{
	friend class NetworkManager;

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
	float TimeScale = 1.f;
	bool IsDeath = false;

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);
		
		_Ser << NetID;
		_Ser << ActorType;

		_Ser << Rotation;
		_Ser << Position;

		_Ser << TimeScale;
		_Ser << IsDeath;
	}

	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);

		_Ser >> NetID;
		_Ser >> ActorType;

		_Ser >> Rotation;
		_Ser >> Position;

		_Ser >> TimeScale;
		_Ser >> IsDeath;
	}

private:
	unsigned int NetID = -1;
	unsigned int ActorType = -1;
};

