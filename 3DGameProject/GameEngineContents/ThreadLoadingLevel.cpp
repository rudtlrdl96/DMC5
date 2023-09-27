#include "PrecompileHeader.h"
#include "ThreadLoadingLevel.h"

#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineTexture.h>
#include <GameEngineCore/GameEngineRenderer.h>

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

	//스레드 로딩이 잘 작동되는지 확인하려는 임시 엑터입니다. 나중에 삭제할 예정이에요
	ThreadTestActor = CreateActor<GameEngineActor>();
	std::shared_ptr<GameEngineRenderer> TestRenderer = nullptr;
	TestRenderer = ThreadTestActor->CreateComponent<GameEngineRenderer>();
	TestRenderer->CreateRenderUnit("sphere", "MeshTexture");
	TestRenderer->GetTransform()->SetLocalScale({ 100.0f, 100.0f, 100.0f });
	ThreadTestActor->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 100.0f });
	TestRenderer->GetRenderBaseValueRef().IsNormal = 1;


	//NetworkTestLevel
	PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Mesh\\Nero.fbx");
	PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0010_03_atos.texout.png");
	PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0000_03_atos.texout.png");
	//PushAllLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character", {".fbx"});
	//PushAllLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character", { ".png" });
}

#include "NetworkGUI.h"

void ThreadLoadingLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

	//모든 로딩이 완료된 순간
	if (LoadWorkCount == ExcuteWorkCount)
	{
		GameEngineCore::ChangeLevel(NextLevelName);
		NetworkGUI::GetInst()->PrintLog("Thread Load Done", float4{ 0.f, 1.f, 1.f, 1.f });
		return;
	}

	NetworkGUI::GetInst()->PrintLog("Thread Load Percent : " + std::to_string(LoadingPercent), float4{ 0.f, 1.f, 1.f, 1.f });
	ThreadTestActor->GetTransform()->AddLocalRotation(float4::FORWARD * 360.f * _DeltaTime);
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
	LoadingPercent = 0.f;

	//이 부분 일단은 터뜨릴께요. 나중에 보고 주석처리 할 지 판단하죠
	if (true == CallBacks.empty())
	{
		MsgAssert(NextLevelName + "레벨에 스레드 로딩을 등록해준 리소스가 존재하지 않습니다");
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

