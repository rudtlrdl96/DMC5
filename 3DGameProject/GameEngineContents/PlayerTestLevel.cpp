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
	std::shared_ptr<GameEngineActor> CollisionActor2 = CreateActor<GameEngineActor>();
	std::shared_ptr<GameEngineCollision> Collision2 = CollisionActor2->CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	CollisionActor2->GetTransform()->SetLocalScale({ 100, 200, 100 });
	CollisionActor2->GetTransform()->SetLocalPosition({ 1000, 500, 100 });
	Collision2->SetColType(ColType::OBBBOX3D);
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
	GameEngineCoreWindow::AddDebugRenderTarget(0, "Forward Target", GetMainCamera()->GetCamForwardTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(1, "Deferred Target", GetMainCamera()->GetCamDeferrdTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(2, "Light Target", GetMainCamera()->GetDeferredLightTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(3, "Alpha Target", GetMainCamera()->GetCamAlphaTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(4, "Last Target", GetMainCamera()->GetCamTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(5, "Bake Shadow", GetLevel()->GetDirectionalLight()->GetBakeTarget(0));
	GameEngineCoreWindow::AddDebugRenderTarget(6, "Last Shadow", GetLevel()->GetDirectionalLight()->GetShadowTarget());

	if (false)
	{
		Nero = CreateActor<PlayerActor_Nero>();
		Nero->SetUserControllType();
	}
	if (true)
	{
		Vergil = CreateActor<PlayerActor_Vergil>();
		Vergil->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
		Vergil->GetTransform()->SetWorldPosition({ 0, 100, 0 });
		Vergil->SetUserControllType();
	}

	if (nullptr == Light)
	{
		Light = CreatePointLight(float4(0, 50, 0), float4(256, 256), 1000);
		Flat = CreateActor<Plane>();
		GameEngineGUI::GUIWindowCreate<PlayerWindow>("PlayerWindow");
		PlayerWindow::_Nero = Nero.get();
	}
}

void PlayerTestLevel::LevelChangeEnd()
{
	if (nullptr != Nero)
	{
		Nero->Death();
		Nero = nullptr;
	}
	if (nullptr != Vergil)
	{
		Vergil->Death();
		Vergil = nullptr;
	}
	IsMessage = false;
}
