#pragma once
#include "StageBaseLevel.h"

class StartStageLevel : public StageBaseLevel
{
public:
	static StartStageLevel* GetInst()
	{
		return Inst;
	}

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
	static StartStageLevel* Inst;

	void TestSave();
	void TestLoad();

	static std::string GetOpenFilePath();
	static std::string GetSaveFilePath();
};

