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
#include "ResultLevel.h"

//Map
#include "TestStageLevel.h"
#include "StageEditLevel.h"
#include "Location2_Level.h"
#include "Location11_Level.h"
#include "Location19_Level.h"

#include "AnimationToolLevel.h"
#include "EffectToolLevel.h"
#include "DynamicResourcesLoadTest.h"


std::vector<std::function<void()>> ContentsCore::GameEndCallBacks;

ContentsCore::ContentsCore() 
{
}

ContentsCore::~ContentsCore() 
{
}

void ContentsCore::GameStart() 
{
	SetOption();
	ContentsResourcesCreate();
	CreateContentsKey();
	EffectTextureLoad();

	GameEngineCore::CreateLevel<MainLevel>();
	GameEngineCore::CreateLevel<TestLevel>();
	GameEngineCore::CreateLevel<IntroLevel>();
	GameEngineCore::CreateLevel<TitleLevel>();
	GameEngineCore::CreateLevel<Char_ChoiceLevel>();
	GameEngineCore::CreateLevel<MainMenuLevel>();
	GameEngineCore::CreateLevel<AnimationToolLevel>();
	GameEngineCore::CreateLevel<TestStageLevel>();
	GameEngineCore::CreateLevel<Location2_Level>();
	GameEngineCore::CreateLevel<Location11_Level>();
	GameEngineCore::CreateLevel<Location19_Level>();
	GameEngineCore::CreateLevel<NetworkTestLevel>();
	GameEngineCore::CreateLevel<PlayUITestLevel>();
	GameEngineCore::CreateLevel<ThreadLoadingLevel>();
	//GameEngineCore::CreateLevel<DynamicResourcesLoadTest>();
	////
	GameEngineCore::CreateLevel<StageEditLevel>();
	GameEngineCore::CreateLevel<ShopLevel>();
	GameEngineCore::CreateLevel<EffectToolLevel>();
	GameEngineCore::CreateLevel<ResultLevel>();
	
	GameEngineCore::ChangeLevel("IntroLevel");
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