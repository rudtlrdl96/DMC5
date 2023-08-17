#pragma once
#include "NetFsmBase.h"


class NetLogFSM : public NetFsmBase
{
public:
	NetLogFSM();
	~NetLogFSM() override;

	NetLogFSM(const NetLogFSM& _Other) = delete;
	NetLogFSM(NetLogFSM&& _Other) noexcept = delete;
	NetLogFSM& operator=(const NetLogFSM& _Other) = delete;
	NetLogFSM& operator=(const NetLogFSM&& _Other) noexcept = delete;

	void Init();

protected:

private:

};

