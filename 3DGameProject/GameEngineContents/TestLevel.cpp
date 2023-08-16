#include "PrecompileHeader.h"
#include "TestLevel.h"

#include <GameEngineCore/PhysXBoxGeometryComponent.h>

#include "TestObject.h"

TestLevel::TestLevel() 
{
}

TestLevel::~TestLevel() 
{
}

void TestLevel::Start()
{
	GetCamera(0)->SetProjectionType(CameraType::Perspective);
}

void TestLevel::Update(float _DeltaTime)
{
	PhysicsLevel::Update(_DeltaTime);

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
	PhysicsLevel::LevelChangeStart();

	std::shared_ptr<TestObject> Component = CreateActor<TestObject>();
}

void TestLevel::LevelChangeEnd()
{
	IsMessage = false;
	GameEngineLevel::LevelChangeEnd();
	PhysicsLevel::LevelChangeEnd();
}
