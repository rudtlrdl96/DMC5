#include "PrecompileHeader.h"
#include "ThreadLoadingLevel.h"

#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineTexture.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include "NetworkTestLevel.h"
#include "FXData.h"
ThreadLoadingLevel* ThreadLoadingLevel::Inst = nullptr;
std::string ThreadLoadingLevel::NextLevelName;

ThreadLoadingLevel::ThreadLoadingLevel()
{
	Inst = this;
}

ThreadLoadingLevel::~ThreadLoadingLevel()
{
	Inst = nullptr;
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


	//===============================================사용법 & 예시=================================================
	
	// 해당 위치의 지정된 확장자파일들을 모두 로딩합니다
		//PushAllLoadCallBack<어떤 레벨로 넘어갈 때, 어떤 타입의 리소스인지>("위치", { "확장자" });
	// 예시
		//PushAllLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character", { ".png", ".tga" });

	//해당 경로의 리소스를 로딩합니다
		//PushLoadCallBack<어떤 레벨로 넘어갈 때, 어떤 타입의 리소스인지>("경로");	
	// 예시
		//PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Mesh\\Nero.fbx");

	//===========================================================================================================

	// 네로
	{
		PushAllLoadCallBack<NetworkTestLevel, FXData>("Effect\\Nero", { ".effect" });
		PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Mesh\\Nero.fbx");
		PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Overture\\Mesh\\Overture.fbx");
		PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Wire\\Nero_WireArm.fbx");
		PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0000_03_atos.texout.png");
		PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0010_03_atos.texout.png");
		PushAllLoadCallBack<NetworkTestLevel, GameEngineFBXAnimation>("Character\\Player\\Nero\\Animation", { ".fbx" });
		PushAllLoadCallBack<NetworkTestLevel, GameEngineFBXAnimation>("Character\\Player\\Nero\\Overture\\Animation", { ".fbx" });
	}

	// 버질
	{
		PushAllLoadCallBack<NetworkTestLevel, FXData>("Effect\\Vergil", { ".effect" });
		PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Vergil\\Mesh\\Vergil.fbx");
		PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Vergil\\Mesh\\pl0300_03_atos.texout.png");
		PushAllLoadCallBack<NetworkTestLevel, GameEngineFBXAnimation>("Character\\Player\\Vergil\\Animation", { ".fbx" });
	}

	//NetwortTestLevel(레벨별로 구분시켜주세요)
	{
		//PushAllLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Effect\\Mesh", { ".fbx" });
		//PushAllLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Effect\\Texture", { ".png" });
		//PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Mesh\\Nero.fbx");
		//PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0010_03_atos.texout.png");
		//PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0000_03_atos.texout.png");
	}


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
	const std::vector<std::function<void()>>& CallBacks = AllThreadLoadCallBack[NextLevelName];
	LoadWorkCount = CallBacks.size();
	ExcuteWorkCount = 0;
	LoadingPercent = 0.f;

	if (true == CallBacks.empty())
	{
		LoadWorkCount = 1;
		ExcuteWorkCount = 1;
		LoadingPercent = 1.f;
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

