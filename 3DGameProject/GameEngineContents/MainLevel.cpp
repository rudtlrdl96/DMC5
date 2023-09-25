#include "PrecompileHeader.h"
#include "MainLevel.h"

#include <GameEngineCore/GameEngineCoreWindow.h>

#include "JudgementCut.h"
#include "ZoomEffect.h"
#include "FXAA_Effect.h"

#include "ShaderTestActor.h"
#include "Plane.h"
#include "Enemy_HellCaina.h"
#include "FreeCameraActor.h"

#include "EffectRenderer.h"

MainLevel* MainLevel::Inst = nullptr;

MainLevel::MainLevel() 
{
	Inst = this;
}

MainLevel::~MainLevel() 
{
	Inst = nullptr;
}

void MainLevel::Start()
{
	BaseLevel::Start();

	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();
	GetCamera(0)->GetCamTarget()->CreateEffect<ZoomEffect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -100.0f });

	//std::shared_ptr<ShaderTestActor> NewTestActor0 = CreateActor<ShaderTestActor>();
	//NewTestActor0->GetTransform()->SetLocalPosition({ 0, 10, 0 });
	//NewTestActor0->InitTest(0);

	
	//std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
	//	{
	//		"Texture", "ShaderDebug"
	//	});
	//
	//GameEngineSprite::LoadSheet(Path + "\\BoneSkull_AttackB.png", 4, 1);
	//GameEngineSprite::LoadFolder(Path + "\\DD_Move");
	//
	//{
	//	std::shared_ptr<EffectRenderer> NewEffect = CreateActor<GameEngineActor>()->CreateComponent<EffectRenderer>();
	//
	//	NewEffect->RectInit("Effect_2D");
	//	NewEffect->GetTransform()->SetWorldScale(float4(100, 100, 100));
	//
	//	NewEffect->CreateAnimation({ .AnimationName = "Test", .SpriteName = "BoneSkull_AttackB.png" });
	//	NewEffect->ChangeAnimation("Test");
	//}
	//
	//{
	//	std::shared_ptr<EffectRenderer> NewEffect = CreateActor<GameEngineActor>()->CreateComponent<EffectRenderer>();
	//
	//	NewEffect->RectInit("Effect_2D");
	//	NewEffect->GetTransform()->SetWorldPosition(float4(100, 0, 0));
	//	NewEffect->GetTransform()->SetWorldScale(float4(100, 100, 100));
	//
	//	NewEffect->CreateAnimation({ .AnimationName = "Test", .SpriteName = "DD_Move" });
	//	NewEffect->ChangeAnimation("Test");
	//	NewEffect->LockRotation();
	//}


}

void MainLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

	if (false == IsMessage)
	{
		IsMessage = true;
		MsgTextBox("CurrentLevel is MainLevel");
	}

	if (true == GameEngineInput::IsDown("SelectLevel_01"))
	{
		GameEngineCore::ChangeLevel("TestLevel");
		return;
	}
	if (true == GameEngineInput::IsDown("SelectLevel_02"))
	{
		GameEngineCore::ChangeLevel("PlayerTestLevel");
		return;
	}
	if (true == GameEngineInput::IsDown("SelectLevel_03"))
	{
		GameEngineCore::ChangeLevel("AnimationToolLevel");
		return;
	}
	if (true == GameEngineInput::IsDown("SelectLevel_04"))
	{
		GameEngineCore::ChangeLevel("StageEditLevel");
		return;
	}
	if (true == GameEngineInput::IsDown("SelectLevel_05"))
	{
		GameEngineCore::ChangeLevel("PlayUITestLevel");
		return;
	}
	if (true == GameEngineInput::IsDown("SelectLevel_06"))
	{
		GameEngineCore::ChangeLevel("TestStageLevel");
		return;
	}
	if (true == GameEngineInput::IsDown("SelectLevel_07"))
	{
		GameEngineCore::ChangeLevel("PhysXTestLevel");
		return;
	}
	if (true == GameEngineInput::IsDown("SelectLevel_08"))
	{
		GameEngineCore::ChangeLevel("NetworkTestLevel");
		return;
	}
	if (true == GameEngineInput::IsDown("SelectLevel_09"))
	{
		GameEngineCore::ChangeLevel("EffectToolLevel");
		return;
	}
}

void MainLevel::LevelChangeStart()
{
	GameEngineLevel::LevelChangeStart();

	GameEngineCoreWindow::Clear();
	GameEngineCoreWindow::AddDebugRenderTarget(0, "Forward Target", GetMainCamera()->GetCamForwardTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(1, "Deferred Target", GetMainCamera()->GetCamDeferrdTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(2, "Light Target", GetMainCamera()->GetDeferredLightTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(3, "Alpha Target", GetMainCamera()->GetCamAlphaTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(4, "Last Target", GetMainCamera()->GetCamTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(5, "Bake Shadow", GetLevel()->GetDirectionalLight()->GetBakeTarget(0));
	GameEngineCoreWindow::AddDebugRenderTarget(6, "Last Shadow", GetLevel()->GetDirectionalLight()->GetShadowTarget());
}

void MainLevel::LevelChangeEnd()
{
	IsMessage = false;
	GameEngineLevel::LevelChangeEnd();
}