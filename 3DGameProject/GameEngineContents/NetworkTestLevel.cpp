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
#include "JudgementCut.h"
#include "ColorEffect.h"
#include "Enemy_Empusa.h"
#include "Enemy_HellCaina.h"
#include "Enemy_HellAntenora.h"
#include "Enemy_ScudoAngelo.h"
#include "Enemy_ProtoAngelo.h"
#include "Enemy_Qliphoth.h"
#include "CavaliereAngelo.h"

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

	//ȣ��Ʈ�϶� � ĳ���ͷ� �÷������� �����մϴ�
	InitHost(Net_ActorType::Nero);
	//Ŭ���̾�Ʈ�϶� � ĳ���ͷ� �÷������� �����մϴ�
	InitClient(Net_ActorType::Vergil);
	InitPool();

	std::shared_ptr<Plane> Flat = CreateActor<Plane>();
	//Enemy = NetworkManager::CreateNetworkActor<Enemy_HellCaina>(this);
}


void NetworkTestLevel::InitPool()
{
	//Passive��Ʈ�� �� �׷� ������Ʈ Ǯ��
	Poolable<PlayerActor_Nero>::CreatePool(this, static_cast<int>(ActorOrder::Player), 1,
		[](std::shared_ptr<PlayerActor_Nero> _ActorPtr)
	{
		_ActorPtr->SetControll(NetControllType::PassiveControll);
	});

	//Passive��Ʈ�� �� ���� ������Ʈ Ǯ��
	Poolable<PlayerActor_Vergil>::CreatePool(this, static_cast<int>(ActorOrder::Player), 1,
		[](std::shared_ptr<PlayerActor_Vergil> _ActorPtr)
	{
		_ActorPtr->SetControll(NetControllType::PassiveControll);
	});

	//Passive��Ʈ�� �� ���ڸ� ���̵� ������Ʈ Ǯ��
	Poolable<Player_MirageBlade>::CreatePool(this, static_cast<int>(ActorOrder::Player), 8);

	//Monster TODO
	//Poolable<Enemy_HellCaina>::CreatePool(this, static_cast<int>(ActorOrder::Enemy), 1,
	//	[](std::shared_ptr<Enemy_HellCaina> _ActorPtr)
	//{
	//	_ActorPtr->SetControll(NetControllType::PassiveControll);
	//});
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
		MsgAssert("NetworkTestLevel���� �߸��� �÷��̾ �����Ϸ��� �߽��ϴ�");
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

	//0��
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
	

	if (true)
	{
		if (GameEngineInput::IsDown("SelectLevel_01"))
		{
			NetworkManager::CreateNetworkActor<Enemy_Empusa>(this);
		}
		if (GameEngineInput::IsDown("SelectLevel_02"))
		{
			NetworkManager::CreateNetworkActor<Enemy_HellCaina>(this);
		}
		if (GameEngineInput::IsDown("SelectLevel_03"))
		{
			NetworkManager::CreateNetworkActor<Enemy_Qliphoth>(this);
		}
		if (GameEngineInput::IsDown("SelectLevel_04"))
		{
			NetworkManager::CreateNetworkActor<Enemy_HellAntenora>(this);
		}
		if (GameEngineInput::IsDown("SelectLevel_05"))
		{
			NetworkManager::CreateNetworkActor<Enemy_ScudoAngelo>(this);
		}
		if (GameEngineInput::IsDown("SelectLevel_06"))
		{
			NetworkManager::CreateNetworkActor<Enemy_ProtoAngelo>(this);
		}
		if (GameEngineInput::IsDown("SelectLevel_07"))
		{
			NetworkManager::CreateNetworkActor<CavaliereAngelo>(this);
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

