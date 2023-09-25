#include "PrecompileHeader.h"
#include "NetworkTestLevel.h"

#include <GameEngineCore/PhysXCapsuleComponent.h>

#include "ContentsEnum.h"

#include "NetworkManager.h"
#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"
#include "Plane.h"
#include "Enemy_HellCaina.h"

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
	CreateScene(GetName());

	BaseLevel::Start();
	BaseLevel::SetNetLevelType(Net_LevelType::NetTestLevel);

	GetCamera(0)->SetProjectionType(CameraType::Perspective);

	GameEngineInput::CreateKey("Test_BackMainLevel", VK_ESCAPE);
	GameEngineInput::CreateKey("Test_EnemyOnOffSwitch", 'C');
}

#include "Player_MirageBlade.h"

void NetworkTestLevel::LevelChangeStart()
{
	BaseLevel::LevelChangeStart();

	std::shared_ptr<PlayerActor_Vergil> Nero = CreateActor<PlayerActor_Vergil>(ActorOrder::Player);
	Nero->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });

	NetworkManager::LinkNetwork(Nero.get());
	std::shared_ptr<Plane> Flat = CreateActor<Plane>();

	Enemy = NetworkManager::CreateNetworkActor<Enemy_HellCaina>(this);

	//NetworkManager::LinkNetwork(CreateActor<Player_MirageBlade>().get());
}


void NetworkTestLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

	//0¹ø
	if (true == GameEngineInput::IsDown("Test_BackMainLevel"))
	{
		GameEngineCore::ChangeLevel("MainLevel");
		return;
	}

	if (nullptr != Enemy && GameEngineInput::IsDown("Test_EnemyOnOffSwitch"))
	{
		if (true == Enemy->IsUpdate())
		{
			Enemy->Off();
		}
		else
		{
			Enemy->On();
		}
	}
}


void NetworkTestLevel::LevelChangeEnd()
{
	BaseLevel::LevelChangeEnd();

	const std::list<std::shared_ptr<GameEngineActor>>& Players = GetActorGroup(ActorOrder::Player);
	for (std::shared_ptr<GameEngineActor> Player : Players)
	{
		PlayerActor_Nero* NetPlayer = nullptr;
		NetPlayer = dynamic_cast<PlayerActor_Nero*>(Player.get());
		if (nullptr == NetPlayer)
			continue;

		
		NetPlayer->NetDisconnect();
		NetPlayer->Death();
	}
}