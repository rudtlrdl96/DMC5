#pragma once
#include "GameEngineNet.h"
#include "GameEngineThread.h"

// 설명 :
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

	//ip가 127.0.0.1인 경우 로컬서버를 의미
	bool Connect(const std::string& _IP, unsigned short _Port);

protected:
	void Send(const char* Data, unsigned int _Size) override;

private:
	GameEngineThread RecvThread;
	SOCKET ClientSocket;
};

