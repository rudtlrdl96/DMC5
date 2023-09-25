#include "PrecompileHeader.h"
#include "PhysXTestLevel.h"

#include <GameEngineCore/PhysXCapsuleComponent.h>
#include <GameEngineCore/PhysXBoxComponent.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "PlayerActor_Nero.h"
#include "Enemy_HellCaina.h"
#include "Plane.h"
#include "Wall.h"
#include "Box200.h"

PhysXTestLevel::PhysXTestLevel() 
{
}

PhysXTestLevel::~PhysXTestLevel() 
{
}

void PhysXTestLevel::Start()
{
	CreateScene(GetName());

	GetCamera(0)->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 50.0f, -100.0f });

	if (false == GameEngineInput::IsKey("TestObj"))
	{
		GameEngineInput::CreateKey("TestObj", 'M');
	}
}

void PhysXTestLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("ReturnToMainLevel"))
	{
		GameEngineCore::ChangeLevel("MainLevel");
	}

	if (true == GameEngineInput::IsDown("TestObj"))
	{
		Death();
	}
}

void PhysXTestLevel::LevelChangeStart()
{
	std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
	Nero->GetPhysXComponent()->SetWorldPosition({ -1200, 100, -1300 });
	Nero->SetUserControllType();

	//std::shared_ptr<Enemy_HellCaina> HellCaina = CreateActor<Enemy_HellCaina>();
	//HellCaina->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
	//HellCaina->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });

	std::shared_ptr<Plane> Flat = CreateActor<Plane>();
	std::shared_ptr<GameEngineLight> Light = CreatePointLight(float4(0, 50, 0), 1000);

	std::shared_ptr<Box200> Box = CreateActor<Box200>();
	Box->GetPhysicXComponent()->SetWorldPosition({-200.0f, 100.0f, 200.0f});
}

void PhysXTestLevel::LevelChangeEnd()
{

}
