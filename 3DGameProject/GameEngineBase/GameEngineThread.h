#pragma once
#include <string>
#include <functional>
#include <atomic>
#include <thread>

// Ό³Έν :
class GameEngineThread
{
public:
	// constrcuter destructer
	GameEngineThread();
	GameEngineThread(std::string _ThreadName, std::function<void()> _CallBack);
	~GameEngineThread();

	//// delete Function
	//GameEngineThread(const GameEngineThread& _Other) = delete;
	//GameEngineThread(GameEngineThread&& _Other) noexcept = delete;
	//GameEngineThread& operator=(const GameEngineThread& _Other) = delete;
	//GameEngineThread& operator=(GameEngineThread&& _Other) noexcept = delete;

	void Start(std::string _ThreadName, std::function<void()> _CallBack);

	void Join();

	static void ThreadEnd() 
	{
		End = true;
	}

	static bool IsThreadEnd()
	{
		return End;
	}

protected:

private:
	static std::atomic_bool End;
	static void ThreadBaseFunction(GameEngineThread* Thread, std::string Name);

	std::thread Thread;
	std::function<void()> CallBack;
};

