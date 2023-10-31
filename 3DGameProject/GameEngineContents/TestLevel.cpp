#include "PrecompileHeader.h"
#include "TestLevel.h"

#include <GameEngineCore/GameEngineCoreWindow.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/PhysXBoxComponent.h>
#include <GameEngineCore/GameEngineFBXMesh.h>

#include "NetworkManager.h"
#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"
#include "Enemy_HellCaina.h"
#include "Enemy_Empusa.h"
#include "Plane.h"
#include "Wall.h"
#include "CavaliereAngelo.h"
#include "TestObject.h"
#include "ColorEffect.h"
#include "JudgementCut.h"
#include "Item_DevilBreaker.h"
#include "FXAA_Effect.h"
#include "DistortionEffect.h"
#include "ZoomEffect.h"
#include "Enemy_HellAntenora.h"
#include "Enemy_ScudoAngelo.h"
#include "Enemy_ProtoAngelo.h"
#include "Enemy_Qliphoth.h"

#include "BGMPlayer.h"
#include "Item_RedOrbLump.h"
#include "FadeEffect.h"
TestLevel::TestLevel()
{
}

TestLevel::~TestLevel()
{

}

void TestLevel::Start()
{
	if (false == GameEngineInput::IsKey("MonsterTest"))
	{
		GameEngineInput::CreateKey("MonsterTest", 'M');
	}

	CreateScene(GetName());

	GetCamera(0)->GetCamTarget()->CreateEffect<ColorEffect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();
	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<ZoomEffect>();

	GetCamera(0)->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 50.0f, -100.0f });
	GetCamera(100)->GetCamTarget()->DepthSettingOff();
	std::shared_ptr<FadeEffect> Fade = GetLastTarget()->CreateEffect<FadeEffect>();

	if (false)
	{
		std::shared_ptr<DistortionEffect> Distortion = GetCamera(0)->GetCamTarget()->CreateEffect<DistortionEffect>();
		Distortion->SetMaskTexture(GetCamera(0)->GetCamAlphaTarget(), 1);
		Distortion->SetDistortionValue(10, 10);
		Distortion->SetMaxPixelValue(100, 100);
	}
}

bool Is = false;
int cal = 0;

void TestLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("MonsterTest"))
	{
		if (nullptr == HellCaina)
		{
			HellCaina = CreateActor<Enemy_HellCaina>();
			HellCaina->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
			HellCaina->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
		}
	}

	if (1 == cal)
	{
		cal = 0;
		if (true == Is)
		{
			if (nullptr != HellCaina)
			{
				HellCaina->On();
			}
		}
		else
		{
			if (nullptr != HellCaina)
			{
				HellCaina->Off();
			}
		}
	}
}

