#include "PrecompileHeader.h"
#include "GameEngineCore.h"

#include <GameEngineBase/GameEngineNetObject.h>
#include <GameEngineBase\GameEngineDebug.h>
#include <GameEngineBase\GameEngineThread.h>
#include <GameEnginePlatform\GameEngineInput.h>
#include <GameEnginePlatform\GameEngineWindow.h>
#include <GameEnginePlatform\GameEngineSound.h>
#include <GameEnginePlatform\GameEngineInput.h>
#include <GameEngineBase\GameEngineTime.h>

#include "GameEngineDevice.h"
#include "GameEngineVideo.h"
#include "GameEngineGUI.h"
#include "GameEnginePhysics.h"
#include "GameEngineOption.h"
#include "GameEngineScreenShoot.h"

GameEngineThreadJobQueue GameEngineCore::JobQueue;

std::map<std::string, std::shared_ptr<GameEngineLevel>> GameEngineCore::LevelMap;
std::shared_ptr<GameEngineLevel> GameEngineCore::MainLevel = nullptr;
std::shared_ptr<GameEngineLevel> GameEngineCore::NextLevel = nullptr;

std::shared_ptr<class GameEngineLevel> GameEngineCore::CurLoadLevel;
float GameEngineCore::FrameTime = 0.0f;
float GameEngineCore::FrameRate = 0.0f;

GameEngineCore::GameEngineCore()
{
}

GameEngineCore::~GameEngineCore()
{
}

void GameEngineCore::Release()
{
	NextLevel = nullptr;
	MainLevel = nullptr;
	CurLoadLevel = nullptr;
	LevelMap.clear();
}

void GameEngineCore::Start(HINSTANCE _instance, std::function<void()> _Start, std::function<void()> _End, float4 _Pos, float4 _Size, bool _IsFullScreen /*= false*/)
{
	GameEngineDebug::LeakCheck();
	GameEngineActor::ScreenRatio.x = _Size.x / 1600;
	GameEngineActor::ScreenRatio.y = _Size.y / 900;
	// 윈도우 창 생성 후 루프문 시작
	GameEngineWindow::WindowCreate(_instance, "DevilMayCry 5", _Size, _Pos, _IsFullScreen);
	GameEngineWindow::WindowLoop(std::bind(GameEngineCore::EngineStart, _Start), GameEngineCore::EngineUpdate, std::bind(GameEngineCore::EngineEnd, _End));
}

void GameEngineCore::EngineStart(std::function<void()> _ContentsStart)
{
	// 기본 키 설정
	{
		if (false == GameEngineInput::IsKey("GUISwitch"))
		{
			GameEngineInput::CreateKey("GUISwitch", VK_F8);
		}
		if (false == GameEngineInput::IsKey("EngineMouseLeft"))
		{
			GameEngineInput::CreateKey("EngineMouseLeft", VK_LBUTTON);
			GameEngineInput::CreateKey("EngineMouseRight", VK_RBUTTON);
		}
		if (false == GameEngineInput::IsKey("ScreenShoot"))
		{
			GameEngineInput::CreateKey("ScreenShoot", VK_OEM_PLUS);
			GameEngineInput::CreateKey("RenderTargetShoot", VK_OEM_MINUS);
		}
	}

	JobQueue.Initialize("EngineJobQueue"); // 스레드 생성
	GameEngineDevice::Initialize();        // 다이렉트 디바이스 생성
	CoreResourcesInit();                   // 다이렉트 리소스 생성
	GameEngineGUI::Initialize();           // Imgui 생성
	GameEnginePhysics::CreatePhysicsX();   // 피직스 이니셜
	CoreOptionInit();					   // 기본 옵션 생성 

	if (nullptr == _ContentsStart)
	{
		MsgAssert("시작 컨텐츠가 존재하지 않습니다.");
	}

	_ContentsStart();
}

void GameEngineCore::LevelInit(std::shared_ptr<GameEngineLevel> _Level)
{
	CurLoadLevel = _Level;
	_Level->Level = _Level.get();
	_Level->Start();
	CurLoadLevel = nullptr;
}

