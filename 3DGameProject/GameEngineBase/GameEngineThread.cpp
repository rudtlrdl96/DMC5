#include "PrecompileHeader.h"
#include "GameEngineThread.h"
#include "GameEngineString.h"

std::atomic_bool GameEngineThread::End = false;

GameEngineThread::GameEngineThread() 
{
}

GameEngineThread::GameEngineThread(std::string _ThreadName, std::function<void(GameEngineThread*)> _CallBack)
{
	Start(_ThreadName, _CallBack);
}

GameEngineThread::~GameEngineThread() 
{
	Join();
}

void GameEngineThread::Start(std::string _ThreadName, std::function<void(GameEngineThread*)> _CallBack)
{
	CallBack = _CallBack;
	Thread = std::thread(ThreadBaseFunction, this, _ThreadName);

}

void GameEngineThread::ThreadBaseFunction(GameEngineThread* Thread, std::string Name) 
{
	// ��������װ� �־��ΰ�
	// ���� �������� �ʴ�.
	// ���ʿ� �װ� ����ó��.

	std::wstring WName = GameEngineString::AnsiToUniCode(Name);
	// GetCurrentThreadId() �츮�� ������ ��û�ϸ� ������� �츮 �����忡 id�� �ο��ϴµ�
	SetThreadDescription(GetCurrentThread(), WName.c_str());
	Thread->CallBack(Thread);
}

void GameEngineThread::Join()
{
	// �������� �ų��� 100���� ���ư��� �־�.
	// CPu�� 100%ġ�� �ְ� �ٸ� �������� ����� ������ ���ٸ�
	// ���� ������ �ٸ� ���� �ϳ��� Sleep�ϰų� �������Ѽ� ���Ѿָ� ����ó���ϰ� �������� 
	// ���ؽ�Ʈ ����Ī�� ���� �ƴϾ�.

	if (nullptr != CallBack)
	{
		// Sleep(0);
		Thread.join();
		CallBack = nullptr;
	}
}