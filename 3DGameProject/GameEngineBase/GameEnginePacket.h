#pragma once


class GameEnginePacket : public std::enable_shared_from_this<GameEnginePacket>
{
public:
	GameEnginePacket();
	~GameEnginePacket();

	GameEnginePacket(const GameEnginePacket& _Other) = delete;
	GameEnginePacket(GameEnginePacket&& _Other) noexcept = delete;
	GameEnginePacket& operator=(const GameEnginePacket& _Other) = delete;
	GameEnginePacket& operator=(const GameEnginePacket&& _Other) noexcept = delete;

protected:

private:
	//패킷 아이디
	unsigned int PacketID = -1; 
	//패킷 크기
	unsigned int Size = -1; 
	//오브젝트 아이디
	unsigned int ObjectID = -1; 
};

