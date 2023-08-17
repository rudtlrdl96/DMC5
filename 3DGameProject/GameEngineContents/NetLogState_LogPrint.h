#pragma once
#include "NetLogStateBase.h"

class NetLogState_LogPrint : public NetLogStateBase
{
public:
	static void PrintLog(const std::string_view& _Text);

	NetLogState_LogPrint();
	~NetLogState_LogPrint();

	NetLogState_LogPrint(const NetLogState_LogPrint& _Other) = delete;
	NetLogState_LogPrint(NetLogState_LogPrint&& _Other) noexcept = delete;
	NetLogState_LogPrint& operator=(const NetLogState_LogPrint& _Other) = delete;
	NetLogState_LogPrint& operator=(const NetLogState_LogPrint&& _Other) noexcept = delete;

protected:
	void Start() override;
	void EnterState() override;
	void Update(float _DeltaTime) override;

private:
	static std::list<std::string> AllLog;
	static std::mutex LogLock;

	std::string TitleName;
};

