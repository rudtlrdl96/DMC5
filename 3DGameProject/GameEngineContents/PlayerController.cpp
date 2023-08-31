#include "PrecompileHeader.h"
#include "PlayerController.h"
#include "PlayerWindow.h"

PlayerController::PlayerController() 
{
}

PlayerController::~PlayerController() 
{
}

char PlayerController::MoveVectorToChar(const float4& _Value)
{
	if (_Value == float4::ZERO)
	{
		return 'n';
	}
	float4 ForwardVector = PlayerTransform->GetWorldForwardVector();	// 카메라상 정면
	float4 Cross = float4::Cross3DReturnNormal(ForwardVector, _Value); // 외적
	float Dot = float4::DotProduct3D(ForwardVector, _Value);

	if (0 < Cross.y)
	{
		if (Dot < -0.75f)
		{
			return '2';
		}
		else if (Dot < -0.25f)
		{
			return '3';
		}
		else if (Dot < 0.25f)
		{
			return '6';
		}
		else if (Dot < 0.75f)
		{
			return '9';
		}
		else
		{
			return '8';
		}
	}
	else
	{
		if (Dot < -0.75f)
		{
			return '2';
		}
		else if (Dot < -0.25f)
		{
			return '1';
		}
		else if (Dot < 0.25f)
		{
			return '4';
		}
		else if (Dot < 0.75f)
		{
			return '7';
		}
		else
		{
			return '8';
		}
	}

	return 0;
}

char PlayerController::MoveVectorToChar4(const float4& _Value)
{
	if (_Value == float4::ZERO)
	{
		return 'n';
	}
	float4 ForwardVector = PlayerTransform->GetWorldForwardVector();	// 카메라상 정면
	float4 Cross = float4::Cross3DReturnNormal(ForwardVector, _Value); // 외적
	float Dot = float4::DotProduct3D(ForwardVector, _Value);

	if (0 < Cross.y)
	{
		if (Dot < 0.25f)
		{
			return '2';
		}
		else if (Dot < 0.75f)
		{
			return '6';
		}
		else
		{
			return '8';
		}
	}
	else
	{
		if (Dot < 0.25f)
		{
			return '2';
		}
		else if (Dot < 0.75f)
		{
			return '4';
		}
		else
		{
			return '8';
		}
	}

	return 0;
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
	PlayerTransform = GetActor()->GetTransform();
}

void PlayerController::Update(float _DeltaTime)
{
	InputReset();
	MoveInput();
	InputRecord();
	ActionInput();
}

void PlayerController::InputReset()
{
	// 검
	 IsBackFrontSword = false;
	 IsFrontSword = false;
	 IsBackSword = false;
	// 점프
	IsLeftJump = false;
	IsRightJump = false;
	IsJump = false;
	// 총
	IsBackFrontGun = false;
	IsFrontGun = false;
	IsBackGun = false;
	// 스킬
	IsBackFrontSkill = false;
	IsFrontSkill = false;

	IsSpecialMove = false;
}

void PlayerController::MoveInput()
{
	MoveVector = float4::ZERO;

	if (GameEngineInput::IsPress("Player_Left"))
	{
		MoveVector += CameraTransform->GetWorldLeftVector();
	}
	if (GameEngineInput::IsPress("Player_Right"))
	{
		MoveVector += CameraTransform->GetWorldRightVector();
	}
	if (GameEngineInput::IsPress("Player_Up"))
	{
		MoveVector += CameraTransform->GetWorldForwardVector();
	}
	if (GameEngineInput::IsPress("Player_Down"))
	{
		MoveVector += CameraTransform->GetWorldBackVector();
	}
	MoveVector.Normalize();
}


void PlayerController::InputRecord()
{
	// 이동 입력 내역을 기록한다
	char MoveChar = MoveVectorToChar(MoveVector);
	Command.TimeCheck(GetLiveTime());

	if (false == IsLockOn) { return; }

	Command.AddKey(MoveVectorToChar(MoveVector), GetLiveTime());
}

void PlayerController::ActionInput()
{
	// Sword
	if (GameEngineInput::IsDown("Player_Sword"))
	{
		if (true == InputCheck_BackFront())
		{
			int a = 0;
		}
		else if (true == InputCheck_Dir('8'))
		{
			if (GameEngineInput::IsPress("Player_Skill") && GameEngineInput::GetPressTime("Player_Skill") < 0.05f)
			{
				IsSpecialMove = true;
			}
			int a = 0;
		}
		else if (true == InputCheck_Dir('2'))
		{
			int a = 0;
		}
	}
	// Jump
	if (GameEngineInput::IsDown("Player_Jump"))
	{
		if (true == InputCheck_Dir('4') || true == InputCheck_Dir('1') || true == InputCheck_Dir('7'))
		{
			IsLeftJump = true;
		}
		else if (true == InputCheck_Dir('6') || true == InputCheck_Dir('3') || true == InputCheck_Dir('9'))
		{
			IsRightJump = true;
		}
		else
		{
			IsJump = true;
		}
	}
	// LockOn
	if (GameEngineInput::IsDown("Player_LockOn"))
	{
		IsLockOn = true;
		if (nullptr != CallBack_LockOnDown)
		{
			CallBack_LockOnDown();
		}
	}
	else if (GameEngineInput::IsUp("Player_LockOn"))
	{
		IsLockOn = false;
		if (nullptr != CallBack_LockOnUp)
		{
			CallBack_LockOnUp();
		}
	}
	// Skill
	if (GameEngineInput::IsDown("Player_Skill"))
	{
		if (true == InputCheck_Dir('8'))
		{
			if (GameEngineInput::IsPress("Player_Sword") && GameEngineInput::GetPressTime("Player_Sword") < 0.05f)
			{
				IsSpecialMove = true;
			}

			int a = 0;
		}
		else if (true == InputCheck_Dir('2'))
		{
			int a = 0;
		}
	}
}

bool PlayerController::InputCheck_BackFront()
{
	if (Command.Keys[0] != '8') { return false; }		// 커맨드가 8이 아닐 시 false

	if (Command.Keys[1] == '2')	// 8이전 커맨드가 2일시
	{
		// 2를 입력했던 경우
		return true;
	}
	else if (Command.Keys[1] == 'n')
	{
		// 중립이었던 경우
		return Command.Keys[2] == '2';	// 2였다면 true 아니면 false
	}
	return false;
}


bool PlayerController::InputCheck_Dir(char _Dir)
{
	return Command.Keys[0] == _Dir;	// 커맨드가 _Dir이면 true
}
