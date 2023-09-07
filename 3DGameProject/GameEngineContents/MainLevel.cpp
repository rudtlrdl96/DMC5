#include "PrecompileHeader.h"
#include "MainLevel.h"

#include <GameEngineCore/GameEngineCoreWindow.h>

#include "JudgementCut.h"
#include "ZoomEffect.h"
#include "FXAA_Effect.h"

#include "ShaderTestActor.h"
#include "Plane.h"
#include "FreeCameraActor.h"


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
	GetMainCamera()->GetTransform()->SetLocalPosition({ 140, 0, -250.0f });

	std::shared_ptr<ShaderTestActor> NewTestActor0 = CreateActor<ShaderTestActor>();
	NewTestActor0->GetTransform()->SetLocalPosition({ 0, 10, 0 });
	NewTestActor0->InitTest(0);

	std::shared_ptr<ShaderTestActor> NewTestActor1 = CreateActor<ShaderTestActor>();
	NewTestActor1->GetTransform()->SetLocalPosition({ 70, 10, 0 });
	NewTestActor1->InitTest(1);

	std::shared_ptr<ShaderTestActor> NewTestActor2 = CreateActor<ShaderTestActor>();
	NewTestActor2->GetTransform()->SetLocalPosition({ 140, 10, 0 });
	NewTestActor2->InitTest(2);

	std::shared_ptr<ShaderTestActor> NewTestActor3 = CreateActor<ShaderTestActor>();
	NewTestActor3->GetTransform()->SetLocalPosition({ 210, 10, 0 });
	NewTestActor3->InitTest(3);

	std::shared_ptr<ShaderTestActor> NewTestActor4 = CreateActor<ShaderTestActor>();
	NewTestActor4->GetTransform()->SetLocalPosition({ 280, 10, 0 });
	NewTestActor4->InitTest(4);

	std::shared_ptr<FreeCameraActor> FreeCameraAc = CreateActor<FreeCameraActor>();
	FreeCameraAc->GetTransform()->AddWorldPosition({ 0.0f, 0.0f, -300.0f });

	//std::shared_ptr<GameEngineLight> NewPointLight = CreatePointLight(float4(140, 10, 0), 300);

	//CreateScene("MainLevel");

	std::shared_ptr<GameEngineLight> NewSpotLight = CreateSpotLight(float4(0, 50, 0), 500, 5);
	NewSpotLight->GetTransform()->SetLocalRotation(float4(-90, 0, 0));
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
		GameEngineCore::ChangeLevel("TitleLevel");
		return;
	}
	if (true == GameEngineInput::IsDown("SelectLevel_06"))
	{
		GameEngineCore::ChangeLevel("TestStageLevel");
		return;
	}
	if (true == GameEngineInput::IsDown("SelectLevel_07"))
	{
		GameEngineCore::ChangeLevel("MonsterTestLevel");
		return;
	}
	if (true == GameEngineInput::IsDown("SelectLevel_08"))
	{
		GameEngineCore::ChangeLevel("NetworkTestLevel");
		return;
	}
}

void MainLevel::LevelChangeStart()
{
	GameEngineLevel::LevelChangeStart();

	GameEngineCoreWindow::Clear();
	GameEngineCoreWindow::AddDebugRenderTarget(0, "AllRenderTarget", GetMainCamera()->GetCamAllRenderTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(1, "LightRenderTarget", GetMainCamera()->GetDeferredLightTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(2, "MainCameraForwardTarget", GetMainCamera()->GetCamForwardTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(3, "DeferredTarget", GetMainCamera()->GetCamDeferrdTarget());
}

void MainLevel::LevelChangeEnd()
{
	IsMessage = false;
	GameEngineLevel::LevelChangeEnd();
}