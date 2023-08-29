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

	static float4 PushForce = float4::ZERO;
	static float MoveSpeed = 1500000.0f;

	ImGui::InputFloat4("PushForce", PushForce.Arr1D);

}

