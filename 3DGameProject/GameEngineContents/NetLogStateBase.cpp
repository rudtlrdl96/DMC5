#include "PrecompileHeader.h"
#include "NetLogStateBase.h"

#include "NetLogViewer.h"

NetLogStateBase::NetLogStateBase()
{

}

NetLogStateBase::~NetLogStateBase()
{

}

void NetLogStateBase::Start()
{
	NetFsmStateBase::Start();
	Owner = NetLogViewer::GetInst();
}

