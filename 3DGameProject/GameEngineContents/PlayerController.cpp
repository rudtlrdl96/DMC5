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
	if (false == GameEngineInput::IsKey("Player_Up"))
	{
		GameEngineInput::CreateKey("Player_Up", 'W');
		GameEngineInput::CreateKey("Player_Left", 'A');
		GameEngineInput::CreateKey("Player_Down", 'S');
		GameEngineInput::CreateKey("Player_Right", 'D');
		GameEngineInput::CreateKey("Player_Sword", 'I');
		GameEngineInput::CreateKey("Player_Gun", 'J');
		GameEngineInput::CreateKey("Player_Jump", 'K');
		GameEngineInput::CreateKey("Player_Skill", 'L');
		GameEngineInput::CreateKey("Player_Exceed", 'Q');
		GameEngineInput::CreateKey("Player_LockOn", VK_SPACE);
		GameEngineInput::CreateKey("Player_DT", VK_LSHIFT);
		GameEngineInput::CreateKey("Player_CameraLeft", 'U');
		GameEngineInput::CreateKey("Player_CameraRight", 'O');
		GameEngineInput::CreateKey("Player_CameraUp", 'Y');
		GameEngineInput::CreateKey("Player_CameraDown", 'P');
	}
}

void PlayerController::Update(float _DeltaTime)
{
	MoveAxis = float4::ZERO;

	if (GameEngineInput::IsPress("Player_Left"))
	{
		MoveAxis += CameraTransform->GetWorldLeftVector();
	}
	if (GameEngineInput::IsPress("Player_Right"))
	{
		MoveAxis += CameraTransform->GetWorldRightVector();
	}
	if (GameEngineInput::IsPress("Player_Up"))
	{
		MoveAxis += CameraTransform->GetWorldForwardVector();
	}
	if (GameEngineInput::IsPress("Player_Down"))
	{
		MoveAxis += CameraTransform->GetWorldBackVector();
	}
	MoveAxis.Normalize();

}

