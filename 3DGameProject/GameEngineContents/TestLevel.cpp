#include "PrecompileHeader.h"
#include "TestLevel.h"

TestLevel::TestLevel() 
{
}

TestLevel::~TestLevel() 
{
}

void TestLevel::Start()
{
	GetCamera(0)->SetProjectionType(CameraType::Orthogonal);
}

void TestLevel::Update(float _DeltaTime)
{
	if (false == IsMessage)
	{
		IsMessage = true;
		MsgTextBox("CurrentLevel is TestLevel");
	}

	if (true == GameEngineInput::IsDown("ReturnToMainLevel"))
	{
		GameEngineCore::ChangeLevel("MainLevel");
	}
}

void TestLevel::LevelChangeStart()
{
	GameEngineLevel::LevelChangeStart();
	GameEnginePhysics::LevelChangeStart();
}

void TestLevel::LevelChangeEnd()
{
	IsMessage = false;
	GameEngineLevel::LevelChangeEnd();
}
