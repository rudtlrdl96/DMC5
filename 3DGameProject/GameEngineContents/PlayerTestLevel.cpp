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
#include "PlayerActor_Vergil.h"
#include "FXAA_Effect.h"

#include <GameEngineCore/GameEngineCollision.h>
#include "PlayerWindow.h"
#include <GameEngineCore/PhysXBoxComponent.h>
#include <GameEngineCore/PhysXDefault.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

PlayerTestLevel::PlayerTestLevel()
{
}

PlayerTestLevel::~PlayerTestLevel()
{

}

void PlayerTestLevel::Start()
{
	CreateScene(GetName());

	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();

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
	GameEngineCoreWindow::Clear();
	GameEngineCoreWindow::AddDebugRenderTarget(0, "AllRenderTarget", GetMainCamera()->GetCamAllRenderTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(1, "LightRenderTarget", GetMainCamera()->GetDeferredLightTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(2, "MainCameraForwardTarget", GetMainCamera()->GetCamForwardTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(3, "DeferredTarget", GetMainCamera()->GetCamDeferrdTarget());
	//GameEngineCoreWindow::AddDebugRenderTarget(4, "CubeRenderTarget", GetMainCamera()->GetCubeRenderTarget());

	SetLevelSceneGravity(2000);
	std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
	Nero->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
	Nero->SinglePlayLoad();
	//std::shared_ptr<PlayerActor_Vergil> Vergil = CreateActor<PlayerActor_Vergil>();
	//Vergil->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
	std::shared_ptr<GameEngineActor> CollisionActor = CreateActor<GameEngineActor>();
	std::shared_ptr<GameEngineCollision> Collision = CollisionActor->CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	CollisionActor->GetTransform()->SetLocalScale({ 100, 100, 100 });
	CollisionActor->GetTransform()->SetLocalPosition({ 100, 0, 100 });
	Collision->SetColType(ColType::OBBBOX3D);
	//Collision->DebugOn();
	//IsDebugSwitch();

	//std::shared_ptr<TestObject> Component = CreateActor<TestObject>();
	std::shared_ptr<Plane> Flat = CreateActor<Plane>();

	PlayerWindow::Function1 = std::bind(&PhysXDefault::SetDynamicFriction, Flat->Component, std::placeholders::_1);
	std::shared_ptr<Wall> Flat2 = CreateActor<Wall>();
	Flat2->GetTransform()->AddWorldPosition(float4{ 700, 0, 700 });
	Flat2->GetTransform()->AddWorldRotation(float4{ 0, 0, 90 });

	std::shared_ptr<Plane> Flat3 = CreateActor<Plane>();
	Flat3->GetTransform()->AddWorldPosition(float4{ -500, -100, 0 });
	//std::shared_ptr<Slope> Flat3 = CreateActor<Slope>();
	//Flat3->GetTransform()->AddWorldPosition(float4{ -200, 0, 100 });
	//Flat3->GetTransform()->AddWorldRotation(float4{ 0, 0, -30 });
	//Flat3->GetTransform()->AddWorldRotation(float4{ 0, -45, 0 });
	std::shared_ptr<Wall> Flat4 = CreateActor<Wall>();
	Flat4->GetTransform()->AddWorldPosition(float4{ 300, 0, 300 });
	Flat4->GetTransform()->AddWorldRotation(float4{ 0, 0, 90 });
	Flat4->GetTransform()->AddWorldRotation(float4{ 0, -40, 0 });

	std::shared_ptr<GameEngineLight> Light = CreatePointLight(float4(0, 50, 0), 1000);

	//std::shared_ptr<Ball> ball = CreateActor<Ball>();

	//std::shared_ptr<TestEmpusa> Em = CreateActor<TestEmpusa>();

	//GameEngineGUI::GUIWindowCreate<PlayerWindow>("PlayerWindow");
	//PlayerWindow::_Nero = Nero.get();
}

void PlayerTestLevel::LevelChangeEnd()
{
	IsMessage = false;
}
