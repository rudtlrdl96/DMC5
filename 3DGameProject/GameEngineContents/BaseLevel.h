#pragma once
#include <GameEngineCore/GameEngineLevel.h>

class BaseLevel : public GameEngineLevel
{
public:
	BaseLevel();
	~BaseLevel();

	BaseLevel(const BaseLevel& _Other) = delete;
	BaseLevel(BaseLevel&& _Other) noexcept = delete;
	BaseLevel& operator=(const BaseLevel& _Other) = delete;
	BaseLevel& operator=(const BaseLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void NetworkUpdate(float _DeltaTime) final;

private:
	float Timer = 0.f;
	const float WaitTime = 0.01f;
};

