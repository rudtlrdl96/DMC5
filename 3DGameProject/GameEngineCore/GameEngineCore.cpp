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

GameEngineThreadJobQueue GameEngineCore::JobQueue;

std::map<std::string, std::shared_ptr<GameEngineLevel>> GameEngineCore::LevelMap;
std::shared_ptr<GameEngineLevel> GameEngineCore::MainLevel = nullptr;
std::shared_ptr<GameEngineLevel> GameEngineCore::NextLevel = nullptr;

std::shared_ptr<class GameEngineLevel> GameEngineCore::CurLoadLevel;

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

void GameEngineCore::Start(HINSTANCE _instance, std::function<void()> _Start, std::function<void()> _End, float4 _Pos, float4 _Size)
{
	GameEngineDebug::LeakCheck();

	// 윈도우 창 생성 후 루프문 시작
	GameEngineWindow::WindowCreate(_instance, "DevilMayCry 5", _Size, _Pos);
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
	}

	JobQueue.Initialize("EngineJobQueue"); // 스레드 생성
	GameEngineDevice::Initialize();        // 다이렉트 디바이스 생성
	CoreResourcesInit();                   // 다이렉트 리소스 생성
	GameEngineGUI::Initialize();           // Imgui 생성

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


void GameEngineCore::EngineUpdate()
{
	if (nullptr != NextLevel)
	{
		std::shared_ptr<GameEngineLevel> PrevLevel = MainLevel;

		if (nullptr != MainLevel)
		{
			MainLevel->LevelChangeEnd();
			MainLevel->ActorLevelChangeEnd();
		}

		MainLevel = NextLevel;

		if (nullptr != MainLevel)
		{
			CurLoadLevel = MainLevel;
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

	// 별로 좋은건 아닙니다.
	if (TimeDeltaTime > 1 / 30.0f)
	{
		TimeDeltaTime = 1 / 30.0f;
	}

	GameEngineInput::Update(TimeDeltaTime);
	GameEngineSound::SoundUpdate();

	// 업데이트가 일어나는 동안 로드가 된애들

	MainLevel->TimeEvent.Update(TimeDeltaTime);
	MainLevel->AccLiveTime(TimeDeltaTime);
	MainLevel->Update(TimeDeltaTime);
	MainLevel->ActorUpdate(TimeDeltaTime);
	GameEngineNetObject::NetworkObjectPacketUpdate(TimeDeltaTime);
	// CurLoadLevel = nullptr;

	GameEngineVideo::VideoState State = GameEngineVideo::GetCurState();
	if (State != GameEngineVideo::VideoState::Running)
	{
		GameEngineDevice::RenderStart();
		MainLevel->Render(TimeDeltaTime);
		MainLevel->ActorRender(TimeDeltaTime);
		GameEngineDevice::RenderEnd();
	}

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

	LevelMap.clear();
	CoreResourcesEnd();
	Release();

	GameEngineDevice::Release();
	GameEngineWindow::Release();
	// GameEnginePhysics::Release();
}

