#pragma once
#include "BaseLevel.h"

enum class Net_ActorType;

class NetworkTestLevel : public BaseLevel
{
public:
	NetworkTestLevel();
	~NetworkTestLevel();

	NetworkTestLevel(const NetworkTestLevel& _Other) = delete;
	NetworkTestLevel(NetworkTestLevel&& _Other) noexcept = delete;
	NetworkTestLevel& operator=(const NetworkTestLevel& _Other) = delete;
	NetworkTestLevel& operator=(const NetworkTestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	bool IsConnect = false;
	std::shared_ptr<class Enemy_Empusa> Enemy = nullptr;

	void InitPool();
	void InitHost(Net_ActorType _PlayerType);
	void InitClient(Net_ActorType _PlayerType);
	std::shared_ptr<class BasePlayerActor> CreatePlayer(Net_ActorType _PlayerType);
};

