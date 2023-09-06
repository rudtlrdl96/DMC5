#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>

#include "PacketEnum.h"
#include "ContentsEnum.h"

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

	unsigned int FsmState = -1;
	bool IsFsmForce = false;

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

		unsigned int Level = static_cast<int>(LevelType);
		_Ser << Level;

		_Ser << FsmState;
		_Ser << IsFsmForce;
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

		unsigned int Level = 0;
		_Ser >> Level;
		LevelType = static_cast<Net_LevelType>(Level);

		_Ser >> FsmState;
		_Ser >> IsFsmForce;
	}

private:
	unsigned int NetID = -1;
	unsigned int ActorType = -1;
	Net_LevelType LevelType = Net_LevelType::UNKNOWN;
	bool IsDeath = false;

};

