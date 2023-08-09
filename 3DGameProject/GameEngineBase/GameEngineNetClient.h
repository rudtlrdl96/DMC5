#pragma once
#include "GameEngineNet.h"
#include "GameEngineThread.h"

// Ό³Έν :
class GameEngineNetClient : public GameEngineNet
{
public:
	// constrcuter destructer
	GameEngineNetClient();
	~GameEngineNetClient();

	// delete Function
	GameEngineNetClient(const GameEngineNetClient& _Other) = delete;
	GameEngineNetClient(GameEngineNetClient&& _Other) noexcept = delete;
	GameEngineNetClient& operator=(const GameEngineNetClient& _Other) = delete;
	GameEngineNetClient& operator=(GameEngineNetClient&& _Other) noexcept = delete;

	bool Connect(const std::string& _IP, unsigned short _Port);

protected:
	void Send(void* Data, unsigned int _Size) override;

private:
	GameEngineThread RecvThread;
	SOCKET ClientSocket;
};

