#include "PrecompileHeader.h"
#include "ThreadLoadingLevel.h"

#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineTexture.h>

#include "NetworkTestLevel.h"

std::string ThreadLoadingLevel::NextLevelName;

ThreadLoadingLevel::ThreadLoadingLevel()
{

}

ThreadLoadingLevel::~ThreadLoadingLevel()
{

}


void ThreadLoadingLevel::Start()
{
	BaseLevel::Start();
	GetCamera(0)->SetProjectionType(CameraType::Perspective);

	PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Mesh\\Nero.fbx");
	PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0010_03_atos.texout.png");
	PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0000_03_atos.texout.png");


}


void ThreadLoadingLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

	if (LoadWorkCount == ExcuteWorkCount)
	{
		GameEngineCore::ChangeLevel(NextLevelName);
		return;
	}

	//TODO
}


void ThreadLoadingLevel::LevelChangeStart()
{
	BaseLevel::LevelChangeStart();

	ThreadLoadStart();
}

void ThreadLoadingLevel::ThreadLoadStart()
{
	const std::vector<std::function<void()>>& CallBacks = AllLoadCallBack[NextLevelName];
	LoadWorkCount = CallBacks.size();
	ExcuteWorkCount = 0;

	for (const std::function<void()>& CallBack : CallBacks)
	{
		GameEngineCore::JobQueue.Work([this, CallBack](GameEngineThread*)
		{
			CallBack();
			++ExcuteWorkCount;
		});
	}
}