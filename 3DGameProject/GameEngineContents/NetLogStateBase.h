#pragma once
#include "NetFsmStateBase.h"

class NetLogViewer;

class NetLogStateBase : public NetFsmStateBase
{
public:
	NetLogStateBase();
	~NetLogStateBase();

	NetLogStateBase(const NetLogStateBase& _Other) = delete;
	NetLogStateBase(NetLogStateBase&& _Other) noexcept = delete;
	NetLogStateBase& operator=(const NetLogStateBase& _Other) = delete;
	NetLogStateBase& operator=(const NetLogStateBase&& _Other) noexcept = delete;

protected:
	void Start() override;

	inline NetLogViewer* GetOwner()
	{
		if (nullptr == Owner)
		{
			MsgAssert("Owner Ptr이 nullptr입니다");
		}

		return Owner;
	}


private:
	NetLogViewer* Owner = nullptr;
};

