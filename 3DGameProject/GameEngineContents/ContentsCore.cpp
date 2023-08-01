#include "PrecompileHeader.h"
#include "ContentsCore.h"

#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

#include "PlayLevel.h"

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

	GameEngineCore::CreateLevel<PlayLevel>();
	GameEngineCore::ChangeLevel("PlayLevel");
}

void ContentsCore::GameEnd() 
{
	//{
	//	GameEngineDirectory NewDir;
	//	NewDir.MoveParentToDirectory("CupHead_Resource");
	//	NewDir.Move("CupHead_Resource");

	//	if (0 == RemoveFontResourceA(NewDir.GetPlusFileName("CupheadFelix-Regular-merged.ttf").GetFullPath().data()))
	//	{
	//		MsgAssert("폰트 삭제에 실패했습니다.");
	//		return;
	//	}

	//	SendMessage(GameEngineWindow::GetHWnd(), WM_FONTCHANGE, NULL, NULL);
	//}
}