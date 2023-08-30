#include "PrecompileHeader.h"
#include <commdlg.h>
#include <fstream>
#include "PlayerWindow.h"
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineRenderer.h>

PlayerActor_Nero* PlayerWindow::_Nero = nullptr;
std::function<void(float)> PlayerWindow::Function1 = nullptr;

PlayerWindow::PlayerWindow()
{
}

PlayerWindow::~PlayerWindow()
{
}

void PlayerWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	if (_Nero == nullptr) { return; }

	static float Gravity = 100.0f;
	static float Jump = 50.0f;
	static float Mass = 50.0f;

	ImGui::InputFloat("Gravity", &Gravity);
	ImGui::InputFloat("Jump", &Jump);
	ImGui::InputFloat("Mass", &Mass);

	if (ImGui::Button("Set"))
	{
		Level->SetLevelSceneGravity(Gravity);
		_Nero->JumpForce = Jump;
		_Nero->SetMass(Mass);
	}
	

	if (Function1 == nullptr) { return; }

	static float DynamicFriction = 50.0f;
	ImGui::InputFloat("DynamicFriction", &DynamicFriction);
	if (ImGui::Button("Set1"))
	{
		Function1(DynamicFriction);
	}
}

