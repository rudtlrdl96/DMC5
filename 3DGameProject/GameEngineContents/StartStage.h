#pragma once
#include "StageBase.h"

class StartStage : public StageBase
{
public:
	StartStage();
	~StartStage();
	StartStage(const StartStage& _Other) = delete;
	StartStage(StartStage&& _Other) noexcept = delete;
	StartStage& operator=(const StartStage& _Other) = delete;
	StartStage& operator=(StartStage&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeStart() override;
private:
};

