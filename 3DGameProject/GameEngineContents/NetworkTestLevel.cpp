#include "PrecompileHeader.h"
#include "NetworkTestLevel.h"

#include "NetworkManager.h"
#include "NetTestPlayer.h"

NetworkTestLevel* NetworkTestLevel::Inst = nullptr;

NetworkTestLevel::NetworkTestLevel()
{
	Inst = this;
}

NetworkTestLevel::~NetworkTestLevel()
{
	Inst = nullptr;
}


void NetworkTestLevel::Start()
{
	BaseLevel::Start();
	GetCamera(0)->SetProjectionType(CameraType::Perspective);

	GameEngineInput::CreateKey("Test_ConnectServer", 'C');
	GameEngineInput::CreateKey("Test_CreateTestNetEnemy", 'V');
}

void NetworkTestLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

	//0¹ø
	if (true == GameEngineInput::IsDown("ReturnToMainLevel"))
	{
		GameEngineCore::ChangeLevel("MainLevel");
		return;
	}

	//1¹ø
	if (true == GameEngineInput::IsDown("Test_ConnectServer") && false == IsConnect)
	{
		IsConnect = true;
		NetworkManager::ConnectServer(PlayerType::Nero);
		return;
	}

	//2¹ø
	if (true == GameEngineInput::IsDown("Test_CreateTestNetEnemy") && NetworkManager::IsServer())
	{
		std::shared_ptr<NetTestPlayer> OtherPlayer = nullptr;
		OtherPlayer = NetworkManager::CreateNetworkActor<NetTestPlayer>(Net_ActorType::Nero);
		return;
	}
}