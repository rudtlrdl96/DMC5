#pragma once
#include "StageBaseLevel.h"

class TestStageLevel : public StageBaseLevel
{
public:
	static TestStageLevel* GetInst()
	{
		return Inst;
	}

	// construtor destructor
	TestStageLevel();
	~TestStageLevel();

	// delete Function
	TestStageLevel(const TestStageLevel& _Other) = delete;
	TestStageLevel(TestStageLevel&& _Other) noexcept = delete;
	TestStageLevel& operator=(const TestStageLevel& _Other) = delete;
	TestStageLevel& operator=(TestStageLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeStart() override;

private:
	static TestStageLevel* Inst;
};

