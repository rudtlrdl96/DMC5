#include "PrecompileHeader.h"
#include "TestLevel.h"

#include "TestObject.h"
#include "Plane.h"
#include "Enemy_Empusa.h"
#include "Enemy_GreenEmpusa.h"
#include "Enemy_RedEmpusa.h"
#include "Enemy_HellCaina.h"
#include "Enemy_HellAntenora.h"
#include "CavaliereAngelo.h"
#include "Ball.h"
#include "Wall.h"
#include "Slope.h"
#include <GameEngineCore/EngineGrid.h>
#include "Box200.h"

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
	CreateScene(GetName());

	GetCamera(0)->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 50.0f, -100.0f });

	// std::shared_ptr<EngineGrid> Grid = CreateActor<EngineGrid>();
}

void TestLevel::Update(float _DeltaTime)
{
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
	std::shared_ptr<TestObject> Component = CreateActor<TestObject>();

	std::shared_ptr<Plane> Flat = CreateActor<Plane>();

	std::shared_ptr<Wall> Flat2 = CreateActor<Wall>();
	Flat2->GetTransform()->AddWorldPosition(float4{ 300, 0, 300 });
	Flat2->GetTransform()->AddWorldRotation(float4{ 0, 0, 90 });
	Flat2->GetTransform()->AddWorldRotation(float4{ 0, -40, 0});

	std::shared_ptr<Box200> Box = CreateActor<Box200>();
	Box->GetTransform()->AddWorldPosition(float4{ -300, 100, -300 });

	//std::shared_ptr<Box200> Box2 = CreateActor<Box200>();
	//Box2->GetTransform()->AddWorldPosition(float4{ -100, 0, 100 });

}

void TestLevel::LevelChangeEnd()
{
	IsMessage = false;
}
