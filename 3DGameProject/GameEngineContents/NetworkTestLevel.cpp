#include "PrecompileHeader.h"
#include "NetworkTestLevel.h"

#include "NetworkManager.h"
#include "NetTestPlayer.h"
#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"

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
	BaseLevel::SetNetLevelType(Net_LevelType::NetTestLevel);

	GetCamera(0)->SetProjectionType(CameraType::Perspective);

	GameEngineInput::CreateKey("Test_ConnectServer", 'C');
	GameEngineInput::CreateKey("Test_CreateTestNetEnemy", 'V');
	GameEngineInput::CreateKey("Test_BackMainLevel", VK_ESCAPE);
}

void NetworkTestLevel::LevelChangeStart()
{
	BaseLevel::LevelChangeStart();

	//예시코드
	//std::shared_ptr<NetTestPlayer> Player = nullptr;
	//Player = CreateActor<NetTestPlayer>();

	std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
	//요걸 호출시켜주시면 됩니다.
	NetworkManager::LinkNetwork(Nero.get());
}

void NetworkTestLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

	//0번
	if (true == GameEngineInput::IsDown("Test_BackMainLevel"))
	{
		GameEngineCore::ChangeLevel("MainLevel");
		return;
	}

	//1번
	if (true == GameEngineInput::IsDown("Test_ConnectServer") && false == IsConnect)
	{
		IsConnect = true;
		//NetworkManager::ConnectServer(PlayerType::Nero);
		return;
	}

	//2번
	if (true == GameEngineInput::IsDown("Test_CreateTestNetEnemy") && NetworkManager::IsServer())
	{
		std::shared_ptr<NetTestPlayer> OtherPlayer = nullptr;
		OtherPlayer = NetworkManager::CreateNetworkActor<NetTestPlayer>(Net_ActorType::Nero);
		return;
	}
}