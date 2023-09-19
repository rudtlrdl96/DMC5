#include "PrecompileHeader.h"
#include "NetworkTestLevel.h"

#include <GameEngineCore/PhysXCapsuleComponent.h>

#include "ContentsEnum.h"

#include "NetworkManager.h"
#include "NetTestPlayer.h"
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
}

void NetworkTestLevel::LevelChangeStart()
{
	BaseLevel::LevelChangeStart();

	//예시코드
	//std::shared_ptr<NetTestPlayer> Player = nullptr;
	//Player = CreateActor<NetTestPlayer>();

	std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>(ActorOrder::Player);
	Nero->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });

	//요걸 호출시켜주시면 됩니다.
	NetworkManager::LinkNetwork(Nero.get());
	std::shared_ptr<Plane> Flat = CreateActor<Plane>();

	NetworkManager::CreateNetworkActor<Enemy_HellCaina>(this);
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