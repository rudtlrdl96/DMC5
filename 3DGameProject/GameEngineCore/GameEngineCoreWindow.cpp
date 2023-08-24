#include "PrecompileHeader.h"
#include "GameEngineCoreWindow.h"
#include <GameEngineBase/GameEngineString.h>

GameEngineCoreWindow::GameEngineCoreWindow() 
{
}

GameEngineCoreWindow::~GameEngineCoreWindow() 
{
}


void GameEngineCoreWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	std::string Text = GameEngineString::AnsiToUTF8("한글 잘 나오나요 플레이어 만드는 버튼이에요");
	if (ImGui::Button(Text.c_str()))
	{
		if (nullptr != Test)
		{
			Test();
		}
	}

}