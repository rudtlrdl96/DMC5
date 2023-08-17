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
			MsgAssert("Owner Ptr�� nullptr�Դϴ�");
		}

		return Owner;
	}


private:
	NetLogViewer* Owner = nullptr;
};

