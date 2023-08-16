#pragma once

class NetFsmBase;

class NetFsmStateBase : public std::enable_shared_from_this<NetFsmStateBase>
{
	friend NetFsmBase;

public:
	NetFsmStateBase();
	virtual ~NetFsmStateBase() = 0;

	NetFsmStateBase(const NetFsmStateBase& _Other) = delete;
	NetFsmStateBase(NetFsmStateBase&& _Other) noexcept = delete;
	NetFsmStateBase& operator=(const NetFsmStateBase& _Other) = delete;
	NetFsmStateBase& operator=(const NetFsmStateBase&& _Other) noexcept = delete;

protected:
	inline NetFsmBase* GetFSM() const
	{
		return FsmPtr;
	}

	virtual void Start() {}
	virtual void EnterState();
	virtual void Update(float _DeltaTime);
	virtual void ExitState() {}

	inline float GetLiveTime() const
	{
		return Timer;
	}

	inline void ResetLiveTime()
	{
		Timer = 0.f;
	}


private:
	NetFsmBase* FsmPtr = nullptr;
	std::string Name;
	float Timer = 0.f;
};

