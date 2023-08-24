#pragma once
#include "StageBaseLevel.h"

class StartStageLevel : public StageBaseLevel
{
public:
	StageData TempData = StageData();

	StartStageLevel();
	~StartStageLevel();
	StartStageLevel(const StartStageLevel& _Other) = delete;
	StartStageLevel(StartStageLevel&& _Other) noexcept = delete;
	StartStageLevel& operator=(const StartStageLevel& _Other) = delete;
	StartStageLevel& operator=(StartStageLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeStart() override;

private:
};