void TestLevel::LevelChangeStart()
{
	//IsDebugSwitch();
	SetLevelSceneGravity(2000);

	if(nullptr == GameEngineFBXMesh::Find("Ground_Mesh.fbx"))
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Mesh");
		NewDir.Move("TestMesh");
		GameEngineFBXMesh::Load(NewDir.GetPlusFileName("Ground_Mesh.fbx").GetFullPath());
	}

	if (nullptr == Player)
	{
		if (true)
		{
			Player = CreateActor<PlayerActor_Nero>();
			Player->GetPhysXComponent()->SetWorldPosition({ -1200, 100, -1300 });
			Player->SetUserControllType();
			NetworkManager::LinkNetwork(Player.get(), this);

			//std::shared_ptr<Item_DevilBreaker> Item_Overture = CreateActor<Item_DevilBreaker>();
			//Item_Overture->SetDevilBreaker(DevilBreaker::Overture);
			//Item_Overture->GetTransform()->SetLocalPosition({ -1000, 50, -1000 });

			//std::shared_ptr<Item_DevilBreaker> Item_Gerbera = CreateActor<Item_DevilBreaker>();
			//Item_Gerbera->SetDevilBreaker(DevilBreaker::Gerbera);
			//Item_Gerbera->GetTransform()->SetLocalPosition({ -1200, 50, -1000 });

			std::shared_ptr<Item_DevilBreaker> Item_Buster = CreateActor<Item_DevilBreaker>();
			Item_Buster->SetDevilBreaker(DevilBreaker::BusterArm);
			Item_Buster->GetTransform()->SetLocalPosition({ -800, 50, -1000 });
		}
		else
		{
			Player = CreateActor<PlayerActor_Vergil>();
			Player->GetPhysXComponent()->SetWorldPosition({ -1200, 100, -1300 });
			Player->SetUserControllType();
			NetworkManager::LinkNetwork(Player.get(), this);
		}
	}
	//Player->CreateComponent<GameEngineCollision>(CollisionOrder::Shop);
	//CreateActor<Item_RedOrbLump>();

	//if (nullptr == TestObj)
	//{
	//	TestObj = CreateActor<TestObject>();
	//	TestObj->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
	//	TestObj->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
	//}

	if (nullptr == HellCaina)
	{
		HellCaina = CreateActor<Enemy_HellCaina>();
		HellCaina->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
		HellCaina->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
	}

	//if (nullptr == Empusa)
	//{
	//	Empusa = CreateActor<Enemy_Empusa>();
	//	Empusa->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
	//	Empusa->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
	//}

	//if (nullptr == Cavaliere)
	//{
	//	Cavaliere = CreateActor<CavaliereAngelo>();
	//	Cavaliere->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
	//	Cavaliere->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
	//	Player->SetBossCam(Cavaliere->GetTransform());
	//}

	if (nullptr == Flat)
	{
		Flat = CreateActor<Plane>();
		Flat->GetPhysXComponent()->SetWorldPosition({ 0, -50, 0 });
	}

	//if (nullptr == HellAntenora)
	//{
	//	HellAntenora = CreateActor<Enemy_HellAntenora>();
	//	HellAntenora->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
	//	HellAntenora->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
	//}

	//if (nullptr == Scudo)
	//{
	//	Scudo = CreateActor<Enemy_ScudoAngelo>();
	//	Scudo->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
	//	Scudo->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
	//}

	//if (nullptr == Proto)
	//{
	//	Proto = CreateActor<Enemy_ProtoAngelo>();
	//	Proto->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });
	//	Proto->GetPhysXComponent()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
	//}

	//if (nullptr == Qliphoth)
	//{
	//	Qliphoth = CreateActor<Enemy_Qliphoth>();
	//	Qliphoth->GetTransform()->SetWorldPosition({ 0, 0, 0 });
	//	Qliphoth->GetTransform()->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
	//}

	//GameEngineCoreWindow::Clear();
	//GameEngineCoreWindow::AddDebugRenderTarget(0, "Forward Target", GetCamera(100)->GetCamForwardTarget());
	//GameEngineCoreWindow::AddDebugRenderTarget(1, "Deferred Target", GetCamera(100)->GetCamDeferrdTarget());
	//GameEngineCoreWindow::AddDebugRenderTarget(2, "Light Target", GetCamera(100)->GetDeferredLightTarget());
	//GameEngineCoreWindow::AddDebugRenderTarget(3, "Alpha Target", GetCamera(100)->GetCamAlphaTarget());
	//GameEngineCoreWindow::AddDebugRenderTarget(4, "Last Target", GetCamera(100)->GetCamTarget());
	//GameEngineCoreWindow::AddDebugRenderTarget(5, "Bake Shadow", GetLevel()->GetDirectionalLight()->GetBakeTarget(0));
	//GameEngineCoreWindow::AddDebugRenderTarget(6, "Last Shadow", GetLevel()->GetDirectionalLight()->GetShadowTarget());
}

void TestLevel::LevelChangeEnd()
{
	Empusa = nullptr;
}
