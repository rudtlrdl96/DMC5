#pragma once
#include <functional>
#include <Windows.h>
#include "GameEngineThread.h"

enum ThreadWorkType
{
	// 서버를 활용할 때에는, 서버에서 전송되는 바이트 크기를 입력한다.
	UserWork = -1,
	Destroy = -2
};

// 설명 : IOCP를 사용한 쓰레드 
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
	int ThreadCount = 0;

	static std::atomic_int RunningThreadCount;
	std::atomic_bool IsRun = true;
	std::vector<std::shared_ptr<GameEngineThread>> AllThread;

	static void ThreadPoolFunction(GameEngineThreadJobQueue* _ThreadPool, GameEngineThread* _Thread, HANDLE _IOCPHandle);
};

