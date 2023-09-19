#include "PrecompileHeader.h"
#include "TestLevel.h"

#include <GameEngineCore/GameEngineCoreWindow.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "PlayerActor_Nero.h"
#include "Enemy_HellCaina.h"
#include "Plane.h"
#include "Wall.h"

#include "FXAA_Effect.h"

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

	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();

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
	SetLevelSceneGravity(2000);

	std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
	Nero->GetPhysXComponent()->SetWorldPosition({ -1200, 100, -1300 });
	Nero->SinglePlayLoad();

	std::shared_ptr<Enemy_HellCaina> HellCaina = CreateActor<Enemy_HellCaina>();
	HellCaina->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
	HellCaina->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });

	std::shared_ptr<Plane> Flat = CreateActor<Plane>();
	std::shared_ptr<GameEngineLight> Light = CreatePointLight(float4(0, 50, 0), 1000);
}

void TestLevel::LevelChangeEnd()
{
	IsMessage = false;
}
