#include "PrecompileHeader.h"
#include "MainLevel.h"

#include <GameEngineCore/GameEngineCoreWindow.h>

#include "JudgementCut.h"
#include "ZoomEffect.h"
#include "FXAA_Effect.h"

#include "ShaderTestActor.h"
#include "Plane.h"
#include "Enemy_HellCaina.h"
#include "PlayerActor_Nero.h"
#include "FreeCameraActor.h"

#include "EffectRenderer.h"

#include "ThreadLoadingLevel.h"

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

	CreateScene(GetName());
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
		GameEngineCore::ChangeLevel("Location2_Level");
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
		GameEngineCore::ChangeLevel("Location11_Level");
		return;
	}
	if (true == GameEngineInput::IsDown("SelectLevel_08"))
	{
		//GameEngineCore::ChangeLevel("NetworkTestLevel");
		ThreadLoadingLevel::ChangeLevel("NetworkTestLevel");
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

	//std::shared_ptr<ShaderTestActor> TestActor = CreateActor<ShaderTestActor>();
}

void MainLevel::LevelChangeEnd()
{
	IsMessage = false;
	GameEngineLevel::LevelChangeEnd();
}