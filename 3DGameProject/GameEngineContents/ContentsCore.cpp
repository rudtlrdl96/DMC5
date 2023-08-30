#include "PrecompileHeader.h"
#include "ContentsCore.h"

#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

#include "PlayerTestLevel.h"
#include "ShaderTestLevel.h"

//UI
#include "MainLevel.h"
#include "TestLevel.h"
#include "IntroLevel.h"
#include "TitleLevel.h"
#include "Char_ChoiceLevel.h"
#include "MainMenuLevel.h"
#include "PlayerTestLevel.h"
#include "EnemyTestLevel.h"
#include "PlayUITestLevel.h"
//Map
#include "BossStageLevel.h"
#include "StartStageLevel.h"
#include "StageEditLevel.h"

#include "AnimationToolLevel.h"
ContentsCore::ContentsCore() 
{
}

ContentsCore::~ContentsCore() 
{
}

void ContentsCore::GameStart() 
{
	new int();

	ContentsResourcesCreate();

	GameEngineCore::CreateLevel<MainLevel>();
	GameEngineCore::CreateLevel<TestLevel>();
	GameEngineCore::CreateLevel<IntroLevel>();
	GameEngineCore::CreateLevel<TitleLevel>();
	GameEngineCore::CreateLevel<Char_ChoiceLevel>();
	GameEngineCore::CreateLevel<MainMenuLevel>();
	GameEngineCore::CreateLevel<PlayerTestLevel>();
	GameEngineCore::CreateLevel<EnemyTestLevel>();
	GameEngineCore::CreateLevel<AnimationToolLevel>();
	GameEngineCore::CreateLevel<BossStageLevel>();
	GameEngineCore::CreateLevel<StartStageLevel>();
	GameEngineCore::CreateLevel<ShaderTestLevel>();
	GameEngineCore::CreateLevel<PlayUITestLevel>();
	//
	GameEngineCore::CreateLevel<StageEditLevel>();

	GameEngineCore::ChangeLevel("MainLevel");
}

void ContentsCore::GameEnd() 
{
	// FontUpRelease();
}

// 폰트 로드
void ContentsCore::FontUpload()
{
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("CupHead_Resource");
	NewDir.Move("CupHead_Resource");

	if (0 == AddFontResourceA(NewDir.GetPlusFileName("CupheadFelix-Regular-merged.ttf").GetFullPath().data()))
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
	NewDir.MoveParentToDirectory("CupHead_Resource");
	NewDir.Move("CupHead_Resource");

	if (0 == RemoveFontResourceA(NewDir.GetPlusFileName("CupheadFelix-Regular-merged.ttf").GetFullPath().data()))
	{
		MsgAssert("폰트 삭제에 실패했습니다.");
		return;
	}

	SendMessage(GameEngineWindow::GetHWnd(), WM_FONTCHANGE, NULL, NULL);
}