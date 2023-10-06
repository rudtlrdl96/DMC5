#include "PrecompileHeader.h"
#include "TestLevel.h"

#include <GameEngineCore/GameEngineCoreWindow.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "NetworkManager.h"
#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"
#include "Enemy_HellCaina.h"
#include "Enemy_Empusa.h"
#include "Plane.h"
#include "Wall.h"
#include "CavaliereAngelo.h"

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
	//GetCamera(100)->SetSortType(0, SortType::ZSort);
	GetCamera(100)->GetCamTarget()->DepthSettingOff();

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

	if (nullptr != HellCaina)
	{
		if (-100.0f >= HellCaina->GetTransform()->GetWorldPosition().y)
		{
			HellCaina->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
			HellCaina->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
		}
	}

	if (nullptr != Empusa)
	{
		if (-100.0f >= Empusa->GetTransform()->GetWorldPosition().y)
		{
			Empusa->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
			Empusa->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
		}
	}
}

void TestLevel::LevelChangeStart()
{
	IsDebugSwitch();
	SetLevelSceneGravity(2000);

	if (true)
	{
		std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
		Nero->GetPhysXComponent()->SetWorldPosition({ -1200, 100, -1300 });
		Nero->SetUserControllType();
		NetworkManager::LinkNetwork(Nero.get(), this);
	}
	else
	{
		std::shared_ptr<PlayerActor_Vergil> Vergil = CreateActor<PlayerActor_Vergil>();
		Vergil->GetPhysXComponent()->SetWorldPosition({ -1200, 100, -1300 });
		Vergil->SetUserControllType();
		NetworkManager::LinkNetwork(Vergil.get(), this);
	}

	//if (nullptr == HellCaina)
	//{
	//	HellCaina = CreateActor<Enemy_HellCaina>();
	//	HellCaina->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
	//	HellCaina->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
	//}

	//if (nullptr == Empusa)
	//{
	//	Empusa = CreateActor<Enemy_Empusa>();
	//	Empusa->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
	//	Empusa->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
	//}

	if (nullptr == Cavaliere)
	{
		Cavaliere = CreateActor<CavaliereAngelo>();
		Cavaliere->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
		Cavaliere->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
	}

	if (nullptr == Flat)
	{
		Flat = CreateActor<Plane>();
	}

	GameEngineCoreWindow::Clear();
	GameEngineCoreWindow::AddDebugRenderTarget(0, "Forward Target", GetCamera(100)->GetCamForwardTarget());
	//GameEngineCoreWindow::AddDebugRenderTarget(1, "Deferred Target", GetCamera(100)->GetCamDeferrdTarget());
	//GameEngineCoreWindow::AddDebugRenderTarget(2, "Light Target", GetCamera(100)->GetDeferredLightTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(3, "Alpha Target", GetCamera(100)->GetCamAlphaTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(4, "Last Target", GetCamera(100)->GetCamTarget());
	//GameEngineCoreWindow::AddDebugRenderTarget(5, "Bake Shadow", GetLevel()->GetDirectionalLight()->GetBakeTarget(0));
	//GameEngineCoreWindow::AddDebugRenderTarget(6, "Last Shadow", GetLevel()->GetDirectionalLight()->GetShadowTarget());
}

void TestLevel::LevelChangeEnd()
{
	IsMessage = false;
}
