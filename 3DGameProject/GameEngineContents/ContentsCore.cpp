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
#include "ShopLevel.h"
#include "PlayerTestLevel.h"
#include "EnemyTestLevel.h"
#include "PlayUITestLevel.h"
//Map
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
	////
	GameEngineCore::CreateLevel<StageEditLevel>();
	GameEngineCore::CreateLevel<ShopLevel>();
	
	GameEngineCore::ChangeLevel("TestLevel");
}

void ContentsCore::GameEnd() 
{
	FontUpRelease();
}

// ��Ʈ �ε�
void ContentsCore::FontUpload()
{
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Font");

	if (0 == AddFontResourceA(NewDir.GetPlusFileName("DMC5Font.otf").GetFullPath().data()))
	{
		MsgAssert("��Ʈ �ε忡 �����߽��ϴ�.");
		return;
	}
	SendMessage(GameEngineWindow::GetHWnd(), WM_FONTCHANGE, NULL, NULL);
}

// ���ε�� ��Ʈ ������
void ContentsCore::FontUpRelease()
{
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Font");

	if (0 == RemoveFontResourceA(NewDir.GetPlusFileName("DMC5Font.otf").GetFullPath().data()))
	{
		MsgAssert("��Ʈ ������ �����߽��ϴ�.");
		return;
	}

	SendMessage(GameEngineWindow::GetHWnd(), WM_FONTCHANGE, NULL, NULL);
}