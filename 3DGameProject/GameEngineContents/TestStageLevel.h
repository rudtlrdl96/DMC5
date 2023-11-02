#pragma once
#include "StageBaseLevel.h"

class TestStageLevel : public StageBaseLevel
{
public:
	static TestStageLevel* Inst;

	// construtor destructor
	TestStageLevel();
	~TestStageLevel();

	// delete Function
	TestStageLevel(const TestStageLevel& _Other) = delete;
	TestStageLevel(TestStageLevel&& _Other) noexcept = delete;
	TestStageLevel& operator=(const TestStageLevel& _Other) = delete;
	TestStageLevel& operator=(TestStageLevel&& _Other) noexcept = delete;

	std::shared_ptr<class BWColorEffect> GetBWEffect()
	{
		return BWEffect;
	}
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeStart() override;

private:

	std::shared_ptr<class BloomEffect> Bloom = nullptr;
	std::shared_ptr<class BWColorEffect> BWEffect = nullptr;
	std::shared_ptr<class DebugKeyUI> DebugKeyUIPtr = nullptr;
	void InitPool();
	void CreateEventZone();
};

