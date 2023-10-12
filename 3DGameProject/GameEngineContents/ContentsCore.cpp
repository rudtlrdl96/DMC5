#include "PrecompileHeader.h"
#include "ContentsCore.h"

#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

#include "NetworkTestLevel.h"
#include "ThreadLoadingLevel.h"

//UI
#include "MainLevel.h"
#include "TestLevel.h"
#include "IntroLevel.h"
#include "TitleLevel.h"
#include "Char_ChoiceLevel.h"
#include "MainMenuLevel.h"
#include "ShopLevel.h"
#include "PlayUITestLevel.h"
//Map
#include "TestStageLevel.h"
#include "StageEditLevel.h"
#include "Location11_Level.h"

#include "AnimationToolLevel.h"
#include "EffectToolLevel.h"


std::vector<std::function<void()>> ContentsCore::GameEndCallBacks;

ContentsCore::ContentsCore() 
{
}

ContentsCore::~ContentsCore() 
{
}

void ContentsCore::GameStart() 
{
	new int();

	SetOption();
	ContentsResourcesCreate();
	CreateContentsKey();

	GameEngineCore::CreateLevel<MainLevel>();
	GameEngineCore::CreateLevel<TestLevel>();
	GameEngineCore::CreateLevel<IntroLevel>();
	GameEngineCore::CreateLevel<TitleLevel>();
	GameEngineCore::CreateLevel<Char_ChoiceLevel>();
	GameEngineCore::CreateLevel<MainMenuLevel>();
	GameEngineCore::CreateLevel<AnimationToolLevel>();
	GameEngineCore::CreateLevel<TestStageLevel>();
	GameEngineCore::CreateLevel<Location11_Level>();
	GameEngineCore::CreateLevel<NetworkTestLevel>();
	GameEngineCore::CreateLevel<PlayUITestLevel>();
	GameEngineCore::CreateLevel<ThreadLoadingLevel>();

	GameEngineCore::CreateLevel<StageEditLevel>();
	GameEngineCore::CreateLevel<ShopLevel>();
	GameEngineCore::CreateLevel<EffectToolLevel>();
	
	GameEngineCore::ChangeLevel("MainLevel");
}

void ContentsCore::GameEnd() 
{
	FontUpRelease();

	for (std::function<void()> CallBack : GameEndCallBacks)
	{
		CallBack();
	}
	GameEndCallBacks.clear();
}

// 폰트 로드
void ContentsCore::FontUpload()
{
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Font");

	if (0 == AddFontResourceA(NewDir.GetPlusFileName("DMC5Font.otf").GetFullPath().data()))
	{
		MsgAssert("폰트 로드에 실패했습니다.");
		return;
	}
	SendMessage(GameEngineWindow::GetHWnd(), WM_FONTCHANGE, NULL, NULL);
}

// 업로드된 폰트 릴리즈
void ContentsCore::FontUpRelease()
{
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Font");

	if (0 == RemoveFontResourceA(NewDir.GetPlusFileName("DMC5Font.otf").GetFullPath().data()))
	{
		MsgAssert("폰트 삭제에 실패했습니다.");
		return;
	}

	SendMessage(GameEngineWindow::GetHWnd(), WM_FONTCHANGE, NULL, NULL);
}