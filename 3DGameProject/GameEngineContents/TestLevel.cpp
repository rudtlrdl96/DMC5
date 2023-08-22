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

	//std::shared_ptr<TestObject> Component = CreateActor<TestObject>();
	std::shared_ptr<Plane> Flat = CreateActor<Plane>();

	//std::shared_ptr<Plane> Flat2 = CreateActor<Plane>();
	//Flat2->GetTransform()->AddWorldPosition(float4{ 100, 0, 100 });
	//Flat2->GetTransform()->AddWorldRotation(float4{ 0, 0, 90 });
	//Flat2->GetTransform()->AddWorldRotation(float4{ 0, -45, 0});
	//std::shared_ptr<TestEmpusa> Em = CreateActor<TestEmpusa>();
	
	std::shared_ptr<Enemy_Empusa> Empusa = CreateActor<Enemy_Empusa>();
	Empusa->GetTransform()->SetLocalPosition({ 0, 0, 10 });

	//std::shared_ptr<Enemy_GreenEmpusa> GreenEmpusa = CreateActor<Enemy_GreenEmpusa>();
	//GreenEmpusa->GetTransform()->SetLocalPosition({ 20, 0, 10 });	
	//
	//std::shared_ptr<Enemy_RedEmpusa> RedEmpusa = CreateActor<Enemy_RedEmpusa>();
	//RedEmpusa->GetTransform()->SetLocalPosition({ -20, 0, 10 });	
	//
	//std::shared_ptr<Enemy_HellCaina> HellCaina = CreateActor<Enemy_HellCaina>();
	//HellCaina->GetTransform()->SetLocalPosition({ -40, 0, 10 });	
	//
	//std::shared_ptr<Enemy_HellAntenora> HellAntenora = CreateActor<Enemy_HellAntenora>();
	//HellAntenora->GetTransform()->SetLocalPosition({ -60, 0, 10 });	
	//
	//std::shared_ptr<CavaliereAngelo> NewCavaliereAngelo = CreateActor<CavaliereAngelo>();
	//NewCavaliereAngelo->GetTransform()->SetLocalPosition({ -90, 0, 10 });

	//std::shared_ptr<Enemy_GreenEmpusa> GreenEmpusa = CreateActor<Enemy_GreenEmpusa>();
	//GreenEmpusa->GetTransform()->SetLocalPosition({ 20, 0, 10 });
}

void TestLevel::LevelChangeEnd()
{
	IsMessage = false;
	GameEngineLevel::LevelChangeEnd();
	PhysicsLevel::LevelChangeEnd();
}
