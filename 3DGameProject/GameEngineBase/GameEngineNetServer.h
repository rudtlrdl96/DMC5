#pragma once
#include "GameEngineNet.h"
#include "GameEngineThread.h"

// 설명 :
class GameEngineNetServer : public GameEngineNet
{
public:
	// constrcuter destructer
	GameEngineNetServer();
	~GameEngineNetServer();

	// delete Function
	GameEngineNetServer(const GameEngineNetServer& _Other) = delete;
	GameEngineNetServer(GameEngineNetServer&& _Other) noexcept = delete;
	GameEngineNetServer& operator=(const GameEngineNetServer& _Other) = delete;
	GameEngineNetServer& operator=(GameEngineNetServer&& _Other) noexcept = delete;

	//백로그는 최대 동접자수
	void ServerOpen(short _Port, int _BackLog = 512);

protected:
	void Send(void* Data, unsigned int _Size) override;

private:
	int BackLog = 512;
	SOCKET AcceptSocket;

	GameEngineThread AccpetThread;

	std::vector<SOCKET> Users;
	std::vector<std::shared_ptr<GameEngineThread>> RecvThreads;

	static void AcceptThread(SOCKET _AcceptSocket, GameEngineNetServer* _Net);
};

