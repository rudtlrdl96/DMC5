#include "PrecompileHeader.h"
#include "NetworkTestLevel.h"

#include <GameEngineCore/PhysXCapsuleComponent.h>

#include "NetworkManager.h"
#include "NetTestPlayer.h"
#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"
#include "Plane.h"

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

	//�����ڵ�
	//std::shared_ptr<NetTestPlayer> Player = nullptr;
	//Player = CreateActor<NetTestPlayer>();

	std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
	Nero->GetPhysXComponent()->SetWorldPosition({ 0, 100, 0 });

	//��� ȣ������ֽø� �˴ϴ�.
	NetworkManager::LinkNetwork(Nero.get());
	std::shared_ptr<Plane> Flat = CreateActor<Plane>();
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
}