#include "PrecompileHeader.h"
#include "MainLevel.h"


#include "JudgementCut.h"
#include "ZoomEffect.h"
#include "FXAA_Effect.h"

#include "ShaderTestActor.h"
#include "Plane.h"



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
	GetCamera(0)->ReflectionOn();
	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();
	GetCamera(0)->GetCamTarget()->CreateEffect<ZoomEffect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();

	if (false == GameEngineInput::IsKey("SelectLevel_01"))
	{
		GameEngineInput::CreateKey("SelectLevel_01", '1');
		GameEngineInput::CreateKey("SelectLevel_02", '2');
		GameEngineInput::CreateKey("SelectLevel_03", '3');
		GameEngineInput::CreateKey("SelectLevel_04", '4');
		GameEngineInput::CreateKey("SelectLevel_05", '5');
		GameEngineInput::CreateKey("SelectLevel_06", '6');
		GameEngineInput::CreateKey("SelectLevel_07", '7');
		GameEngineInput::CreateKey("SelectLevel_08", '8');
		GameEngineInput::CreateKey("SelectLevel_09", '9');
		GameEngineInput::CreateKey("ReturnToMainLevel", '0');
	}

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -500.0f });

	std::shared_ptr<ShaderTestActor> NewTestActor0 = CreateActor<ShaderTestActor>();
	NewTestActor0->GetTransform()->SetLocalPosition({ 0, 10, 0 });
	NewTestActor0->InitTest(0);

	std::shared_ptr<ShaderTestActor> NewTestActor1 = CreateActor<ShaderTestActor>();
	NewTestActor1->GetTransform()->SetLocalPosition({ 100, 10, 0 });
	NewTestActor1->InitTest(1);

	std::shared_ptr<ShaderTestActor> NewTestActor2 = CreateActor<ShaderTestActor>();
	NewTestActor2->GetTransform()->SetLocalPosition({ 200, 10, 0 });
	NewTestActor2->InitTest(2);

	std::shared_ptr<ShaderTestActor> NewTestActor3 = CreateActor<ShaderTestActor>();
	NewTestActor3->GetTransform()->SetLocalPosition({ 300, 10, 0 });
	NewTestActor3->InitTest(3);

	std::shared_ptr<ShaderTestActor> NewTestActor4 = CreateActor<ShaderTestActor>();
	NewTestActor4->GetTransform()->SetLocalPosition({ 400, 10, 0 });
	NewTestActor4->InitTest(4);
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


}

void MainLevel::LevelChangeEnd()
{
	IsMessage = false;
	GameEngineLevel::LevelChangeEnd();
}