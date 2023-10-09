#include "PrecompileHeader.h"
#include <commdlg.h>
#include <fstream>
#include "PlayerWindow.h"
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include "PlayerCamera.h"
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
	static float Power;
	static float Time;
	static float FadeIn;
	static float FadeOut;

	ImGui::InputFloat("Power", &Power);
	ImGui::InputFloat("Time", &Time);
	ImGui::InputFloat("FadeIn", &FadeIn);
	ImGui::InputFloat("FadeOut", &FadeOut);
	if (ImGui::Button("Shake"))
	{
		PlayerCamera::Shake(Power, Time, FadeIn, FadeOut);
	}


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

