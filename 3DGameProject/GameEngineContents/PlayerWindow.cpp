#include "PrecompileHeader.h"
#include <commdlg.h>
#include <fstream>
#include "PlayerWindow.h"
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineRenderer.h>
PlayerWindow::PlayerWindow()
{
}

PlayerWindow::~PlayerWindow()
{
}

void PlayerWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	if (0 == RenderUnits->size()) { return; }

	static int index;
	ImGui::InputInt("Index", &index);

	if (ImGui::Button("On"))
	{
		RenderUnits->operator[](0).operator[](index)->On();
	}
	if (ImGui::Button("Off"))
	{
		RenderUnits->operator[](0).operator[](index)->Off();
	}
}

