#include "PrecompileHeader.h"
#include "GameEngineCore.h"

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

// App 시작 시 최초 진입 함수, functional을 통해 binding된 함수 호출
void GameEngineCore::Start(HINSTANCE _instance, std::function<void()> _Start, std::function<void()> _End, float4 _Pos, float4 _Size)
{
	// GameEngineBase의 GameEngineDebug에서 LeakCheck 함수 호출
	GameEngineDebug::LeakCheck();

	// Window 생성
	GameEngineWindow::WindowCreate(_instance, "DirectX3D", _Size, _Pos);
	GameEngineWindow::WindowLoop(std::bind(GameEngineCore::EngineStart, _Start), GameEngineCore::EngineUpdate, std::bind(GameEngineCore::EngineEnd, _End));
}

// 윈도우 생성 후 Direct, GUI 최초 설정하는 위치
void GameEngineCore::EngineStart(std::function<void()> _ContentsStart)
{
	// 기본 키설정 선언
	{
		if (false == GameEngineInput::IsKey("EngineMouseLeft"))
		{
			GameEngineInput::CreateKey("EngineMouseLeft", VK_LBUTTON);
			GameEngineInput::CreateKey("EngineMouseRight", VK_RBUTTON);
		}

		if (false == GameEngineInput::IsKey("GUISwitch"))
		{
			GameEngineInput::CreateKey("GUISwitch", VK_F8);
		}
	}

	JobQueue.Initialize("EngineJobQueue"); // CPU 갯수에 맞춰 스레드 생성
	GameEngineDevice::Initialize();        // DirectX 11 설정
	CoreResourcesInit();                   // Direct 리소스 설정
	GameEngineGUI::Initialize();           // IMGUI 설정
	//GameEnginePhysics::Initialize();      

	if (nullptr == _ContentsStart)
	{
		MsgAssert("시작 컨텐츠가 존재하지 않습니다.");
	}

	_ContentsStart();
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

		// PrevLevel
		// 레벨체인지가 완료된 시점에서 Texture의 상태를 한번 생각해봅시다.

		// 1은 가지고 있다.
		// GameEngineResources<GameEngineTexture>가 1개의 레퍼런스 카운트를 가지고 있을 것이다.

		// 이전레벨에 존재하는 TextureSetter내부에 보관되고 있는 애들은 2이상의 가지고 있을 것이다.

		// 3이상인 애들은 => 이전레벨과 지금레벨에서 모두 사용하는 
		// 애들 TextureResources에서도 들고 있을것이기 때문에 레퍼런스 카운트가 3이상이다.
		// 2인애들은 이전레벨에서만 사용하거나 지금레벨에서만 사용애들입니다.
		// 레퍼런스 카운트 관리해볼것이다.

		// Prev레벨에서 사용한 텍스처들
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

	GameEngineDevice::Release();
	GameEngineWindow::Release();
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

void GameEngineCore::LevelInit(std::shared_ptr<GameEngineLevel> _Level) 
{
	CurLoadLevel = _Level;
	_Level->Level = _Level.get();
	_Level->Start();
	CurLoadLevel = nullptr;
}

