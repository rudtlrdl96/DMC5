#include "PrecompileHeader.h"
#include <commdlg.h>
#include <fstream>
#include "PlayerWindow.h"
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineRenderer.h>

PlayerActor_Nero* PlayerWindow::_Nero = nullptr;
GameEngineFBXRenderer* PlayerWindow::Renderer = nullptr;
std::function<void(float)> PlayerWindow::Function1 = nullptr;

PlayerWindow::PlayerWindow()
{
}

PlayerWindow::~PlayerWindow()
{
}

void PlayerWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	static int Index = 0;
	if (nullptr != Renderer)
	{
		ImGui::InputInt("Index", &Index);
		if (ImGui::Button("On"))
		{
			Renderer->GetAllRenderUnit()[0][Index]->On();
		}
		if (ImGui::Button("Off"))
		{
			Renderer->GetAllRenderUnit()[0][Index]->Off();
		}
	}
	if (_Nero == nullptr) { return; }
}

