#pragma once
#include "NetFsmStateBase.h"

class NetLogState_EntryReady : public NetFsmStateBase
{
public:
	NetLogState_EntryReady();
	~NetLogState_EntryReady() override;

	NetLogState_EntryReady(const NetLogState_EntryReady& _Other) = delete;
	NetLogState_EntryReady(NetLogState_EntryReady&& _Other) noexcept = delete;
	NetLogState_EntryReady& operator=(const NetLogState_EntryReady& _Other) = delete;
	NetLogState_EntryReady& operator=(const NetLogState_EntryReady&& _Other) noexcept = delete;

protected:

private:

};

