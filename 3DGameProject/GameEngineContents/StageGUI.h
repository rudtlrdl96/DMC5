#pragma once
#include <GameEngineCore/GameEngineGUI.h>

class StageGUI : public GameEngineGUIWindow
{
public:
	// construtor destructor
	StageGUI();
	~StageGUI();

	// delete Function
	StageGUI(const StageGUI& _Other) = delete;
	StageGUI(StageGUI&& _Other) noexcept = delete;
	StageGUI& operator=(const StageGUI& _Other) = delete;
	StageGUI& operator=(StageGUI&& _Other) noexcept = delete;

protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> _Level, float _DeltaTime) override;

private:
	float FPSTime = 0.f;
	void FPSView(float _DeltaTime);
};

