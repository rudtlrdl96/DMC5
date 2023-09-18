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

	std::shared_ptr<GameEngineActor> CollisionActor = CreateActor<GameEngineActor>();
	std::shared_ptr<GameEngineCollision> Collision = CollisionActor->CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	CollisionActor->GetTransform()->SetLocalScale({ 100, 200, 100 });
	CollisionActor->GetTransform()->SetLocalPosition({ 100, 100, 100 });
	Collision->SetColType(ColType::OBBBOX3D);
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

	Vergil = CreateActor<PlayerActor_Vergil>();
	Vergil->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
	Vergil->GetTransform()->SetWorldPosition({ 0, 100, 0 });

	if (nullptr == Light)
	{
		Light = CreatePointLight(float4(0, 50, 0), 1000);
		Flat = CreateActor<Plane>();
	}
}

void PlayerTestLevel::LevelChangeEnd()
{
	Vergil->Death();
	Vergil = nullptr;
	IsMessage = false;
}
