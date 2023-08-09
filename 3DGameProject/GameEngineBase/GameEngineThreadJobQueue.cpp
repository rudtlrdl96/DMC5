#include "PrecompileHeader.h"
#include "GameEngineThreadJobQueue.h"
#include "GameEngineDebug.h"

std::atomic_int GameEngineThreadJobQueue::RunningThreadCount = 0;

GameEngineThreadJobQueue::GameEngineThreadJobQueue() 
{
}

GameEngineThreadJobQueue::~GameEngineThreadJobQueue() 
{
	IsRun = false;

	while (RunningThreadCount)
	{
		// 쓰레드 깨우는 함수
		if (
			FALSE == 
			PostQueuedCompletionStatus(IOCPHandle, static_cast<DWORD>(ThreadWorkType::Destroy), 0, nullptr)
			)
		{
			MsgAssert("쓰레드에게 디스트로이 명령을 내리는데 실패했습니다.");
		}

		Sleep(0);
	}
}

void GameEngineThreadJobQueue::ThreadPoolFunction(GameEngineThreadJobQueue* _ThreadPool, GameEngineThread* _Thread, HANDLE _IOCPHandle) 
{
	DWORD Byte;
	ULONG_PTR PtrKey;
	LPOVERLAPPED OverLapped = nullptr;

	while (true == _ThreadPool->IsRun)
	{
		GetQueuedCompletionStatus(_IOCPHandle, &Byte, &PtrKey, &OverLapped, INFINITE);

		ThreadWorkType WorkType = static_cast<ThreadWorkType>(Byte);
		switch (WorkType)
		{
		case UserWork:
		{
			Job* JobPtr = reinterpret_cast<Job*>(PtrKey);

			if (nullptr != JobPtr->Function)
			{
				JobPtr->Function(_Thread);
			}
			delete JobPtr;
			break;
		}
		case Destroy:
			--RunningThreadCount;
			return;
		default:
			break;
		}
	}
}

// 보통 스레드 선언 시의 갯수는 (자신의 CPU 코어개수 * 2 - 1)로 설정
void GameEngineThreadJobQueue::Initialize(const std::string& _ThreadName, int _ThreadCount)
{
	ThreadCount = _ThreadCount;

	if (0 == ThreadCount)
	{
		// systeminfo에서 cpu의 코어 갯수 가져오기
		SYSTEM_INFO Info;
		GetSystemInfo(&Info); 
		ThreadCount = Info.dwNumberOfProcessors;
	}

	// IOCP 핸들 생성
	IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, ThreadCount);

	if (nullptr == IOCPHandle)
	{
		MsgAssert("IOCP핸들 생성에 실패했습니다.");
	}

	RunningThreadCount = ThreadCount;
	AllThread.resize(ThreadCount);

	for (size_t i = 0; i < ThreadCount; i++)
	{
		// 스레드 생성 함수
		AllThread[i] = std::make_shared<GameEngineThread>();
		AllThread[i]->Start(_ThreadName + " " + std::to_string(i), std::bind(ThreadPoolFunction, this, AllThread[i].get(), IOCPHandle));
	}
}

void GameEngineThreadJobQueue::Work(std::function<void(GameEngineThread*)> _Work)
{
	Job* NewJob = new Job();
	NewJob->Function = _Work;
	PostQueuedCompletionStatus(IOCPHandle, static_cast<DWORD>(ThreadWorkType::UserWork), reinterpret_cast<unsigned __int64>(NewJob), nullptr);
}