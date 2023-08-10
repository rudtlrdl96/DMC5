#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>

class ConnectIDPacket : public GameEnginePacket
{
public:
	ConnectIDPacket();
	~ConnectIDPacket();

	ConnectIDPacket(const ConnectIDPacket& _Other) = delete;
	ConnectIDPacket(ConnectIDPacket&& _Other) noexcept = delete;
	ConnectIDPacket& operator=(const ConnectIDPacket& _Other) = delete;
	ConnectIDPacket& operator=(const ConnectIDPacket&& _Other) noexcept = delete;

protected:

private:

};

