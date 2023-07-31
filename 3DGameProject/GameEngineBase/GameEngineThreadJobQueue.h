#pragma once
#include <functional>
#include <Windows.h>
#include "GameEngineThread.h"

enum ThreadWorkType
{
	// �����϶��� �������� ���ƿ� ����Ʈũ�Ⱑ ���ƿ��� ������
	// ������
	UserWork = -1,
	Destroy = -2
};

// ���� : IOCP�� ����� ������ 
// IOCP ����ϴ� => ���ڱ� �ѵ�� �������� �־�.
// �� ���� IOCP�����ϴµ� �� ��¥ ���� �����ߴµ�.
// �� �󸶳� �ƴ�? �츮 ��̰� IOCP�̾߱⳪ ���� �׷��� �׷���...
class GameEngineThreadJobQueue
{
private:
	class Job
	{
	public:
		std::function<void(GameEngineThread*)> Function;
	};

public:
	// constrcuter destructer
	GameEngineThreadJobQueue();
	~GameEngineThreadJobQueue();

	// delete Function
	GameEngineThreadJobQueue(const GameEngineThreadJobQueue& _Other) = delete;
	GameEngineThreadJobQueue(GameEngineThreadJobQueue&& _Other) noexcept = delete;
	GameEngineThreadJobQueue& operator=(const GameEngineThreadJobQueue& _Other) = delete;
	GameEngineThreadJobQueue& operator=(GameEngineThreadJobQueue&& _Other) noexcept = delete;

	void Work(std::function<void(GameEngineThread*)> _Work);

	void Initialize(const std::string& _ThreadName, int _ThreadCount = 0);

protected:


private:

	HANDLE IOCPHandle = nullptr;
	int ThreadCount;

	static std::atomic_int RunningThreadCount;
	std::atomic_bool IsRun = true;
	std::vector<std::shared_ptr<GameEngineThread>> AllThread;

	static void ThreadPoolFunction(GameEngineThreadJobQueue* _ThreadPool, GameEngineThread* _Thread, HANDLE _IOCPHandle);
};

