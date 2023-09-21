#include "PrecompileHeader.h"
#include "TestEventZone.h"

#include <GameEngineCore/GameEngineCollision.h>

TestEventZone::TestEventZone()
{

}

TestEventZone::~TestEventZone()
{

}

void TestEventZone::Start()
{
	TriggerZone = CreateComponent<GameEngineCollision>(CollisionOrder::EventZone);
	TriggerZone->SetColType(ColType::OBBBOX3D);
	Event = []()
		{
			GameEngineCore::ChangeLevel("MainLevel");
		};
}
