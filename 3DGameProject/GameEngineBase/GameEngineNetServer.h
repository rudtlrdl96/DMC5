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

	//���� ���
	inline void AddUser(int _ID, SOCKET _UserSocket)
	{
		if (true == Users.contains(_ID))
		{
			MsgAssert("�̹� �����ϴ� ������ �� �����Ҽ��� �����ϴ� ID ���� �Դϴ�.");
			return;
		}

		Users[_ID] = _UserSocket;
	}

	inline SOCKET GetUser(int _ID)
	{
		std::map<int, SOCKET>::iterator FindIter = Users.find(_ID);
		if (Users.end() == FindIter)
		{
			MsgAssert(std::to_string(_ID) + " �� ������ �������� �ʽ��ϴ�");
		}

		return FindIter->second;
	}

protected:
	void Send(const char* Data, unsigned int _Size, int _IgnoreID = -1) override;

private:
	int BackLog = 512;
	SOCKET AcceptSocket;

	GameEngineThread AccpetThread;

	//<�ʱ� Ŀ��Ʈ ID, ����>
	std::map<int, SOCKET> Users;

	std::vector<std::shared_ptr<GameEngineThread>> RecvThreads;

	//Ŭ�� ������ ����Ǿ��� �� ó���� �ݹ�
	std::function<void(SOCKET _AcceptSocket, GameEngineNetServer* _Net)> AcceptCallBack = nullptr;

	static void AcceptThread(SOCKET _AcceptSocket, GameEngineNetServer* _Net);

	std::mutex SendTextLock;
};

