#include "PrecompileHeader.h"
#include "NetworkTestLevel.h"

#include <GameEngineCore/PhysXCapsuleComponent.h>

#include "ContentsEnum.h"

#include "NetworkManager.h"
#include "NetworkGUI.h"

#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"
#include "Player_MirageBlade.h"
#include "Plane.h"
#include "Enemy_HellCaina.h"
#include "JudgementCut.h"
#include "ColorEffect.h"

NetworkTestLevel::NetworkTestLevel()
{
}

NetworkTestLevel::~NetworkTestLevel()
{

}


void NetworkTestLevel::Start()
{
	CreateScene(GetName());

	BaseLevel::Start();
	BaseLevel::SetNetLevelType(Net_LevelType::NetTestLevel);

	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();
	GetCamera(0)->GetCamTarget()->CreateEffect<ColorEffect>();
	GetCamera(0)->SetProjectionType(CameraType::Perspective);

	GameEngineInput::CreateKey("Test_BackMainLevel", VK_ESCAPE);
	GameEngineInput::CreateKey("Test_EnemyDeath", 'C');
	GameEngineInput::CreateKey("Test_EnemyOnOff", 'V');
}








void NetworkTestLevel::LevelChangeStart()
{
	BaseLevel::LevelChangeStart();

	//호스트일때 어떤 캐릭터로 플레이할지 결정합니다
	InitHost(Net_ActorType::Vergil);
	//클라이언트일때 어떤 캐릭터로 플레이할지 결정합니다
	InitClient(Net_ActorType::Nero);
	InitPool();

	std::shared_ptr<Plane> Flat = CreateActor<Plane>();
	Enemy = NetworkManager::CreateNetworkActor<Enemy_HellCaina>(this);
}


void NetworkTestLevel::InitPool()
{
	//Passive컨트롤 용 네로 오브젝트 풀링
	Poolable<PlayerActor_Nero>::CreatePool(this, static_cast<int>(ActorOrder::Player), 1,
		[](std::shared_ptr<PlayerActor_Nero> _ActorPtr)
	{
		_ActorPtr->SetControll(NetControllType::PassiveControll);
	});

	//Passive컨트롤 용 버질 오브젝트 풀링
	Poolable<PlayerActor_Vergil>::CreatePool(this, static_cast<int>(ActorOrder::Player), 1,
		[](std::shared_ptr<PlayerActor_Vergil> _ActorPtr)
	{
		_ActorPtr->SetControll(NetControllType::PassiveControll);
	});

	//Passive컨트롤 용 미자리 블레이드 오브젝트 풀링
	Poolable<Player_MirageBlade>::CreatePool(this, static_cast<int>(ActorOrder::Player), 8);

	//Monster TODO
	Poolable<Enemy_HellCaina>::CreatePool(this, static_cast<int>(ActorOrder::Enemy), 1,
		[](std::shared_ptr<Enemy_HellCaina> _ActorPtr)
	{
		_ActorPtr->SetControll(NetControllType::PassiveControll);
	});
}


void NetworkTestLevel::InitHost(Net_ActorType _PlayerType)
{
	if (false == NetworkManager::IsServer())
		return;

	std::shared_ptr<BasePlayerActor> Player = nullptr;
	Player = CreatePlayer(_PlayerType);
}


void NetworkTestLevel::InitClient(Net_ActorType _PlayerType)
{
	if (false == NetworkManager::IsClient())
		return;

	std::shared_ptr<BasePlayerActor> Player = nullptr;
	Player = CreatePlayer(_PlayerType);
}


std::shared_ptr<BasePlayerActor> NetworkTestLevel::CreatePlayer(Net_ActorType _PlayerType)
{
	std::shared_ptr<BasePlayerActor> Player = nullptr;

	switch (_PlayerType)
	{
	case Net_ActorType::Nero:
		Player = CreateActor<PlayerActor_Nero>(ActorOrder::Player);
		break;
	case Net_ActorType::Vergil:
		Player = CreateActor<PlayerActor_Vergil>(ActorOrder::Player);
		break;
	default:
	{
		MsgAssert("NetworkTestLevel에서 잘못된 플레이어를 생성하려고 했습니다");
		return nullptr;
	}
	}

	Player->GetPhysXComponent()->SetWorldPosition({ 0, 100, 100 });
	NetworkManager::LinkNetwork(Player.get(), this);
	return Player;
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


	if (true == GameEngineInput::IsDown("Test_EnemyDeath"))
	{
		if (nullptr == Enemy)
		{
			Enemy = NetworkManager::CreateNetworkActor<Enemy_HellCaina>(this);
		}
		else
		{
			Enemy->Death();
			Enemy = nullptr;
		}
	}

	if (true == GameEngineInput::IsDown("Test_EnemyOnOff") && nullptr != Enemy)
	{
		if (Enemy->IsUpdate())
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

