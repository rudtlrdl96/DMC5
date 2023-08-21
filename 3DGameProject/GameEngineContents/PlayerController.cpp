#include "PrecompileHeader.h"
#include "PlayerController.h"

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
	MoveInput();
	InputRecord();
	ActionInput();
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
	
	char MoveChar = MoveVectorToChar(MoveVector);
	if (QCommand.size() > 0)
	{
		if (true == QCommand.front()->TimeCheck(GetLiveTime()))
		{
			delete QCommand.front();
			QCommand.front() = nullptr;
			QCommand.pop();
			if (QCommand.size() == 0)
			{
				return;
			}
		}
		if (MoveChar == QCommand.back()->GetKey())
		{
			return;
		}
	}
	if (QCommand.size() == 0)
	{
		QCommand.push(new CommandRecord(MoveVectorToChar(MoveVector), GetLiveTime(), nullptr));
		return;
	}
	QCommand.push(new CommandRecord(MoveVectorToChar(MoveVector), GetLiveTime(), QCommand.front()));
}

void PlayerController::ActionInput()
{
	if (GameEngineInput::IsPress("Player_Sword"))
	{
		if (true == InputCheck_BackForward())
		{
			int a = 0;
		}
	}

}

bool PlayerController::InputCheck_BackForward()
{
	CommandRecord* BackCommand = QCommand.back();		// 커맨드큐에서 가장마지막 커맨드를 받아온다
	if (BackCommand == nullptr) { return false; }				// 커맨드가 nullptr시 false
	if (BackCommand->GetKey() != '8') { return false; }		// 커맨드가 8이 아닐 시 false

	CommandRecord* SecondCommand = BackCommand->GetFront();	// 8이전의 입력한 커맨드를 받아옴
	if (SecondCommand == nullptr) { return false; }				// 커맨드가 nullptr시 false

	if (SecondCommand->GetKey() == '2')
	{
		// 2를 입력했던 경우
		return true;
	}
	else if (SecondCommand->GetKey() == 'n')
	{
		// 중립이었던 경우
		CommandRecord* ThirdCommand = SecondCommand->GetFront();	// 중립 이전의 입력한 커맨드를 받아옴
		if (ThirdCommand == nullptr) { return false; }				// 커맨드가 nullptr시 false
		return ThirdCommand->GetKey() == '2';	// 2였다면 true 아니면 false
	}
	return false;
}


bool PlayerController::InputCheck_Forward()
{
	return false;
}

bool PlayerController::InputCheck_Back()
{

	return false;
}

bool PlayerController::InputCheck_Left()
{
	return false;

}


bool PlayerController::InputCheck_Right()
{
	return false;

}