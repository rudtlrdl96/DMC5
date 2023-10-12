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
	EventZone::Start();
	Event = []()
		{
			GameEngineCore::ChangeLevel("MainLevel");
		};
}
