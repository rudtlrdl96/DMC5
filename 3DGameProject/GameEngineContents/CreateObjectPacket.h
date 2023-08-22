#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>

#include "PacketEnum.h"
//#include "NetworkManager.h"

class CreateObjectPacket : public GameEnginePacket
{
public:
	static const PacketEnum Type = PacketEnum::CreateObjectPacket;

public:
	CreateObjectPacket()
	{
		SetPacketID(Type);
		//SetObjectID(NetworkManager::GetNetID());
	}

	~CreateObjectPacket() override
	{

	}

	unsigned int ActorType = -1;

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);

		_Ser << ActorType;
	}

	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);

		_Ser >> ActorType;
	}


private:

};