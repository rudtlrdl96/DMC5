#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>

#include "PacketEnum.h"
#include "ContentsEnum.h"

class FsmChangePacket : public GameEnginePacket
{
	friend class NetworkManager;

public:
	static const PacketEnum Type = PacketEnum::FsmChangePacket;

public:
	FsmChangePacket()
	{
		SetPacketID(Type);
	}

	~FsmChangePacket() override
	{

	}

	int FsmState = -1;
	int AttackerID = 0;

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);

		_Ser << FsmState;
		_Ser << AttackerID;
		_Ser << NetID;
		
		unsigned int Level = static_cast<int>(LevelType);
		_Ser << Level;
	}

	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);

		_Ser >> FsmState;
		_Ser >> AttackerID;
		_Ser >> NetID;

		unsigned int Level = 0;
		_Ser >> Level;
		LevelType = static_cast<Net_LevelType>(Level);
	}

private:
	unsigned int NetID = -1;
	Net_LevelType LevelType = Net_LevelType::UNKNOWN;
};

