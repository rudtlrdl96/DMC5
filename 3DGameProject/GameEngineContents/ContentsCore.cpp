#include "PrecompileHeader.h"
#include "ContentsCore.h"

#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

#include "PlayerTestLevel.h"
#include "ShaderTestLevel.h"
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
#include "PlayerTestLevel.h"
#include "EnemyTestLevel.h"
#include "PlayUITestLevel.h"
//Map
#include "TestStageLevel.h"
#include "StageEditLevel.h"
#include "MonsterTestLevel.h"
#include "AnimationToolLevel.h"
#include "EffectToolLevel.h"
#include "PhysXTestLevel.h"

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
	GameEngineCore::CreateLevel<PlayerTestLevel>();
	GameEngineCore::CreateLevel<EnemyTestLevel>();
	GameEngineCore::CreateLevel<AnimationToolLevel>();
	GameEngineCore::CreateLevel<TestStageLevel>();
	GameEngineCore::CreateLevel<ShaderTestLevel>();
	GameEngineCore::CreateLevel<NetworkTestLevel>();
	GameEngineCore::CreateLevel<MonsterTestLevel>();
	GameEngineCore::CreateLevel<PlayUITestLevel>();
	GameEngineCore::CreateLevel<PhysXTestLevel>();
	GameEngineCore::CreateLevel<ThreadLoadingLevel>();
	////
	GameEngineCore::CreateLevel<StageEditLevel>();
	GameEngineCore::CreateLevel<ShopLevel>();
	GameEngineCore::CreateLevel<EffectToolLevel>();
	
	GameEngineCore::ChangeLevel("MainLevel");
}

void ContentsCore::GameEnd() 
{
	FontUpRelease();
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