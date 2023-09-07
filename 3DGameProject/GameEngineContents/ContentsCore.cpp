#include "PrecompileHeader.h"
#include "ContentsCore.h"

#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

#include "PlayerTestLevel.h"
#include "ShaderTestLevel.h"
#include "NetworkTestLevel.h"

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
#include "Nero_ShopLevel.h"
//Map
//#include "BossStageLevel.h"
#include "StartStageLevel.h"
#include "TestStageLevel.h"
#include "StageEditLevel.h"
#include "MonsterTestLevel.h"
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

	SetOption();
	ContentsResourcesCreate();
	CreateContentsKey();

	GameEngineCore::CreateLevel<MainLevel>();
	//GameEngineCore::CreateLevel<TestLevel>();
	//GameEngineCore::CreateLevel<IntroLevel>();
	//GameEngineCore::CreateLevel<TitleLevel>();
	//GameEngineCore::CreateLevel<Char_ChoiceLevel>();
	//GameEngineCore::CreateLevel<MainMenuLevel>();
	GameEngineCore::CreateLevel<PlayerTestLevel>();
	//GameEngineCore::CreateLevel<EnemyTestLevel>();
	//GameEngineCore::CreateLevel<AnimationToolLevel>();
	//GameEngineCore::CreateLevel<BossStageLevel>();
	//GameEngineCore::CreateLevel<TestStageLevel>();
	//GameEngineCore::CreateLevel<StartStageLevel>();
	//GameEngineCore::CreateLevel<ShaderTestLevel>();
	//GameEngineCore::CreateLevel<NetworkTestLevel>();
	//GameEngineCore::CreateLevel<MonsterTestLevel>();
	//GameEngineCore::CreateLevel<PlayUITestLevel>();
	//GameEngineCore::CreateLevel<Nero_ShopLevel>();
	//
	//GameEngineCore::CreateLevel<StageEditLevel>();

	GameEngineCore::ChangeLevel("PlayerTestLevel");
}

void ContentsCore::GameEnd() 
{
	// FontUpRelease();
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
	NewDir.MoveParentToDirectory("CupHead_Resource");
	NewDir.Move("CupHead_Resource");

	if (0 == RemoveFontResourceA(NewDir.GetPlusFileName("CupheadFelix-Regular-merged.ttf").GetFullPath().data()))
	{
		MsgAssert("폰트 삭제에 실패했습니다.");
		return;
	}

	SendMessage(GameEngineWindow::GetHWnd(), WM_FONTCHANGE, NULL, NULL);
}