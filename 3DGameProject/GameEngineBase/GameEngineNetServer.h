#pragma once
#include "GameEngineNet.h"
#include "GameEngineThread.h"

// ���� :
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

	//��α״� �ִ� �����ڼ�
	void ServerOpen(short _Port, int _BackLog = 512);

	//Ŭ�� ������ ����Ǿ��� �� ó���� �ݹ� ���
	inline void SetAcceptCallBack(std::function<void(SOCKET, GameEngineNetServer*)> _AccpetCallBack)
	{
		AcceptCallBack = _AccpetCallBack;
	}

protected:
	void Send(const char* Data, unsigned int _Size) override;

private:
	int BackLog = 512;
	SOCKET AcceptSocket;

	GameEngineThread AccpetThread;

	std::vector<SOCKET> Users;
	std::vector<std::shared_ptr<GameEngineThread>> RecvThreads;

	//Ŭ�� ������ ����Ǿ��� �� ó���� �ݹ�
	std::function<void(SOCKET _AcceptSocket, GameEngineNetServer* _Net)> AcceptCallBack = nullptr;

	static void AcceptThread(SOCKET _AcceptSocket, GameEngineNetServer* _Net);
};

