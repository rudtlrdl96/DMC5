#include "PrecompileHeader.h"
#include "PlayerTestLevel.h"

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
#include "PlayerActor_Nero.h"
#include <GameEngineCore/GameEngineCollision.h>
#include "PlayerWindow.h"
PlayerTestLevel::PlayerTestLevel()
{
}

PlayerTestLevel::~PlayerTestLevel()
{

}

void PlayerTestLevel::Start()
{
	GetCamera(0)->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 50.0f, -100.0f });
}

void PlayerTestLevel::Update(float _DeltaTime)
{
	if (false == IsMessage)
	{
		IsMessage = true;
		//MsgTextBox("CurrentLevel is PlayerTestLevel");
	}

	if (true == GameEngineInput::IsDown("ReturnToMainLevel"))
	{
		GameEngineCore::ChangeLevel("MainLevel");
	}
}

void PlayerTestLevel::LevelChangeStart()
{
	std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
	Nero->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
	std::shared_ptr<GameEngineActor> CollisionActor = CreateActor<GameEngineActor>();
	std::shared_ptr<GameEngineCollision> Collision = CollisionActor->CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	CollisionActor->GetTransform()->SetLocalScale({ 100, 100, 100 });
	CollisionActor->GetTransform()->SetLocalPosition({ 100, 0, 100 });
	Collision->SetColType(ColType::OBBBOX3D);
	Collision->DebugOn();
	IsDebugSwitch();

	//std::shared_ptr<TestObject> Component = CreateActor<TestObject>();
	std::shared_ptr<Plane> Flat = CreateActor<Plane>();

	std::shared_ptr<Wall> Flat2 = CreateActor<Wall>();
	Flat2->GetTransform()->AddWorldPosition(float4{ 700, 0, 700 });
	Flat2->GetTransform()->AddWorldRotation(float4{ 0, 0, 90 });

	//std::shared_ptr<Slope> Flat3 = CreateActor<Slope>();
	//Flat3->GetTransform()->AddWorldPosition(float4{ -200, 0, 100 });
	//Flat3->GetTransform()->AddWorldRotation(float4{ 0, 0, -30 });
	//Flat3->GetTransform()->AddWorldRotation(float4{ 0, -45, 0 });

	std::shared_ptr<GameEngineLight> Light = CreateActor<GameEngineLight>();

	//std::shared_ptr<Ball> ball = CreateActor<Ball>();

	//std::shared_ptr<TestEmpusa> Em = CreateActor<TestEmpusa>();

	GameEngineGUI::GUIWindowCreate<PlayerWindow>("PlayerWindow");
	PlayerWindow::_Nero = Nero.get();
}

void PlayerTestLevel::LevelChangeEnd()
{
	IsMessage = false;
}
