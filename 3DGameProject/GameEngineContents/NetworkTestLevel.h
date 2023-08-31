#pragma once
#include "BaseLevel.h"

class NetworkTestLevel : public BaseLevel
{
public:
	static NetworkTestLevel* GetInst()
	{
		return Inst;
	}

	NetworkTestLevel();
	~NetworkTestLevel();

	NetworkTestLevel(const NetworkTestLevel& _Other) = delete;
	NetworkTestLevel(NetworkTestLevel&& _Other) noexcept = delete;
	NetworkTestLevel& operator=(const NetworkTestLevel& _Other) = delete;
	NetworkTestLevel& operator=(const NetworkTestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	static NetworkTestLevel* Inst;

	bool IsConnect = false;
};

