#pragma once
#include "StageBase.h"

class BossStage : public StageBase
{
public:
	BossStage();
	~BossStage();
	BossStage(const BossStage& _Other) = delete;
	BossStage(BossStage&& _Other) noexcept = delete;
	BossStage& operator=(const BossStage& _Other) = delete;
	BossStage& operator=(BossStage&& _Other) noexcept = delete;



protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
};