void GameEngineCore::LevelSetName(std::shared_ptr<GameEngineLevel> Level, std::string _Name)
{
	Level->SetName(_Name);
}

void GameEngineCore::ChangeLevel(const std::string_view& _Name)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);

	if (LevelMap.end() == LevelMap.find(UpperName))
	{
		MsgAssert("존재하지 않는 레벨로 체인지 하려고 했습니다.");
		return;
	}

	NextLevel = LevelMap[UpperName];
}

bool GameEngineCore::IsLevel(const std::string_view& _Name)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);

	return LevelMap.contains(UpperName);
}

void GameEngineCore::EngineUpdate()
{
	if (nullptr != NextLevel)
	{
		std::shared_ptr<GameEngineLevel> PrevLevel = MainLevel;

		if (nullptr != MainLevel)
		{
			MainLevel->ActorLevelChangeEnd();
			MainLevel->LevelChangeEnd();
			MainLevel->PostProcessLevelChangeEnd();
			MainLevel->RenderTargetTextureRelease();
		}

		MainLevel = NextLevel;

		if (nullptr != MainLevel)
		{
			CurLoadLevel = MainLevel;
			MainLevel->RenderTargetTextureCreate();
			MainLevel->PostProcessLevelChangeStart();
			GameEnginePhysics::ChangeScene(MainLevel->GetName());
 			MainLevel->LevelChangeStart();
			MainLevel->ActorLevelChangeStart();
		}

		NextLevel = nullptr;
		GameEngineTime::GlobalTime.Reset();
	}

	if (nullptr == MainLevel)
	{
		MsgAssert("아무런 레벨도 동작하지 않고 있습니다.");
		return;
	}

	float TimeDeltaTime = GameEngineTime::GlobalTime.TimeCheck();

	if (TimeDeltaTime > 1 / 30.0f)
	{
		TimeDeltaTime = 1 / 30.0f;
	}
	
	FrameTime += TimeDeltaTime;
	float FrameLimit = 1.0f / 60.0f;

	if (FrameLimit > FrameTime)
	{
		return;
	}
	else
	{
		TimeDeltaTime = FrameTime;
		FrameTime = 0.0f;
		FrameRate = 1.0f / TimeDeltaTime;
	}
	TimeDeltaTime *= GameEngineTime::GlobalTime.GetGlobalTimeScale();
	GameEngineInput::Update(TimeDeltaTime);
	GameEngineSound::SoundUpdate();

	MainLevel->TimeEvent.Update(TimeDeltaTime);
	MainLevel->AccLiveTime(TimeDeltaTime);
	GameEnginePhysics::Simulate(TimeDeltaTime);
	MainLevel->Update(TimeDeltaTime);
	MainLevel->ActorUpdate(TimeDeltaTime);
	MainLevel->NetworkUpdate(TimeDeltaTime);

	GameEngineVideo::VideoState State = GameEngineVideo::GetCurState();

	if (State != GameEngineVideo::VideoState::Running)
	{
		GameEngineDevice::RenderStart();
		MainLevel->Render(TimeDeltaTime);
		GameEngineDevice::RenderEnd();
	}

	if (true == GameEngineInput::IsDown("ScreenShoot"))
	{
		GameEngineScreenShoot::ScreenShoot();
	}
	if (true == GameEngineInput::IsDown("RenderTargetShoot"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Texture", "ScreenShot"
			});

		GameEngineScreenShoot::RenderTargetShoot(MainLevel->GetScreenShootTarget(), Path, "RenderTargetShoot.DDS");
	}

	MainLevel->LightRelease();
	MainLevel->ActorRelease();
}

void GameEngineCore::EngineEnd(std::function<void()> _ContentsEnd)
{
	if (nullptr == _ContentsEnd)
	{
		MsgAssert("끝 컨텐츠가 존재하지 않습니다.");
	}

	_ContentsEnd();

	GameEngineGUI::Release();

	GameEnginePhysics::ReleasePhysicsX();
	LevelMap.clear();
	CoreResourcesEnd();
	Release();

	GameEngineDevice::Release();
	GameEngineWindow::Release();
}

