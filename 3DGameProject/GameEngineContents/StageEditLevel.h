#pragma once
#include "StageBaseLevel.h"

class StageEditLevel : public StageBaseLevel
{
public:
	StageEditLevel();
	~StageEditLevel();
	StageEditLevel(const StageEditLevel& _Other) = delete;
	StageEditLevel(StageEditLevel&& _Other) noexcept = delete;
	StageEditLevel& operator=(const StageEditLevel& _Other) = delete;
	StageEditLevel& operator=(StageEditLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void LevelChangeStart() override;
	void LevelChangeEnd() override;
private:
	std::shared_ptr<GameEngineGUIWindow> Editor = nullptr;
	std::shared_ptr<class EngineGrid> Grid = nullptr;
	std::shared_ptr<class FreeCameraActor> FreeCam = nullptr;
};

