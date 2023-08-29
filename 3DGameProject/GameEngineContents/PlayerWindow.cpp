#include "PrecompileHeader.h"
#include <commdlg.h>
#include <fstream>
#include "PlayerWindow.h"
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineRenderer.h>

PlayerActor_Nero* PlayerWindow::_Nero = nullptr;

PlayerWindow::PlayerWindow()
{
}

PlayerWindow::~PlayerWindow()
{
}

void PlayerWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	if (_Nero == nullptr) { return; }

	static float Mass = 50.0f;
	static float Jump = 50.0f;

	ImGui::InputFloat("Mass", &Mass);
	ImGui::InputFloat("Jump", &Jump);

	if (ImGui::Button("Set"))
	{
		_Nero->SetMass(Mass);
		_Nero->JumpForce = Jump;
	}
}

