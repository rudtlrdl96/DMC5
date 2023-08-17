#pragma once
#include "NetLogStateBase.h"

class NetLogState_LogPrint : public NetLogStateBase
{
public:
	NetLogState_LogPrint();
	~NetLogState_LogPrint();

	NetLogState_LogPrint(const NetLogState_LogPrint& _Other) = delete;
	NetLogState_LogPrint(NetLogState_LogPrint&& _Other) noexcept = delete;
	NetLogState_LogPrint& operator=(const NetLogState_LogPrint& _Other) = delete;
	NetLogState_LogPrint& operator=(const NetLogState_LogPrint&& _Other) noexcept = delete;

protected:
	void EnterState() override;
	void Update(float _DeltaTime) override;

private:
	std::string TitleName;
};

