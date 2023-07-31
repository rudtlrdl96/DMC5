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

void GameEngineCore::EngineStart(std::function<void()> _ContentsStart)
{
	// �ھ��̴ϼȶ�����
	// Rect Box

	if (false == GameEngineInput::IsKey("GUISwitch"))
	{
		GameEngineInput::CreateKey("GUISwitch", VK_F8);
	}
	

	JobQueue.Initialize("EngineJobQueue");

	GameEngineDevice::Initialize();

	CoreResourcesInit();

	GameEngineGUI::Initialize();

	if (nullptr == _ContentsStart)
	{
		MsgAssert("���� �������� �������� �ʽ��ϴ�.");
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
		// ����ü������ �Ϸ�� �������� Texture�� ���¸� �ѹ� �����غ��ô�.

		// 1�� ������ �ִ�.
		// GameEngineResources<GameEngineTexture>�� 1���� ���۷��� ī��Ʈ�� ������ ���� ���̴�.

		// ���������� �����ϴ� TextureSetter���ο� �����ǰ� �ִ� �ֵ��� 2�̻��� ������ ���� ���̴�.

		// 3�̻��� �ֵ��� => ���������� ���ݷ������� ��� ����ϴ� 
		// �ֵ� TextureResources������ ��� �������̱� ������ ���۷��� ī��Ʈ�� 3�̻��̴�.
		// 2�ξֵ��� �������������� ����ϰų� ���ݷ��������� ���ֵ��Դϴ�.
		// ���۷��� ī��Ʈ �����غ����̴�.

		// Prev�������� ����� �ؽ�ó��
		NextLevel = nullptr;
		GameEngineTime::GlobalTime.Reset();
	}

	if (nullptr == MainLevel)
	{
		MsgAssert("�ƹ��� ������ �������� �ʰ� �ֽ��ϴ�.");
		return;
	}

	float TimeDeltaTime = GameEngineTime::GlobalTime.TimeCheck();

	// ���� ������ �ƴմϴ�.
	if (TimeDeltaTime > 1 / 30.0f)
	{
		TimeDeltaTime = 1 / 30.0f;
	}

	GameEngineInput::Update(TimeDeltaTime);
	GameEngineSound::SoundUpdate();

	// ������Ʈ�� �Ͼ�� ���� �ε尡 �Ⱦֵ�

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
		MsgAssert("�� �������� �������� �ʽ��ϴ�.");
	}

	_ContentsEnd();

	GameEngineGUI::Release();

	LevelMap.clear();
	CoreResourcesEnd();

	GameEngineDevice::Release();
	GameEngineWindow::Release();
}

void GameEngineCore::Start(HINSTANCE _instance,  std::function<void()> _Start, std::function<void()> _End, float4 _Pos, float4 _Size)
{
	GameEngineDebug::LeakCheck();

	if (false == GameEngineInput::IsKey("EngineMouseLeft"))
	{
		GameEngineInput::CreateKey("EngineMouseLeft", VK_LBUTTON);
		GameEngineInput::CreateKey("EngineMouseRight", VK_RBUTTON);
	}

	GameEngineWindow::WindowCreate(_instance, "MainWindow", _Size, _Pos);
	GameEngineWindow::WindowLoop(std::bind(GameEngineCore::EngineStart, _Start), GameEngineCore::EngineUpdate, std::bind(GameEngineCore::EngineEnd, _End));
}

void GameEngineCore::ChangeLevel(const std::string_view& _Name) 
{
	std::string UpperName = GameEngineString::ToUpper(_Name);

	if (LevelMap.end() == LevelMap.find(UpperName))
	{
		MsgAssert("�������� �ʴ� ������ ü���� �Ϸ��� �߽��ϴ�.");
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

