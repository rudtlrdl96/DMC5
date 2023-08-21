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
	float4 ForwardVector = PlayerTransform->GetWorldForwardVector();	// ī�޶�� ����
	float4 Cross = float4::Cross3DReturnNormal(ForwardVector, _Value); // ����
	float Dot = float4::DotProduct3D(ForwardVector, _Value);

	if (0 < Cross.y)
	{
		if (Dot < -0.75f)
		{
			return '4';
		}
		else if (Dot < -0.25f)
		{
			return '1';
		}
		else if (Dot < 0.25f)
		{
			return '2';
		}
		else if (Dot < 0.75f)
		{
			return '3';
		}
		else
		{
			return '6';
		}
	}
	else
	{
		if (Dot < -0.75f)
		{
			return '4';
		}
		else if (Dot < -0.25f)
		{
			return '7';
		}
		else if (Dot < 0.25f)
		{
			return '8';
		}
		else if (Dot < 0.75f)
		{
			return '9';
		}
		else
		{
			return '6';
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

	std::shared_ptr<PlayerWindow> Window = GameEngineGUI::FindGUIWindowConvert<PlayerWindow>("PlayerWindow");
	Window->Text = " ";
	if (QCommand.size() == 0)
	{
		return;
	}
	CommandRecord* Back = QCommand.back();
	while (Back != nullptr)
	{
		if (Back == nullptr) { return; }
		Window->Text += Back->GetKey();
		Window->Text += "\n";
		Back = Back->GetFront();
	}
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
	// �̵� �Է� ������ ����Ѵ�
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
			QCommand.front()->FrontClear();
		}
		if (MoveChar == QCommand.back()->GetKey())
		{
			return;
		}
	}

	if (false == IsLockOn) { return; }

	if (QCommand.size() == 0)
	{
		QCommand.push(new CommandRecord(MoveVectorToChar(MoveVector), GetLiveTime(), nullptr));
		return;
	}
	QCommand.push(new CommandRecord(MoveVectorToChar(MoveVector), GetLiveTime(), QCommand.back()));
}

void PlayerController::ActionInput()
{
	// Sword
	if (GameEngineInput::IsDown("Player_Sword"))
	{
		if (true == InputCheck_BackFront())
		{
			if (nullptr != CallBack_BackFrontSword)
			{
				CallBack_BackFrontSword();
			}
		}
		else if (true == InputCheck_Dir('8'))
		{
			if (nullptr != CallBack_FrontSword)
			{
				CallBack_FrontSword();
			}
		}
		else if (true == InputCheck_Dir('2'))
		{
			if (nullptr != CallBack_BackSword)
			{
				CallBack_BackSword();
			}
		}

		if (nullptr != CallBack_Sword)
		{
			CallBack_Sword();
		}
	}
	// Jump
	if (GameEngineInput::IsDown("Player_Jump"))
	{
		if (true == InputCheck_Dir('4'))
		{
			if (nullptr != CallBack_LeftJump)
			{
				CallBack_LeftJump();
			}
		}
		else if (true == InputCheck_Dir('6'))
		{
			if (nullptr != CallBack_RightJump)
			{
				CallBack_RightJump();
			}
		}
		else
		{
			int a = 0;
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
}

bool PlayerController::InputCheck_BackFront()
{
	if (QCommand.size() == 0) { return false; }
	CommandRecord* BackCommand = QCommand.back();		// Ŀ�ǵ�ť���� ���帶���� Ŀ�ǵ带 �޾ƿ´�
	if (BackCommand == nullptr) { return false; }				// Ŀ�ǵ尡 nullptr�� false
	if (BackCommand->GetKey() != '8') { return false; }		// Ŀ�ǵ尡 8�� �ƴ� �� false

	CommandRecord* SecondCommand = BackCommand->GetFront();	// 8������ �Է��� Ŀ�ǵ带 �޾ƿ�
	if (SecondCommand == nullptr) { return false; }				// Ŀ�ǵ尡 nullptr�� false

	if (SecondCommand->GetKey() == '2')
	{
		// 2�� �Է��ߴ� ���
		return true;
	}
	else if (SecondCommand->GetKey() == 'n')
	{
		// �߸��̾��� ���
		CommandRecord* ThirdCommand = SecondCommand->GetFront();	// �߸� ������ �Է��� Ŀ�ǵ带 �޾ƿ�
		if (ThirdCommand == nullptr) { return false; }				// Ŀ�ǵ尡 nullptr�� false
		return ThirdCommand->GetKey() == '2';	// 2���ٸ� true �ƴϸ� false
	}
	return false;
}


bool PlayerController::InputCheck_Dir(char _Dir)
{
	if (QCommand.size() == 0) { return false; }
	CommandRecord* BackCommand = QCommand.back();		// Ŀ�ǵ�ť���� ���帶���� Ŀ�ǵ带 �޾ƿ´�
	if (BackCommand == nullptr) { return false; }				// Ŀ�ǵ尡 nullptr�� false
	return BackCommand->GetKey() == _Dir;	// Ŀ�ǵ尡 _Dir�̸� true
}
