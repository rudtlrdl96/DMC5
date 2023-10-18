#include "PrecompileHeader.h"
#include "TestEventZone1.h"
#include "TestStageLevel.h"

TestEventZone1::TestEventZone1()
{

}

TestEventZone1::~TestEventZone1()
{

}

void TestEventZone1::Start()
{
	EventZone::Start();
	Event = TestStageLevel::Inst->TestEvent1;
}
