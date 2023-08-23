#pragma once

class StageEditLevel : public GameEngineLevel
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
	std::shared_ptr<GameEngineGUIWindow> Edit = nullptr;
};

