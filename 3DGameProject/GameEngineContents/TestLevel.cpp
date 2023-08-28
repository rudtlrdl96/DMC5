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
#include "PlayerActor_Nero.h"

//#include <GameEngineCore/GameEngineCollision.h>

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
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 50.0f, -100.0f });

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
	//CreateActor<PlayerActor_Nero>();
	//std::shared_ptr<GameEngineActor> CollisionActor = CreateActor<GameEngineActor>();
	//std::shared_ptr<GameEngineCollision> Collision = CollisionActor->CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	//CollisionActor->GetTransform()->SetLocalScale({ 100, 100, 100 });
	//CollisionActor->GetTransform()->SetLocalPosition({ 100, 0, 100 });
	//Collision->SetColType(ColType::OBBBOX3D);
	//IsDebugSwitch();

	std::shared_ptr<TestObject> Component = CreateActor<TestObject>();
	std::shared_ptr<Plane> Flat = CreateActor<Plane>();

	std::shared_ptr<Plane> Flat2 = CreateActor<Plane>();
	Flat2->GetTransform()->AddWorldPosition(float4{ 100, 0, 100 });
	Flat2->GetTransform()->AddWorldRotation(float4{ 0, 0, 90 });
	Flat2->GetTransform()->AddWorldRotation(float4{ 0, -40, 0});

	//std::shared_ptr<Plane> Flat3 = CreateActor<Plane>();
	//Flat3->GetTransform()->AddWorldPosition(float4{ -100, 0, 0 });
	//Flat3->GetTransform()->AddWorldRotation(float4{ 0, 0, -40 });
	//Flat3->GetTransform()->AddWorldRotation(float4{ 0, -45, 0 });

	std::shared_ptr<GameEngineLight> Light = CreateActor<GameEngineLight>();

	//std::shared_ptr<Ball> ball = CreateActor<Ball>();

	//std::shared_ptr<TestEmpusa> Em = CreateActor<TestEmpusa>();
}

void TestLevel::LevelChangeEnd()
{
	IsMessage = false;
}
