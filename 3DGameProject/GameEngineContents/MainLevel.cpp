#include "PrecompileHeader.h"
#include "MainLevel.h"

#include "NetworkManager.h"

#include "NetTestPlayer.h"
#include "NetTestEnemy.h"

MainLevel* MainLevel::Inst = nullptr;

#include "JudgementCut.h"
#include "ZoomEffect.h"
#include "FXAA_Effect.h"

#include "Plane.h"

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

		GameEngineInput::CreateKey("ConnectServer", 'C');
		GameEngineInput::CreateKey("CreateTestNetEnemy", 'V');
	}

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -500.0f });

	/*std::shared_ptr<NetTestPlayer> NewActor = CreateActor<NetTestPlayer>();
	NewActor->SetUserControllType();*/

	//std::shared_ptr<Plane> Flat = CreateActor<Plane>();
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
		GameEngineCore::ChangeLevel("StartStageLevel");
		return;
	}

	if (true == GameEngineInput::IsDown("ConnectServer"))
	{
		NetworkManager::ConnectServer(PlayerType::Nero);
		return;
	}

	if (true == GameEngineInput::IsDown("CreateTestNetEnemy") && NetworkManager::IsServer())
	{
		std::shared_ptr<NetTestPlayer> OtherPlayer = nullptr;
		OtherPlayer = NetworkManager::CreateNetworkActor<NetTestPlayer>(Net_ActorType::Nero);
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