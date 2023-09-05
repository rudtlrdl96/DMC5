#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentsEnum.h"

class BaseLevel : public GameEngineLevel
{
public:
	BaseLevel();
	~BaseLevel();

	BaseLevel(const BaseLevel& _Other) = delete;
	BaseLevel(BaseLevel&& _Other) noexcept = delete;
	BaseLevel& operator=(const BaseLevel& _Other) = delete;
	BaseLevel& operator=(const BaseLevel&& _Other) noexcept = delete;

	inline Net_LevelType GetNetLevelType() const
	{
		return NetLevelType;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void NetworkUpdate(float _DeltaTime) final;

	inline void SetNetLevelType(Net_LevelType _Type)
	{
		NetLevelType = _Type;
	}

private:
	float Timer = 0.f;
	const float WaitTime = 0.01f;

	Net_LevelType NetLevelType = Net_LevelType::UNKNOWN;
};

