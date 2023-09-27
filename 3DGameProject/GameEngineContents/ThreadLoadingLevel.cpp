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

	//NetworkTestLevel
	PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Mesh\\Nero.fbx");
	PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0010_03_atos.texout.png");
	PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0000_03_atos.texout.png");
	//PushAllLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character", {".fbx"});
	//PushAllLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character", {".png"});

}

#include "NetworkGUI.h"

void ThreadLoadingLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

	if (LoadWorkCount == ExcuteWorkCount)
	{
		GameEngineCore::ChangeLevel(NextLevelName);
		return;
	}

	//TODO
	NetworkGUI::GetInst()->PrintLog("Thread Load Percent : " + std::to_string(LoadingPercent), float4{ 0.f, 1.f, 1.f, 1.f });
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
	LoadingPercent = 0.f;

	//�� �κ� �ϴ��� �Ͷ߸�����. ���߿� ���� �ּ�ó�� �� �� �Ǵ�����
	if (true == CallBacks.empty())
	{
		MsgAssert(NextLevelName + "������ ������ �ε��� ������� ���ҽ��� �������� �ʽ��ϴ�");
		return;
	}

	for (const std::function<void()>& CallBack : CallBacks)
	{
		GameEngineCore::JobQueue.Work([this, CallBack](GameEngineThread*)
		{
			CallBack();
			++ExcuteWorkCount;
			LoadingPercent = static_cast<float>(ExcuteWorkCount) / static_cast<float>(LoadWorkCount);
		});
	}
}

