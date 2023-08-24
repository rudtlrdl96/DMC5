#pragma once
#include "StageBaseLevel.h"

class BossStageLevel : public StageBaseLevel
{
public:
	BossStageLevel();
	~BossStageLevel();
	BossStageLevel(const BossStageLevel& _Other) = delete;
	BossStageLevel(BossStageLevel&& _Other) noexcept = delete;
	BossStageLevel& operator=(const BossStageLevel& _Other) = delete;
	BossStageLevel& operator=(BossStageLevel&& _Other) noexcept = delete;



protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
};

