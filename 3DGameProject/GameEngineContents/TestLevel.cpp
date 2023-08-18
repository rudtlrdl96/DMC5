#include "PrecompileHeader.h"
#include "TestLevel.h"

#include <GameEngineCore/PhysXBoxGeometryComponent.h>

#include "TestObject.h"
#include "Plane.h"
#include "Enemy_Empusa.h"

TestLevel* TestLevel::TestLevelPtr = nullptr;

TestLevel::TestLevel() 
{
	TestLevelPtr = this;
}

TestLevel::~TestLevel() 
{

}

void TestLevel::Start()
{
	GetCamera(0)->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -100.0f });
}

void TestLevel::Update(float _DeltaTime)
{
	PhysicsLevel::Update(_DeltaTime);

	if (false == IsMessage)
	{
		IsMessage = true;
		//MsgTextBox("CurrentLevel is TestLevel");
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
	std::shared_ptr<Plane> Flat = CreateActor<Plane>();
	std::shared_ptr<Enemy_Empusa> NewEmemy = CreateActor<Enemy_Empusa>();
}

void TestLevel::LevelChangeEnd()
{
	IsMessage = false;
	GameEngineLevel::LevelChangeEnd();
	PhysicsLevel::LevelChangeEnd();
}
