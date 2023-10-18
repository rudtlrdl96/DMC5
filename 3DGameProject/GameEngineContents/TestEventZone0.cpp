#include "PrecompileHeader.h"
#include "TestEventZone0.h"
#include "TestStageLevel.h"

TestEventZone0::TestEventZone0()
{

}

TestEventZone0::~TestEventZone0()
{

}

void TestEventZone0::Start()
{
	EventZone::Start();
	Event = TestStageLevel::Inst->TestEvent0;
}
