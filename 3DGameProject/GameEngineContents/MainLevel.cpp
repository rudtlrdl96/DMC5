#include "PrecompileHeader.h"
#include "MainLevel.h"

#include "Player.h"
#include "NetworkManager.h"

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

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -500.0f });

	std::shared_ptr<Player> NewPlayer = CreateActor<Player>();
}



void MainLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

	if (false == IsMessage)
	{
		IsMessage = true;
		MsgTextBox("CurrentLevel is MainLevel");
	}

	if (true == GameEngineInput::IsDown("SelectLevel_01"))
	{
		GameEngineCore::ChangeLevel("TestLevel");
		return;
	}
	if (true == GameEngineInput::IsDown("SelectLevel_02"))
	{
		GameEngineCore::ChangeLevel("PlayerTestLevel");
		return;
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