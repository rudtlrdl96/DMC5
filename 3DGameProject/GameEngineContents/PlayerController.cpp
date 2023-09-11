#include "PrecompileHeader.h"
#include "PlayerController.h"
#include "PlayerWindow.h"
#include "BasePlayerActor.h"
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
	float4 ForwardVector;
	if (nullptr != Actor->LockOnEnemyTransform)
	{
		ForwardVector = Actor->LockOnEnemyTransform->GetWorldPosition() - GetTransform()->GetWorldPosition();
		ForwardVector.y = 0;
		ForwardVector.Normalize();
	}
	else
	{
		ForwardVector = PlayerTransform->GetWorldForwardVector();	// �÷��̾� ����
	}
	float4 Cross = float4::Cross3DReturnNormal(ForwardVector, _Value); // ����
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
	float4 ForwardVector;
	if (nullptr != Actor->LockOnEnemyTransform)
	{
		ForwardVector = Actor->LockOnEnemyTransform->GetWorldPosition() - GetTransform()->GetWorldPosition();
		ForwardVector.y = 0;
		ForwardVector.Normalize();
	}
	else
	{
		ForwardVector = PlayerTransform->GetWorldForwardVector();	// �÷��̾� ����
	}
	float4 Cross = float4::Cross3DReturnNormal(ForwardVector, _Value); // ����
	float Dot = float4::DotProduct3D(ForwardVector, _Value);

	if (0 < Cross.y)
	{
		if (Dot < -0.5f)
		{
			return '2';
		}
		else if (Dot < 0.5f)
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
		if (Dot < -0.5f)
		{
			return '2';
		}
		else if (Dot < 0.5f)
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
	Actor = dynamic_cast<BasePlayerActor*>(GetActor());
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
	// ��
	IsBackFrontSword = false;
	IsFrontSword = false;
	IsBackSword = false;
	IsSword = false;
	// ����
	IsLeftJump = false;
	IsRightJump = false;
	IsJump = false;
	// ��
	IsBackFrontGun = false;
	IsFrontGun = false;
	IsBackGun = false;
	// ��ų
	IsBackFrontSkill = false;
	IsFrontSkill = false;
	IsSkill = false;

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
	Command.TimeCheck(GetLiveTime());

	// �̵� �Է� ������ ����Ѵ�
	char MoveChar = MoveVectorToChar(MoveVector);
	Command.AddKey(MoveVectorToChar(MoveVector), GetLiveTime());
}

void PlayerController::ActionInput()
{
	// Sword
	if (GameEngineInput::IsDown("Player_Sword"))
	{
		if (true == InputCheck_BackFront())
		{
			IsBackFrontSword = true;
		}
		else if (true == InputCheck_Dir('8'))
		{
			if (GameEngineInput::IsPress("Player_Skill") && GameEngineInput::GetPressTime("Player_Skill") < 0.05f)
			{
				IsSpecialMove = true;
			}
			else
			{
				IsFrontSword = true;
			}
		}
		else if (true == InputCheck_Dir('2'))
		{
			IsBackSword = true;
		}
		else
		{
			IsSword = true;
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
		if (IsLockOn == false)
		{
			IsSkill = true;
		}
		else if (true == InputCheck_Dir('8'))
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
	if (Command.Keys[0] != '8') { return false; }		// Ŀ�ǵ尡 8�� �ƴ� �� false

	if (Command.Keys[1] == '2')	// 8���� Ŀ�ǵ尡 2�Ͻ�
	{
		// 2�� �Է��ߴ� ���
		return true;
	}
	else if (Command.Keys[1] == 'n')
	{
		// �߸��̾��� ���
		return Command.Keys[2] == '2';	// 2���ٸ� true �ƴϸ� false
	}
	return false;
}


bool PlayerController::InputCheck_Dir(char _Dir)
{
	return Command.Keys[0] == _Dir;	// Ŀ�ǵ尡 _Dir�̸� true
}
