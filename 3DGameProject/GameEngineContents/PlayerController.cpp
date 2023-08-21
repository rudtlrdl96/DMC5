#include "PrecompileHeader.h"
#include "PlayerController.h"

PlayerController::PlayerController() 
{
}

PlayerController::~PlayerController() 
{
}

void PlayerController::Start()
{
	if (false == GameEngineInput::IsKey("Up"))
	{
		GameEngineInput::CreateKey("Up", 'W');
		GameEngineInput::CreateKey("Left", 'A');
		GameEngineInput::CreateKey("Down", 'S');
		GameEngineInput::CreateKey("Right", 'D');
		GameEngineInput::CreateKey("Sword", 'I');
		GameEngineInput::CreateKey("Gun", 'J');
		GameEngineInput::CreateKey("Jump", 'K');
		GameEngineInput::CreateKey("Skill", 'L');
		GameEngineInput::CreateKey("Exceed", 'Q');
		GameEngineInput::CreateKey("LockOn", VK_SPACE);
		GameEngineInput::CreateKey("DT", VK_LSHIFT);
		GameEngineInput::CreateKey("CameraLeft", 'U');
		GameEngineInput::CreateKey("CameraRight", 'O');
		GameEngineInput::CreateKey("CameraUp", 'Y');
		GameEngineInput::CreateKey("CameraDown", 'P');
	}
}

void PlayerController::Update(float _DeltaTime)
{
	MoveAxis = float4::ZERO;

	if (GameEngineInput::IsPress("Left"))
	{
		MoveAxis += CameraTransform->GetWorldLeftVector();
	}
	if (GameEngineInput::IsPress("Right"))
	{
		MoveAxis += CameraTransform->GetWorldRightVector();
	}
	if (GameEngineInput::IsPress("Up"))
	{
		MoveAxis += CameraTransform->GetWorldForwardVector();
	}
	if (GameEngineInput::IsPress("Down"))
	{
		MoveAxis += CameraTransform->GetWorldBackVector();
	}
	MoveAxis.Normalize();

}

