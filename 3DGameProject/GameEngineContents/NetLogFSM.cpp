#include "PrecompileHeader.h"
#include "NetLogFSM.h"

#include "NetLogState_EntryReady.h"
#include "NetLogState_LogPrint.h"

NetLogFSM::NetLogFSM()
{

}

NetLogFSM::~NetLogFSM()
{

}

void NetLogFSM::Init()
{
	CreateState<NetLogState_EntryReady>();
	CreateState<NetLogState_LogPrint>();


	ChangeState<NetLogState_EntryReady>();
}
