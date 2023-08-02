#include "PrecompileHeader.h"
#include "MainLevel.h"

MainLevel::MainLevel() 
{
}

MainLevel::~MainLevel() 
{
}

void MainLevel::Start()
{
	if (false == GameEngineInput::IsKey("SelectLevel_01"))
	{
		GameEngineInput::CreateKey("SelectLevel_01", '1');
		GameEngineInput::CreateKey("SelectLevel_02", '2');
		GameEngineInput::CreateKey("SelectLevel_03", '3');
		GameEngineInput::CreateKey("SelectLevel_04", '4');
		GameEngineInput::CreateKey("SelectLevel_05", '5');
		GameEngineInput::CreateKey("SelectLevel_06", '6');
		GameEngineInput::CreateKey("SelectLevel_07", '7');
		GameEngineInput::CreateKey("SelectLevel_08", '8');
		GameEngineInput::CreateKey("SelectLevel_09", '9');
		GameEngineInput::CreateKey("ReturnToMainLevel", '0');
	}

	GetCamera(0)->SetProjectionType(CameraType::Orthogonal);
}

void MainLevel::Update(float _DeltaTime)
{
	if (false == IsMessage)
	{
		IsMessage = true;
		MsgTextBox("CurrentLevel is MainLevel");
	}

	if (true == GameEngineInput::IsDown("SelectLevel_01"))
	{
		GameEngineCore::ChangeLevel("TestLevel");
	}
}

void MainLevel::LevelChangeStart()
{
	GameEngineLevel::LevelChangeStart();
}

void MainLevel::LevelChangeEnd()
{
	IsMessage = false;
	GameEngineLevel::LevelChangeEnd();
}