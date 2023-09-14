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

	if (nullptr == GameEngineTexture::Find("TestSaveTexture.DDS"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{"Texture", "ReflectionTexture"},
			"TestSaveTexture.DDS");
	
		GameEngineTexture::Load(Path);
	}

	std::shared_ptr<ShaderTestActor> NewTestActor0 = CreateActor<ShaderTestActor>();
	NewTestActor0->GetTransform()->SetLocalPosition({ 0, 10, 0 });
	//NewTestActor0->InitTest(0);
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