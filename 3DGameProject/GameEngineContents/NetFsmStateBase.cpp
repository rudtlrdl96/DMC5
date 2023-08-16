#include "PrecompileHeader.h"
#include "NetFsmStateBase.h"

NetFsmStateBase::NetFsmStateBase()
{

}

NetFsmStateBase::~NetFsmStateBase()
{

}


void NetFsmStateBase::EnterState()
{
	Timer = 0.f;
}

void NetFsmStateBase::Update(float _DeltaTime)
{
	Timer += _DeltaTime;
}
