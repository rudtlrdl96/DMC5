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
	//��Ŷ ���̵�
	unsigned int PacketID = -1; 
	//��Ŷ ũ��
	unsigned int Size = -1; 
	//������Ʈ ���̵�
	unsigned int ObjectID = -1; 
};

